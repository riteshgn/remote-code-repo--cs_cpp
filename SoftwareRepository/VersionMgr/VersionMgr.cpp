///////////////////////////////////////////////////////////////////////////
// SingleDigitVersionMgr.cpp - Implements the SingleDigitVersionMgr APIs //
// ver 1.1                                                               //
// Language:    C++, Visual Studio 2017                                  //
// Application: SoftwareRepository, CSE687 - Object Oriented Design      //
// Author:      Ritesh Nair (rgnair@syr.edu)                             //
///////////////////////////////////////////////////////////////////////////

/*
* Maintenance History:
* --------------------
* ver 1.1 : 30 Apr 2018
* - SingleDigitVersion implements the IPayload interface and can now be persisted
* - added backup and restore functionality
* ver 1.0 : 10 Mar 2018
* - first release
*/

#include "SingleDigitVersionMgr.h"
#include "SingleDigitVersionMgrTests.h"

using namespace SoftwareRepository;
using namespace NoSqlDb;
using namespace XmlProcessing;

/////////////////////////////////////////////////////////////////////
// SingleDigitVersion methods

//----< converts the payload to an xml element >---------------------

IPayload<SingleDigitVersion>::Sptr SingleDigitVersion::toXmlElement()
{
    IPayload<SingleDigitVersion>::Sptr pPayload = makeTaggedElement("payload");

    pPayload->addChild(makeTaggedElement("author", authorId_));
    pPayload->addChild(makeTaggedElement("currentVersion", std::to_string(currentVersion_)));

    return pPayload;
}

//----< converts from an xml element to the payload >---------------------

SingleDigitVersion SingleDigitVersion::fromXmlElement(IPayload<SingleDigitVersion>::Sptr pPayloadElem)
{
    SingleDigitVersion payload;

    for (auto pValueChild : pPayloadElem->children())
    {
        if (pValueChild->tag() == "author"
            && pValueChild->children().size() > 0)
        {
            payload.setAuthorId(pValueChild->children()[0]->value());
        }
        else if (pValueChild->tag() == "currentVersion"
            && pValueChild->children().size() > 0)
        {
            payload.setCurrentVersion(std::stoi(pValueChild->children()[0]->value()));
        }
    }

    return payload;
}

//----< stringifies the payload >---------------------

std::string SingleDigitVersion::toString() const
{
    return "Current Version: [" + std::to_string(currentVersion_) + "], Author: [" + authorId_;
}

/////////////////////////////////////////////////////////////////////
// SingleDigitVersionMgr methods

//----< gets the current version for a resource >---------------------

ResourceVersion SingleDigitVersionMgr::getCurrentVersion(ResourceIdentity resourceId)
{
    if (!db_.contains(resourceId))
        return -1;

    return db_[resourceId].payLoad().getCurrentVersion();
}

//----< gets the next version for a resource >---------------------

ResourceVersion SingleDigitVersionMgr::getNextVersion(ResourceIdentity resourceId)
{
    if (!db_.contains(resourceId))
        return -1;

    return (db_[resourceId].payLoad().getCurrentVersion() + 1);
}

//----< saves the next version for the resource >---------------------

ResourceVersion SingleDigitVersionMgr::incrementVersionAndSave(ResourceIdentity resourceId, AuthorId authorId)
{
    ResourceVersion newVersion = getNextVersion(resourceId);
    if (newVersion == -1)
    {
        SingleDigitVersion version;
        version.setCurrentVersion(1);
        version.setAuthorId(authorId);
        NoSqlDb::DbElement<SingleDigitVersion> elem;
        elem.payLoad(version);
        db_[resourceId] = elem;

        return 1;
    }

    db_[resourceId].payLoad().setCurrentVersion(newVersion);
    return newVersion;
}

//----< checks if resource has a particular version and returns true; false otherwise >---------------------

bool SingleDigitVersionMgr::isValidVersion(ResourceIdentity resourceId, ResourceVersion version)
{
    return (db_.contains(resourceId)
        && version > 0
        && version <= db_[resourceId].payLoad().getCurrentVersion());
}

//----< returns true if user has permission for requested action >---------------------------

bool SingleDigitVersionMgr::hasPermission(ResourceIdentity resourceId, AuthorId authorId, Action action)
{
    // if db does not contain a record for this resource ID
    // then treat the author as the owner for the resource

    return (!db_.contains(resourceId)
        || authorId == db_[resourceId].payLoad().getAuthorId());
}

//----< loads db content from specified file path >-----------------------------
/*
*  - If backup file contains existsing keys in the database then the key values will be
*    overidden from the backup
*/

inline void SingleDigitVersionMgr::loadDb(const SourceLocation& filePath)
{
    persistence_.importDb(filePath, false);
}

//----< saves db content to specified file path >-----------------------------

inline void SingleDigitVersionMgr::saveDb(const SourceLocation& filePath)
{
    persistence_.exportDb(db_.keys(), filePath);
}

///////////////////////////////////////////////////////////////////////
// test functions

using namespace SoftwareRepositoryTests;
using namespace TestCore;

using ExceptionMsgFn = std::function<void(AbstractTest::TestMsg)>;

// Test Functor Implementations

//----< tests adding a new version and fetching the current version >----------------------

bool TestAddingNewEntryAndFetchingCurrentVersion::operator()()
{
    SingleDigitVersionMgr versionMgr;
    std::cout << "\n  >> Number of keys in version db at the beginning of the test: " << versionMgr.size();

    ResourceIdentity resourceId = "test_ns::testFile1.h";
    ResourceVersion version = versionMgr.getCurrentVersion(resourceId);
    if (version != -1)
    {
        setMessage("Found version for a resource which doesnt exist");
        return false;
    }
    std::cout << "\n  >> Fetching current version for resource ID '" << resourceId << "' returns result = -1\n";
 
    versionMgr.incrementVersionAndSave(resourceId, "rgnair");
    std::cout << "\n  >> Added version entry for a new resource with ID " << resourceId;
    std::cout << "\n  >> Number of keys in version db: " << versionMgr.size();

    version = versionMgr.getCurrentVersion(resourceId);
    if (version != 1)
    {
        setMessage("Current version not found; v1");
        return false;
    }
    std::cout << "\n  >> Fetching current version for resource ID '" << resourceId << "' returns result = 1\n";

    versionMgr.incrementVersionAndSave(resourceId, "rgnair");
    std::cout << "\n  >> Incremented version for resource with ID: " << resourceId;
    std::cout << "\n  >> Number of keys in version db: " << versionMgr.size();

    version = versionMgr.getCurrentVersion(resourceId);
    if (version != 2)
    {
        setMessage("Current version not found; v2");
        return false;
    }
    std::cout << "\n  >> Fetching current version for resource ID '" << resourceId << "' returns result = 2\n";

    setMessage("add / increment entry and fetch current version");
    return true;
}

//----< tests fetching the next version for a resource >----------------------

bool TestFetchingNextVersion::operator()()
{
    SingleDigitVersionMgr versionMgr;
    ResourceIdentity resourceId1 = "test_ns::testFile1.h";
    ResourceIdentity resourceId2 = "test_ns::testFile2.h";
    ResourceIdentity resourceId3 = "test_ns::testFile3.h";
    versionMgr.incrementVersionAndSave(resourceId2, "rgnair");
    versionMgr.incrementVersionAndSave(resourceId3, "rgnair");
    versionMgr.incrementVersionAndSave(resourceId3, "rgnair");

    std::cout << "\n  >> Number of keys in version db at the beginning of the test: " << versionMgr.size();
    std::cout << "\n  >> Keys: ";
    versionMgr.showKeys();
    std::cout << "\n";

    ResourceVersion version = versionMgr.getNextVersion(resourceId1);
    if (version != -1)
    {
        setMessage("Found next version for a file that does not exist");
        return false;
    }
    std::cout << "\n  >> Fetching next version for resource ID '" << resourceId1 << 
        "' which does not exist in DB returns result = -1\n";

    version = versionMgr.getNextVersion(resourceId2);
    if (version != 2)
    {
        setMessage("Incorrect version found for resouce 2");
        return false;
    }
    std::cout << "\n  >> Fetching next version for resource ID '" << resourceId2 << 
        "' which has current version = 1 in DB returns result = 2\n";

    version = versionMgr.getNextVersion(resourceId3);
    if (version != 3)
    {
        setMessage("Incorrect version found for resouce 3");
        return false;
    }
    std::cout << "\n  >> Fetching next version for resource ID '" << resourceId3 << 
        "' which has current version = 2 in DB returns result = 3\n";

    setMessage("fetch next version");
    return true;
}

//----< verifies various versioning scenarios for valid version checks >----------------------

bool verifyValidVersionChanges(SingleDigitVersionMgr& versionMgr,
    ResourceIdentities resourceIds, ExceptionMsgFn setMessage)
{
    if (versionMgr.isValidVersion(resourceIds[0], 1))
    {
        setMessage("Returned as valid version for key which does not exist");
        return false;
    }
    std::cout << "\n  >> Checking valid version for resource ID '" << resourceIds[0] <<
        "' which does not exist in DB returns result = false\n";

    if (versionMgr.isValidVersion(resourceIds[1], -1))
    {
        setMessage("Returned as valid version for a negative verison");
        return false;
    }
    std::cout << "\n  >> Checking if version -1 is valid for resource ID '" << resourceIds[1] <<
        "' which has current version = 1 in DB returns result = false\n";

    if (!versionMgr.isValidVersion(resourceIds[2], 1))
    {
        setMessage("Should have returned valid version for 1");
        return false;
    }
    std::cout << "\n  >> Checking if version 1 is valid for resource ID '" << resourceIds[2] <<
        "' which has current version = 2 in DB returns result = true\n";

    if (!versionMgr.isValidVersion(resourceIds[2], 2))
    {
        setMessage("Should have returned valid version for 2");
        return false;
    }
    std::cout << "\n  >> Checking if version 2 is valid for resource ID '" << resourceIds[2] <<
        "' which has current version = 2 in DB returns result = true\n";

    if (versionMgr.isValidVersion(resourceIds[2], 3))
    {
        setMessage("Should have returned invalid version for 3");
        return false;
    }
    std::cout << "\n  >> Checking if version 3 is valid for resource ID '" << resourceIds[2] <<
        "' which has current version = 2 in DB returns result = false\n";

    return true;
}

//----< tests verifying valid version for a resource >----------------------

bool TestCheckingValidVersion::operator()()
{
    SingleDigitVersionMgr versionMgr;
    ResourceIdentity resourceId1 = "test_ns::testFile1.h";
    ResourceIdentity resourceId2 = "test_ns::testFile2.h";
    ResourceIdentity resourceId3 = "test_ns::testFile3.h";
    versionMgr.incrementVersionAndSave(resourceId2, "rgnair");
    versionMgr.incrementVersionAndSave(resourceId3, "rgnair");
    versionMgr.incrementVersionAndSave(resourceId3, "rgnair");

    std::cout << "\n  >> Number of keys in version db at the beginning of the test: " << versionMgr.size();
    std::cout << "\n  >> Keys: ";
    versionMgr.showKeys();
    std::cout << "\n";

    ExceptionMsgFn setMsg = [this](AbstractTest::TestMsg msg) { setMessage(msg); };
    verifyValidVersionChanges(versionMgr,
        { resourceId1, resourceId2, resourceId3 }, setMsg);

    setMessage("check valid version");
    return true;
}

//----< test stub >----------------------------------------------------

#ifdef TEST_VERSIONMGR

int main()
{
    TestAddingNewEntryAndFetchingCurrentVersion testAddingNewEntryAndFetchingCurrentVersion("adding / incrementing entry and fetching latest version");
    TestFetchingNextVersion testFetchingNextVersion("fetching next version");
    TestCheckingValidVersion testCheckingValidVersion("checking valid version");

    TestSuite singleDigitVerMgr("Testing Integer Version Manager");
    singleDigitVerMgr.registerEx({ testAddingNewEntryAndFetchingCurrentVersion,
        testFetchingNextVersion, testCheckingValidVersion });
    singleDigitVerMgr.executeAll();

    return 0;
}

#endif // TEST_VERSIONMGR

