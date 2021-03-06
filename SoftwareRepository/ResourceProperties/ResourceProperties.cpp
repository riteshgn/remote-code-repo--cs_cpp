//////////////////////////////////////////////////////////////////////////
// ResourceProperties.cpp - Implements the ResourcePropertiesDb APIs    //
// ver 1.1                                                              //
// Language:    C++, Visual Studio 2017                                 //
// Application: SoftwareRepository, CSE687 - Object Oriented Design     //
// Author:      Ritesh Nair (rgnair@syr.edu)                            //
//////////////////////////////////////////////////////////////////////////

/*
* Maintenance History:
* --------------------
* ver 1.1 : 30 Apr 2018
* - FileResourcePayload implements the IPayload interface and can now be persisted
* ver 1.0 : 23 Apr 2018
* - first release
*/

#include "ResourceProperties.h"
#include "ResourcePropertiesTests.h"
#include "../RepoUtilities/RepoUtilities.h"

using namespace SoftwareRepository;
using namespace NoSqlDb;
using namespace XmlProcessing;

/////////////////////////////////////////////////////////////////////
// FileResourcePayload methods

//----< converts the payload to an xml element >---------------------

IPayload<FileResourcePayload>::Sptr FileResourcePayload::toXmlElement()
{
    IPayload<FileResourcePayload>::Sptr pPayload = makeTaggedElement("payload");

    pPayload->addChild(makeTaggedElement("author", author_));
    pPayload->addChild(makeTaggedElement("state", STATE_STRINGS[state_]));
    pPayload->addChild(makeTaggedElement("namespace", namespace_));
    pPayload->addChild(makeTaggedElement("package", package_));
    pPayload->addChild(makeTaggedElement("version", std::to_string(version_)));

    IPayload<FileResourcePayload>::Sptr pCategories = makeTaggedElement("categories");
    for (Category category : categories_)
    {
        pCategories->addChild(makeTaggedElement("category", category));
    }
    pPayload->addChild(pCategories);

    return pPayload;
}

//----< converts from an xml element to the payload >---------------------

FileResourcePayload FileResourcePayload::fromXmlElement(IPayload<FileResourcePayload>::Sptr pPayloadElem)
{
    FileResourcePayload payload;

    for (auto pValueChild : pPayloadElem->children())
    {
        if (pValueChild->tag() == "author"
            && pValueChild->children().size() > 0)
        {
            payload.setAuthor(pValueChild->children()[0]->value());
        }
        else if (pValueChild->tag() == "state"
            && pValueChild->children().size() > 0)
        {
            payload.setState(reverseStateLookup()[pValueChild->children()[0]->value()]);
        }
        else if (pValueChild->tag() == "namespace"
            && pValueChild->children().size() > 0)
        {
            payload.setNamespace(pValueChild->children()[0]->value());
        }
        else if (pValueChild->tag() == "package"
            && pValueChild->children().size() > 0)
        {
            payload.setPackageName(pValueChild->children()[0]->value());
        }
        else if (pValueChild->tag() == "version"
            && pValueChild->children().size() > 0)
        {
            payload.setVersion(std::stoi(pValueChild->children()[0]->value()));
        }
        else if (pValueChild->tag() == "categories")
        {
            for (auto pCategory : pValueChild->children())
            {
                payload.addCategory(pCategory->children()[0]->value());
            }
        }
    }

    return payload;
}

//----< stringifies the payload >---------------------

std::string FileResourcePayload::toString() const
{
    return "Version: [" + std::to_string(version_) + "], Author: [" + author_ +
        "], State: [" + stringifyResourceState(state_) + "], " +
        "Package: [" + package_ + "], Categories: " + stringifyCategories();
}

//----< stringifies the categories >---------------------

std::string FileResourcePayload::stringifyCategories() const
{
    return toCommaSeparatedString<Category>(categories_);
}

/////////////////////////////////////////////////////////////////////
// ResourceProperties methods

//----< converts the db representation of dependencies into the unordered_map representation >-------------

ResourceIdentitiesWithVersion ResourceProperties::convertDepStringToMap(std::string depStr) const
{
    int pos = depStr.find_last_of(PREFIX_DBKEY_VERSION);

    // uncomment below lines for some debug logs
    /*std::cout << "\n depStr: " << depStr << " pos: " << pos << "\n";
    std::cout << "\n 1: " << depStr.substr(0, pos) << "\n";
    std::cout << "\n 2: " << depStr.substr(pos + 1) << "\n\n";*/

    return { { depStr.substr(0, pos), std::stoi(depStr.substr(pos + 1)) } };
}

//----< returns all existing dependencies >-------------------------------------

Dependencies ResourceProperties::getDependencies() const {
    std::vector<std::string> deps = db_[dbKey_].metadata().children();
    Dependencies dependencies;
    for (std::string depStr : deps)
    {
        ResourceIdentitiesWithVersion idsWithVersion = convertDepStringToMap(depStr);
        for (std::pair<ResourceIdentity, ResourceVersion> element : idsWithVersion)
        {
            dependencies.push_back(element);
        }
    }
    return dependencies;
}

//----< adds a category to the payload >-------------------------------------

ResourceProperties& ResourceProperties::addCategory(Category category)
{
    db_[dbKey_].payLoad().addCategory(category);
    return *this;
}

//----< adds multiple categories to the payload >---------------------------

ResourceProperties& ResourceProperties::addCategory(Categories categories)
{
    for (Category category : categories)
    {
        addCategory(category);
    }
    return *this;
}

//----< adds a dependency to the metadata >---------------------------

ResourceProperties& ResourceProperties::addDependency(ResourceIdentity resourceId, ResourceVersion version)
{
    std::string depKey = getDbKeyForVersion(resourceId, version);
    db_[dbKey_].metadata().addRelationship(depKey);
    return *this;
}

//----< adds multiple dependencies to the metadata >---------------------------

ResourceProperties& ResourceProperties::addDependency(Dependencies dependencies) {
    for (std::pair<ResourceIdentity, ResourceVersion> element : dependencies)
    {
        addDependency(element.first, element.second);
    }
    return *this;
}

//----< sets resource state to provided state in the payload >---------------------------

ResourceProperties& ResourceProperties::mark(State state)
{
    db_[dbKey_].payLoad().setState(state);
    return *this;
}

//----< checks if the resource is in open state and returns true; false otherwise >---------------------------

bool ResourceProperties::isOpen() const
{
    return (RESOURCE_STATE::OPEN == db_[dbKey_].payLoad().getState());
}

//----< checks if all dependencies are closed and return true; false otherwise >---------------------------

bool ResourceProperties::areDependenciesClosed() const {
    std::vector<std::string> deps = db_[dbKey_].metadata().children();
    for (std::string dep : deps)
    {
        if (RESOURCE_STATE::OPEN == db_[dep].payLoad().getState())
            return false;
    }

    return true;
}

//----< gets list of open dependencies >---------------------------

Dependencies ResourceProperties::getOpenDependencies() const {
    Dependencies openDeps;

    std::vector<std::string> deps = db_[dbKey_].metadata().children();
    for (std::string dep : deps)
    {
        if (RESOURCE_STATE::OPEN == db_[dep].payLoad().getState())
        {
            ResourceIdentitiesWithVersion depWithVer = ResourceProperties::convertDepStringToMap(dep);
            for (std::pair<ResourceIdentity, ResourceVersion> element : depWithVer)
            {
                openDeps.push_back(element);
            }
        }

    }

    return openDeps;
}

//----< removes a category from the payload >---------------------------

ResourceProperties& ResourceProperties::removeCategory(Category category)
{
    db_[dbKey_].payLoad().removeCategory(category);
    return *this;
}

//----< removes multiple categories from the payload >---------------------------

ResourceProperties& ResourceProperties::removeCategory(Categories categories)
{
    for (Category category : categories)
    {
        removeCategory(category);
    }
    return *this;
}

//----< removes a dependency from the metadata >---------------------------

ResourceProperties& ResourceProperties::removeDependency(ResourceIdentity resourceId, ResourceVersion version)
{
    ResourcePropsDbKey depKey = getDbKeyForVersion(resourceId, version);
    db_[dbKey_].metadata().removeRelationship(depKey);
    return *this;
}

//----< removes multiple dependencies from the metadata >---------------------------

ResourceProperties& ResourceProperties::removeDependency(Dependencies dependencies)
{
    for (std::pair<ResourceIdentity, ResourceVersion> element : dependencies)
    {
        removeDependency(element.first, element.second);
    }
    return *this;
}

//----< stringifies the file resource >---------------------

std::string ResourceProperties::toString() const
{
    return R"(
      Properties =>
      --  Key                    )" + dbKey_ + R"(
      --  Description            )" + getDescription() + R"(
      --  Author                 )" + getAuthorId() + R"(
      --  Categories             )" + stringifyCategories(getCategories()) + R"(
      --  Dependencies           )" + stringifyDependencies(getDependencies()) + R"(
      --  isOpen                 )" + std::to_string(isOpen()) + R"(
      --  areDependenciesClosed  )" + std::to_string(areDependenciesClosed()) + R"(
      --  getOpenDependencies    )" + stringifyDependencies(getOpenDependencies()) + "\n\n";
}

///////////////////////////////////////////////////////////////////////
// test functions

using namespace SoftwareRepositoryTests;
using namespace TestCore;
using namespace NoSqlDb;

// Test Helper functions

//----< populates a test properties db with test data >----------------------

void populateTestData(DbCore<FileResourcePayload>& db)
{
    // -- test file
    DbElementMetadata metadata;
    metadata.descrip("useful file in test_ns");
    metadata.name("testFile.h");
    metadata.addRelationship("test_ns::testFile2.h#1").addRelationship("test_ns::testFile3.h#2");

    FileResourcePayload payload;
    payload.setAuthor("test_ns_owner").setState(RESOURCE_STATE::OPEN);
    payload.addCategory("utility").addCategory("functionality123");

    DbElement<FileResourcePayload> elem;
    elem.metadata(metadata);
    elem.payLoad(payload);
    db["test_ns::testFile.h#1"] = elem;

    // -- test file 2
    DbElementMetadata metadata2;
    metadata2.descrip("useful file 2 in test_ns");
    metadata2.name("testFile2.h");
    FileResourcePayload payload2;
    payload2.setAuthor("test_ns_owner").setState(RESOURCE_STATE::CLOSED);
    elem.metadata(metadata2);
    elem.payLoad(payload2);
    db["test_ns::testFile2.h#1"] = elem;

    // -- test file 3 v1
    metadata2.descrip("useful file 3v1 in test_ns");
    metadata2.name("testFile3.h");
    payload2.setAuthor("test_ns_owner").setState(RESOURCE_STATE::CLOSED);
    elem.metadata(metadata2);
    elem.payLoad(payload2);
    db["test_ns::testFile3.h#1"] = elem;

    // -- test file 3 v2
    metadata2.descrip("useful file 3v2 in test_ns");
    metadata2.name("testFile3.h");
    payload2.setAuthor("test_ns_owner").setState(RESOURCE_STATE::OPEN);
    elem.metadata(metadata2);
    elem.payLoad(payload2);
    db["test_ns::testFile3.h#2"] = elem;

    std::cout << "\n  Initialized properties DB as below\n";
    std::cout << "\n  Keys: ";
    showKeys(db);
    std::cout << "\n";
    showDb(db);
    std::cout << "\n";
}

//----< tests fetching properties for a given resource >----------------------

bool TestFetchPropertiesForAFileResource::operator()()
{
    DbCore<FileResourcePayload> db;
    populateTestData(db);
    std::string dbKey = "test_ns::testFile.h#1";
    ResourceProperties properties(db, dbKey);
    if (properties.getAuthorId() != "test_ns_owner")
    {
        setMessage("Author not set");
        return false;
    }
    if (properties.getCategories().size() != 2)
    {
        setMessage("Categories not set");
        return false;
    }
    if (properties.getDependencies().size() != 2)
    {
        setMessage("Dependencies not set");
        return false;
    }
    if (properties.getDescription() != "useful file in test_ns")
    {
        setMessage("Description not set");
        return false;
    }
    if (!properties.isOpen())
    {
        setMessage("State not set");
        return false;
    }
    if (properties.areDependenciesClosed())
    {
        setMessage("Dependency state check incorrect");
        return false;
    }
    if (properties.getOpenDependencies().size() != 1)
    {
        setMessage("Could not fetch open dependencies");
        return false;
    }
    std::cout << "\n  >> Fetch details for key: " << dbKey;
    std::cout << properties.toString();
    setMessage("fetch properties for file resource");
    return true;
}

//----< tests adding dependency >----------------------

bool testAddingDependency(DbCore<FileResourcePayload>& db)
{
    ResourceProperties testFile3V1Properties(db, "test_ns::testFile3.h#1");
    testFile3V1Properties.addDependency({ { "test_ns::testFile2.h", 1 } });
    if (db["test_ns::testFile3.h#1"].metadata().children().size() != 1)
        return false;

    std::cout << "\n  >> Added dependency on testFile2 v1 to testFile3 v1 \n";
    showDb(db);
    std::cout << "\n";

    return true;
}

//----< tests adding a category and commiting the version >----------------------

bool testAddingCategoryAndCommit(DbCore<FileResourcePayload>& db)
{
    ResourceProperties testFile3V2Properties(db, "test_ns::testFile3.h#2");
    testFile3V2Properties.addCategory("io").addCategory("server");
    testFile3V2Properties.markClosed();
    if (db["test_ns::testFile3.h#2"].payLoad().getCategories().size() != 2)
    {
        std::cout << "\n  Adding categories did not work\n";
        return false;
    }
    if (db["test_ns::testFile3.h#2"].payLoad().getState() != RESOURCE_STATE::CLOSED)
    {
        std::cout << "\n  Commit did not work\n";
        return false;
    }

    std::cout << "\n  >> Added categories for testFile3 v2 and committed it \n";
    showDb(db);
    std::cout << "\n";

    return true;
}

//----< tests removing category and dependency >----------------------

bool testRemoveCategoryAndDependency(DbCore<FileResourcePayload>& db)
{
    ResourceProperties testFileV1Properties(db, "test_ns::testFile.h#1");
    testFileV1Properties.removeCategory("functionality123");
    testFileV1Properties.removeDependency("test_ns::testFile2.h", 1);

    if (db["test_ns::testFile.h#1"].payLoad().getCategories().size() != 1)
    {
        std::cout << "Removing categories did not work";
        return false;
    }
    if (db["test_ns::testFile.h#1"].metadata().children().size() != 1)
    {
        std::cout << "Removing dependencies did not work";
        return false;
    }

    std::cout << "\n  >> Removed 'functionality123' as category and dependency on " <<
        "'test_ns::testFile2.h, v1' for 'test_ns::testFile.h, v1' \n";
    showDb(db);
    std::cout << "\n";

    return true;
}

//----< tests modifying a resource's properties >----------------------

bool TestModifyFileResourceProperties::operator()()
{
    DbCore<FileResourcePayload> db;
    populateTestData(db);

    if (!testAddingDependency(db))
    {
        setMessage("Adding dependencies did not work");
        return false;
    }

    if (!testAddingCategoryAndCommit(db))
    {
        setMessage("Adding categories amd commit did not work");
        return false;
    }

    if (!testRemoveCategoryAndDependency(db))
    {
        setMessage("Removing categories and dependencies did not work");
        return false;
    }

    setMessage("modify resource properties");
    return true;
}

//----< test stub >----------------------------------------------------

#ifdef TEST_RESOURCEPROPERTIES

int main()
{
    TestFetchPropertiesForAFileResource testFetchPropertiesForAFileResource("fetching file resource properties");
    TestModifyFileResourceProperties testModifyFileResourceProperties("modifying file resource properties");
    TestSuite resourceProps("Testing Resource Properties");
    resourceProps.registerEx({
        testFetchPropertiesForAFileResource,
        testModifyFileResourceProperties });
    resourceProps.executeAll();

    return 0;
}

#endif // TEST_RESOURCEPROPERTIES

