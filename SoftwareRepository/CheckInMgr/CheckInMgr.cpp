///////////////////////////////////////////////////////////////////////
// CheckInMgr.cpp - Implements the CheckInMgr APIs                   //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: SoftwareRepository, CSE687 - Object Oriented Design  //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
///////////////////////////////////////////////////////////////////////

/*
* Maintenance History:
* --------------------
* ver 1.0 : 10 Mar 2018
* - first release
*/

#include "CheckInMgr.h"
#include "CheckInMgrTests.h"
#include "../VersionMgr/SingleDigitVersionMgr.h"
#include "../ResourcePropertiesDb/ResourcePropertiesDb.h"
#include "../RepoStore/FileSystemStore.h"
#include "../FileSystemDemo/FileSystem.h"

using namespace SoftwareRepository;

/////////////////////////////////////////////////////////////////////
// CheckInMgr methods

//----< validates a checkin request, makes necessary entries in properties db and save the file to repo store >---------------------

bool CheckInMgr::checkIn(FileResource res, AuthorId requestorId, bool autoCommit)
{
    clearFailures();

    // if resource exists in DB and requestor does not have permission
    // on the resource return false
    if (!pVersionMgr_->hasPermission(res.getIdentity(), requestorId))
    {
        addFailure(FAILURE_CODE::FORBIDDEN);
        return false;
    }

    // if new repo resource or if current version of the resource is not open
    // create a new entry in properties DB
    if (!pPropsDb_->exists(res.getIdentity())
        || !pPropsDb_->get(res.getIdentity()).isOpen())
        pPropsDb_->createEntry(res, requestorId);

    // add categories and dependencies to the open version
    IResourceProperties<FileResourcePayload>& properties = pPropsDb_->get(res.getIdentity());
    properties.addDependency(res.getDependencies());
    properties.addCategory(res.getCategories());

    // save the file to repository backend
    // if it fails return false
    if (!pStore_->save(res, pVersionMgr_->getCurrentVersion(res.getIdentity()), res.getSourceLocation()))
    {
        addFailure(FAILURE_CODE::STORE_SAVE_FAILED);
        return false;
    }

    // all is good; return true
    return true;
}

//----< validates a commit request and marks appropriate status in properties db >----------------------

bool CheckInMgr::commit(ResourceIdentity resourceId, AuthorId requestorId)
{
    clearFailures();

    if (!pPropsDb_->exists(resourceId))
    {
        addFailure(FAILURE_CODE::RESOURCE_NOT_FOUND);
        return false;
    }

    if (!pVersionMgr_->hasPermission(resourceId, requestorId))
    {
        addFailure(FAILURE_CODE::FORBIDDEN);
        return false;
    }

    IResourceProperties<FileResourcePayload>& props = pPropsDb_->get(resourceId);
    if (!props.areDependenciesClosed())
    {
        props.markPendingClose();

        addFailure(FAILURE_CODE::OPEN_DEPENDENCIES_PENDING_CLOSE);
        return false;
    }

    props.markClosed();
    return true;
}

///////////////////////////////////////////////////////////////////////
// test functions

using namespace SoftwareRepositoryTests;
using namespace TestCore;

using CheckInTestsSetMsgFn = std::function<void(AbstractTest::TestMsg)>;

// Test Helper functions

//----< populates a test properties db with test data >----------------------

CheckInMgr::FileResources populatePropertiesDbForCheckInTests(IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb)
{
    CheckInMgr::FileResources fileResources;
    fileResources.push_back(FileResource("test_ns", "test-repo-core-1.h"));
    fileResources.push_back(FileResource("test_ns", "test-repo-core-2.h"));
    fileResources.push_back(FileResource("test_ns2", "test-repo-core-3.h"));
    fileResources.push_back(FileResource("test_ns2", "test-repo-core-4.h"));
    fileResources.push_back(FileResource("test_ns2", "test-repo-core-5.h"));
    fileResources.push_back(FileResource("test_ns3", "test-repo-core-notfound.h"));

    pPropsDb->createEntry(fileResources[0], "test_ns_owner");
    pPropsDb->createEntry(fileResources[1], "test_ns_owner");
    pPropsDb->createEntry(fileResources[2], "test_ns2_owner");
    pPropsDb->createEntry(fileResources[3], "test_ns2_owner");
    pPropsDb->createEntry(fileResources[4], "test_ns2_owner");

    std::vector<int> resourcesToClose = { 1, 3, 4 };
    for (int indx : resourcesToClose)
        pPropsDb->get(fileResources[indx].getIdentity()).markClosed();

    pPropsDb->get(fileResources[1].getIdentity())
        .addDependency(fileResources[3].getIdentity(), 1);

    pPropsDb->get(fileResources[0].getIdentity())
        .addDependency({ { fileResources[1].getIdentity(), 1 },{ fileResources[2].getIdentity(), 1 } });

    std::cout << "\n  Initialized properties DB as below\n";
    std::cout << "\n Keys:";
    pPropsDb->showKeys();
    std::cout << "\n";
    pPropsDb->showDb();
    std::cout << "\n";

    return fileResources;
}

//----< verifies that the API result is a failure and the correct failure reason is set >----------------------

bool hasCheckInOperationFailed(CheckInMgr& checkInMgr, bool result, std::string errorMsg,
    FailureCode expectedCode, CheckInTestsSetMsgFn setMessage)
{
    if (result)
    {
        setMessage(errorMsg);
        return true;
    }

    FailureReasons reasons = checkInMgr.whyItFailed();
    if (reasons.size() != 1
        || reasons.find(expectedCode) == reasons.end())
    {
        setMessage("failure reason was not registered");
        return true;
    }

    return false;
}

//----< verifies if the properties db is correctly updated after a check-in operation >----------------------

bool verifyPropsDbUpdatesAfterCheckin(IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb,
    FileSystemStore& store, CheckInMgr::FileResources& fileResources,
    CheckInTestsSetMsgFn setMessage)
{
    Categories categories = pPropsDb->get(fileResources[2].getIdentity(), 1).getCategories();
    if (categories.size() != 1)
    {
        setMessage("category not set");
        return false;
    }

    Dependencies dependencies = pPropsDb->get(fileResources[2].getIdentity(), 1).getDependencies();
    if (dependencies.size() != 1)
    {
        setMessage("dependency not set");
        return false;
    }

    std::string repoPathToFile = store.getRootFolder() + '\\' +
        FileSystemStore::getFilenameForSave(fileResources[2].getIdentity(), 1);
    if (!FileSystem::File::exists(repoPathToFile))
    {
        setMessage("file not saved to repository");
        return false;
    }

    std::cout << "\n  >> Successfully checked-in file '" << fileResources[2].getResourceName() <<
        "' along with adding category 'cat3' and dependency '" << fileResources[4].getIdentity() << " v1'.";
    std::cout << "\n  >> File stored in repository at " << repoPathToFile;
    std::cout << "\n";
    pPropsDb->showDb();
    std::cout << "\n";

    return true;
}

// Test Functor Implementations

//----< tests validations on check-in request >----------------------

bool TestCheckInValidations::operator()()
{
    SingleDigitVersionMgr versionMgr;
    ResourcePropertiesDb propsDb(&versionMgr);
    FileSystemStore store;
    IRepoStore<FileResource> *pStore = &store;

    CheckInMgr::FileResources fileResources = populatePropertiesDbForCheckInTests(&propsDb);
    CheckInMgr checkInMgr(&versionMgr, &propsDb, pStore);

    std::cout << "\n  >> Attempting to check-in file '" << fileResources[2].getResourceName() <<
        "' using user 'test_ns_owner'";
    bool result = checkInMgr.checkIn(fileResources[2], "test_ns_owner");

    CheckInTestsSetMsgFn setMsg = [this](AbstractTest::TestMsg msg) { setMessage(msg); };

    if (hasCheckInOperationFailed(checkInMgr, result, "checkin by non-owner should have failed",
        FAILURE_CODE::FORBIDDEN, setMsg))
        return false;

    std::cout << "\n  >> Validation failed and checkIn was not performed ";
    std::cout << "\n  Failure Reason = " << getFailureReason(FAILURE_CODE::FORBIDDEN) << "\n";

    setMessage("validations for checkin");
    return true;
}

//----< tests a valid check-in request >----------------------

bool TestCheckIn::operator()()
{
    SingleDigitVersionMgr versionMgr;
    ResourcePropertiesDb propsDb(&versionMgr);
    FileSystemStore store;
    IRepoStore<FileResource> *pStore = &store;

    CheckInMgr::FileResources fileResources = populatePropertiesDbForCheckInTests(&propsDb);
    CheckInMgr checkInMgr(&versionMgr, &propsDb, pStore);

    std::string filePath = "../Demo/code/Projects/TreeWalkDemo/TreeWalkDemo.cpp";
    if (!FileSystem::File::exists(filePath))
    {
        setMessage("Test file not found on FS");
        return false;
    }

    std::cout << "\n  >> Attempting to check-in file '" << fileResources[2].getResourceName() <<
        "' with source at " << filePath;
    fileResources[2]
        .setSourceLocation(filePath)
        .setDependency(fileResources[4], 1)
        .setCategory("cat3");

    bool result = checkInMgr.checkIn(fileResources[2], "test_ns2_owner");
    if (!result)
    {
        setMessage("check in failed");
        FailureReasons reasons = checkInMgr.whyItFailed();
        if (!reasons.empty())
        {
            std::cout << "\n  Reason(s) - ";
            for (std::pair<FailureCode, FailureReason> elem : reasons)
                std::cout << "\n    " << elem.second;
            std::cout << "\n";
        }
        return false;
    }

    CheckInTestsSetMsgFn setMsg = [this](AbstractTest::TestMsg msg) { setMessage(msg); };
    if (!verifyPropsDbUpdatesAfterCheckin(&propsDb, store, fileResources, setMsg))
        return false;

    setMessage("check-in");
    return true;
}

//----< tests validations on commit request >----------------------

bool TestCommitValidations::operator()()
{
    SingleDigitVersionMgr versionMgr;
    ResourcePropertiesDb propsDb(&versionMgr);
    FileSystemStore store;
    IRepoStore<FileResource> *pStore = &store;

    CheckInMgr::FileResources fileResources = populatePropertiesDbForCheckInTests(&propsDb);
    CheckInMgr checkInMgr(&versionMgr, &propsDb, pStore);

    std::cout << "\n  >> Attempting to commit resource '" << fileResources[5].getResourceName() <<
        "' which does not exist";
    bool result = checkInMgr.commit(fileResources[5].getIdentity(), "doesnotmatter");

    CheckInTestsSetMsgFn setMsg = [this](AbstractTest::TestMsg msg) { setMessage(msg); };
    if (hasCheckInOperationFailed(checkInMgr, result, "commit on unknown file should have failed",
        FAILURE_CODE::RESOURCE_NOT_FOUND, setMsg))
        return false;

    std::cout << "\n  >> Validation failed and commit was not performed ";
    std::cout << "\n  Failure Reason = " << getFailureReason(FAILURE_CODE::RESOURCE_NOT_FOUND) << "\n";

    std::cout << "\n  >> Attempting to commit file '" << fileResources[2].getResourceName() <<
        "' using user 'test_ns_owner'";
    result = checkInMgr.commit(fileResources[2].getIdentity(), "test_ns_owner");
    if (hasCheckInOperationFailed(checkInMgr, result, "commit by non-owner should have failed",
        FAILURE_CODE::FORBIDDEN, setMsg))
        return false;

    std::cout << "\n  >> Validation failed and commit was not performed ";
    std::cout << "\n  Failure Reason = " << getFailureReason(FAILURE_CODE::FORBIDDEN) << "\n";

    std::cout << "\n  >> Attempting to commit file '" << fileResources[0].getResourceName() <<
        "' which has open dependency '" << fileResources[2].getResourceName() << "'";
    result = checkInMgr.commit(fileResources[0].getIdentity(), "test_ns_owner");
    if (hasCheckInOperationFailed(checkInMgr, result, "checkin with open dependencies should have failed",
        FAILURE_CODE::OPEN_DEPENDENCIES_PENDING_CLOSE, setMsg))
        return false;

    std::cout << "\n  >> Validation failed and checkIn was not performed ";
    std::cout << "\n  Failure Reason = " << getFailureReason(FAILURE_CODE::OPEN_DEPENDENCIES_PENDING_CLOSE) << "\n";
    propsDb.showDb();
    std::cout << "\n";

    setMessage("validations for commit");
    return true;
}

//----< tests a valid commit request >----------------------

bool TestCommit::operator()()
{
    SingleDigitVersionMgr versionMgr;
    ResourcePropertiesDb propsDb(&versionMgr);
    FileSystemStore store;
    IRepoStore<FileResource> *pStore = &store;

    CheckInMgr::FileResources fileResources = populatePropertiesDbForCheckInTests(&propsDb);
    CheckInMgr checkInMgr(&versionMgr, &propsDb, pStore);

    std::cout << "\n  >> Attempting to commit resource '" << fileResources[2].getResourceName();
    bool result = checkInMgr.commit(fileResources[2].getIdentity(), "test_ns2_owner");
    if (!result)
    {
        setMessage("commit failed");
        FailureReasons reasons = checkInMgr.whyItFailed();
        if (!reasons.empty())
        {
            std::cout << "\n  Reason(s) - ";
            for (std::pair<FailureCode, FailureReason> elem : reasons)
                std::cout << "\n    " << elem.second;
            std::cout << "\n";
        }
        return false;
    }

    std::cout << "\n  >> Successfully committed file '" << fileResources[2].getResourceName();
    std::cout << "\n";
    propsDb.showDb();
    std::cout << "\n";

    setMessage("commit");
    return true;
}

//----< test stub >----------------------------------------------------

#ifdef TEST_CHECKIN_MGR

int main()
{
    TestCheckInValidations testCheckInValidations("check-in requests should be validated");
    TestCheckIn testCheckIn("valid check-in request");

    TestSuite checkInMgr("Testing Repo Core");
    checkInMgr.registerEx({ testCheckInValidations, testCheckIn });
    checkInMgr.executeAll();

    return 0;
}


#endif // TEST_CHECKIN_MGR
