//////////////////////////////////////////////////////////////////////////
// ResourcePropertiesDb.cpp - Implements the ResourcePropertiesDb APIs  //
// ver 1.2                                                              //
// Language:    C++, Visual Studio 2017                                 //
// Application: SoftwareRepository, CSE687 - Object Oriented Design     //
// Author:      Ritesh Nair (rgnair@syr.edu)                            //
//////////////////////////////////////////////////////////////////////////

/*
* Maintenance History:
* --------------------
* ver 1.2 : 30 Apr 2018
* - added backup and restore functionality
* ver 1.1 : 23 Apr 2018
* - removed resource properties and put it into its own package
* ver 1.0 : 10 Mar 2018
* - first release
*/

#include "ResourcePropertiesDb.h"
#include "ResourcePropertiesDbTests.h"
#include "../RepoUtilities/RepoUtilities.h"
#include "../VersionMgr/SingleDigitVersionMgr.h"
#include "../RepoBrowser/BrowserFilters.h"

using namespace SoftwareRepository;
using namespace NoSqlDb;

/////////////////////////////////////////////////////////////////////
// ResourcePropertiesDb methods

//----< creates a new entry in the properties db for the given resource >---------------------------

bool ResourcePropertiesDb::createEntry(FileResource res, AuthorId authorId)
{
    ResourceVersion version = pVersionMgr_->incrementVersionAndSave(res.getIdentity(), authorId);

    FileResourcePayload payload;
    payload
        .setAuthor(authorId)
        .setVersion(version)
        .setState(RESOURCE_STATE::OPEN)
        .setNamespace(res.getNamespace())
        .setPackageName(res.getPackageName());

    for (Category category : res.getCategories())
    {
        payload.addCategory(category);
    }

    DbElementMetadata metadata;
    metadata.name(res.getResourceName());
    metadata.descrip(res.getDescription());

    for (std::pair<ResourceIdentity, ResourceVersion> deps : res.getDependencies())
    {
        metadata.addRelationship(getDbKeyForVersion(deps.first, deps.second));
    }

    DbElement<FileResourcePayload> dbElem;
    dbElem.metadata(metadata);
    dbElem.payLoad(payload);

    ResourcePropsDbKey dbKey = getDbKeyForVersion(res.getIdentity(), version);

    db_[dbKey] = dbElem;

    return true;
}

//----< checks if a given resource exists in the database and returns true; false otherwise >---------

bool ResourcePropertiesDb::exists(ResourceIdentity resourceId)
{
    ResourceVersion version = pVersionMgr_->getCurrentVersion(resourceId);
    if (version == -1)
        return false;

    return exists(resourceId, version);
}

//----< checks if a given version of a resource exists in the database >---------------------------

bool ResourcePropertiesDb::exists(ResourceIdentity resourceId, ResourceVersion version)
{
    return browser_.exists(resourceId, version);
}

//----< fetches properties for a resource from properties database >---------------------------

ResourceProperties& ResourcePropertiesDb::get(ResourceIdentity resourceId) {
    ResourceVersion version = pVersionMgr_->getCurrentVersion(resourceId);
    return get(resourceId, version);
};

//----< fetches properties for a given version of a resource from properties database >---------------------------

ResourceProperties& ResourcePropertiesDb::get(ResourceIdentity resourceId, ResourceVersion version) {
    return browser_.get(resourceId, version);
}

void ResourcePropertiesDb::executeQuery(FileResource res, ResourceVersion version, 
    ResultProcessors processors, bool includeConsoleProcessor)
{
    // if no result processors are provided then prints to console
    if (includeConsoleProcessor || processors.empty())
        processors.push_back(consoleProcessor_);

    browser_.executeQuery(res, version, processors);
}

//----< searches the properties db with given filters and executes the callback on each result >---------------------------

void ResourcePropertiesDb::executeQuery(Filters filters, ResultProcessors processors, bool includeConsoleProcessor)
{
    // if no result processors are provided then prints to console
    if (includeConsoleProcessor || processors.empty())
        processors.push_back(consoleProcessor_);

    browser_.executeQuery(filters, processors);
}

//----< returns a unique identifier for a resource based on its latest version >-----------------------------
/*
*  - Pulls the latest version for this resource from the version manager
*/

ResourcePropsDbKey ResourcePropertiesDb::getDbKey(ResourceIdentity resourceId)
{
    ResourceVersion version = pVersionMgr_->getCurrentVersion(resourceId);
    return getDbKeyForVersion(resourceId, version);
}

//----< loads db content from specified file path >-----------------------------
/*
*  - If backup file contains existsing keys in the database then the key values will be
*    overidden from the backup
*/

inline void ResourcePropertiesDb::loadDb(const SourceLocation& filePath)
{
    persistence_.importDb(filePath, false);
}

//----< saves db content to specified file path >-----------------------------

inline void ResourcePropertiesDb::saveDb(const SourceLocation& filePath)
{
    persistence_.exportDb(db_.keys(), filePath);
}

///////////////////////////////////////////////////////////////////////
// test functions

using namespace SoftwareRepositoryTests;
using namespace TestCore;

// Test Helper functions

//----< populates properties db with test data >----------------------

void populateTestData(ResourcePropertiesDb& propsDb)
{
    // -- test file 2 (CLOSED)
    FileResource res2("test_ns", "testFile2.h");
    res2.setDescription("useful file in test_ns");
    propsDb.createEntry(res2, "test_ns_owner");
    propsDb.get(res2.getIdentity()).markClosed();

    // -- test file 3 v1 (CLOSED)
    FileResource res3("test_ns", "testFile3.h");
    res3.setDescription("useful file 3v1 in test_ns");
    propsDb.createEntry(res3, "test_ns_owner");
    propsDb.get(res3.getIdentity()).markClosed();

    // -- test file 3 v2
    res3.setDescription("useful file 3v2 in test_ns");
    propsDb.createEntry(res3, "test_ns_owner");

    // -- test file
    FileResource res("test_ns", "testFile.h");
    res
        .setDescription("useful file in test_ns")
        .setDependency(res2, 1)
        .setDependency(res3, 2)
        .setCategory("utility")
        .setCategory("functionality123");
    propsDb.createEntry(res, "test_ns_owner");

    std::cout << "\n  Initialized properties DB as below\n";
    std::cout << "\n  Keys: ";
    propsDb.showKeys();
    std::cout << "\n";
    propsDb.showDb();
    std::cout << "\n";
}

//----< test adding a resource >------------------------------------------

bool testAddingOneResource(ResourcePropertiesDb& propsDb)
{
    FileResource fileResource("test_ns", "testFile.h");
    fileResource.setDescription("useful file in test_ns");
    fileResource.setCategory("utility");
    propsDb.createEntry(fileResource, "test_ns_owner");

    if (propsDb.size() != 1)
    {
        std::cout << "\n  Entry 1 was not added\n";
        return false;
    }
    if (!propsDb.exists(fileResource.getIdentity(), 1))
    {
        std::cout << "\n Entry 1 was not found \n";
        return false;
    }

    std::cout << "\n  >> Successfully created entry and checked existence for resource: ";
    std::cout << fileResource.toString();
    std::cout << "\n  >> Properties DB size after entry: " << propsDb.size() << "\n";

    return true;
}

//----< test adding a second resource >------------------------------------------

bool testAddingSecondResourceWithFirstOneAsDep(ResourcePropertiesDb& propsDb)
{
    FileResource fileResource("test_ns", "testFile.h");
    FileResource fileResource2("test_ns", "testFile2.h");
    fileResource2.setDescription("useful file 2 in test_ns");
    fileResource2.setDependency(fileResource, 1);
    propsDb.createEntry(fileResource2, "test_ns_owner");

    if (propsDb.size() != 2)
    {
        std::cout << "\n  Entry 2 was not added\n";
        return false;
    }
    if (!propsDb.exists(fileResource2.getIdentity(), 1))
    {
        std::cout << "\n  Entry 2 was not found\n";
        return false;
    }

    std::cout << "\n  >> Successfully created entry and checked existence for another resource: ";
    std::cout << fileResource2.toString();
    std::cout << "\n  >> Properties DB size after entry: " << propsDb.size() << "\n";

    return true;
}

//----< test adding another version of an existing resource >------------------------------------------

bool testAddingSecondVersionOfFirstResource(ResourcePropertiesDb& propsDb)
{
    FileResource fileResource("test_ns", "testFile.h");
    propsDb.createEntry(fileResource, "test_ns_owner");

    if (propsDb.size() != 3)
    {
        std::cout << "\n  Entry 1 v2 was not added\n";
        return false;
    }
    if (!propsDb.exists(fileResource.getIdentity(), 2))
    {
        std::cout << "\n  Entry 1 v2 was not found\n";
        return false;
    }

    std::cout << "\n  >> Successfully created entry for another version of the first resource: ";
    std::cout << "\n  >> Properties DB size after entry: " << propsDb.size() << "\n";

    return true;
}

// Test Functor Implementations

//----< test check for resource existence and creation of a new entry >------------------------------------------

bool TestResourceExistsAndCreateNewPropertiesEntry::operator()()
{
    SingleDigitVersionMgr versionMgr;
    ResourcePropertiesDb propsDb(&versionMgr);
    if (propsDb.size() > 0)
    {
        setMessage("Expected an empty DB");
        return false;
    }

    std::cout << "\n  >> Properties DB size at beginning of test: " << propsDb.size() << "\n";

    if (!testAddingOneResource(propsDb))
    {
        setMessage("Entry 1 could not be created");
        return false;
    }

    if (!testAddingSecondResourceWithFirstOneAsDep(propsDb))
    {
        setMessage("Entry 2 could not be created");
        return false;
    }

    if (!testAddingSecondVersionOfFirstResource(propsDb))
    {
        setMessage("Entry 1 v2 could not be created");
        return false;
    }


    std::cout << "\n  >> Printing entries from properties db ";
    FileResource fileResource("test_ns", "testFile.h");
    FileResource fileResource2("test_ns", "testFile2.h");
    std::cout << propsDb.get(fileResource.getIdentity(), 1).toString();
    std::cout << propsDb.get(fileResource2.getIdentity(), 1).toString();
    std::cout << propsDb.get(fileResource.getIdentity(), 2).toString();

    setMessage("resource exists and creation of new resource properties");
    return true;
}

//----< tests executing a query on the properties db >----------------------

/////////////////////////////////////////////////////////////////////
// ConsoleResultProcessor
// - prints each navigation step on the standard console output

class Accumulator : public IBrowserResultProcessor<FileResource>
{
public:
    int count() { return noOfRecords_; }
    virtual bool operator()(FileResource res, ResourceVersion version, Level level) override {
        noOfRecords_++;
        return true;
    };
private:
    int noOfRecords_ = 0;
};

bool TestExecuteQuery::operator()()
{
    SingleDigitVersionMgr versionMgr;
    ResourcePropertiesDb propsDb(&versionMgr);
    populateTestData(propsDb);

    std::cout << "\n  Attempting to browse all files with filename = 'testFile3.h'";

    FilenameFilter filter = FilenameFilter::create("testFile3.h");
    ResourcePropertiesDb::Filters filters = { filter };
    Accumulator resultAccumulator;
    propsDb.executeQuery(filters, { resultAccumulator }, true);

    if (resultAccumulator.count() != 2)
    {
        setMessage("Query failed");
        return false;
    }

    std::cout << "\n  Browse Query executed successfully (" << std::to_string(resultAccumulator.count()) << " records found)\n";

    setMessage("executing nosqldb::query");
    return true;
}

//----< test stub >----------------------------------------------------

#ifdef TEST_RESOURCE_PROPS_DB

int main()
{
    TestResourceExistsAndCreateNewPropertiesEntry testResourceExistsAndCreateNewPropertiesEntry("resource exists and create properties for new entry");
    TestExecuteQuery testExecuteQuery("executing nosqldb::query on the properties db");

    TestSuite resourcePropsDb("Testing Resource Properties DB");
    resourcePropsDb.registerEx({
        testResourceExistsAndCreateNewPropertiesEntry,
        testExecuteQuery });
    resourcePropsDb.executeAll();

    return 0;
}

#endif // TEST_RESOURCE_PROPS_DB

