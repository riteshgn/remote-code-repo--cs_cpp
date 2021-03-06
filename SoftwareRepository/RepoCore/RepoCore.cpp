///////////////////////////////////////////////////////////////////////
// RepoCore.cpp - Implements the RepoCore APIs                       //
// ver 1.1                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: SoftwareRepository, CSE687 - Object Oriented Design  //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
///////////////////////////////////////////////////////////////////////

/*
* Maintenance History:
* --------------------
* ver 1.1 : 30 Apr 2018
* - added backup and restore functionality
* - browsing with filters (query builders)
* ver 1.0 : 10 Mar 2018
* - first release
*/

#include "RepoCore.h"
#include "RepoCoreTests.h"
#include "../FileSystemDemo/FileSystem.h"
#include "../VersionMgr/SingleDigitVersionMgr.h"
#include "../RepoStore/FileSystemStore.h"
#include "../ResourcePropertiesDb/ResourcePropertiesDb.h"
#include "../RepoBrowser/RepoBrowser.h"
#include "../RepoUtilities/RepoUtilities.h"

using namespace SoftwareRepository;

/////////////////////////////////////////////////////////////////////
// RepoCore methods

//----< browse repository for the latest version of provided resource >---------------------

bool RepoCore::browse(Filters filters, ResultProcessors processors)
{
    pPropsDb_->executeQuery(filters, processors);
    return true;
}

//----< browse repository for the given version of provided resource >---------------------

bool RepoCore::browse(FileResource resource, ResourceVersion version,
    ResultProcessors processors)
{
    clearFailures();

    if (!pPropsDb_->exists(resource.getIdentity(), version))
    {
        addFailure(FAILURE_CODE::RESOURCE_NOT_FOUND);
        return false;
    }

    pPropsDb_->executeQuery(resource, version, processors);
    return true;
}

//----< checkin >---------------------

bool RepoCore::checkIn(FileResource res, AuthorId requestorId, bool autoCommit)
{
    clearFailures();

    bool result = checkInMgr_.checkIn(res, requestorId, autoCommit);

    if (!result)
    {
        FailureReasons reasons = checkInMgr_.whyItFailed();
        for (std::pair<FailureCode, FailureReason> elem : reasons)
            reasons_[elem.first] = elem.second;
    }

    // all is good; return true
    return result;
}

//----< checkout >---------------------

bool RepoCore::checkout(ResourceIdentity resourceId, ResourceVersion version,
    AuthorId requestorId, CheckoutLocation location, bool fetchDependencies)
{
    clearFailures();

    bool result = checkOutMgr_.checkout(resourceId, version, 
        requestorId, location, fetchDependencies);

    if (!result)
    {
        FailureReasons reasons = checkOutMgr_.whyItFailed();
        for (std::pair<FailureCode, FailureReason> elem : reasons)
            reasons_[elem.first] = elem.second;
    }

    // all is good; return true
    return result;
}

//----< commit >---------------------

bool RepoCore::commit(ResourceIdentity resourceId, AuthorId requestorId)
{
    clearFailures();

    bool result = checkInMgr_.commit(resourceId, requestorId);

    if (!result)
    {
        FailureReasons reasons = checkOutMgr_.whyItFailed();
        for (std::pair<FailureCode, FailureReason> elem : reasons)
            reasons_[elem.first] = elem.second;
    }

    return result;
}

//----< modify a repository resource's metadata >---------------------

IResourceProperties<FileResourcePayload>& RepoCore::modify(ResourceIdentity resourceId, AuthorId requestorId)
{
    clearFailures();

    if (!pPropsDb_->exists(resourceId))
    {
        throw std::exception(getFailureReason(FAILURE_CODE::RESOURCE_NOT_FOUND).c_str());
    }

    if (!pVersionMgr_-> hasPermission(resourceId, requestorId))
    {
        throw std::exception(getFailureReason(FAILURE_CODE::FORBIDDEN).c_str());
    }

    IResourceProperties<FileResourcePayload>& props = pPropsDb_->get(resourceId);
    if (!props.isOpen())
    {
        throw std::exception(getFailureReason(FAILURE_CODE::CLOSED_VERSION).c_str());
    }

    return props;
}

//----< loads the version manager and propsdb from specified folder >---------------------

void RepoCore::loadRepo(const SourceLocation &repoFolder)
{
    pVersionMgr_->loadDb(repoFolder + "/versionmgr_state.xml");
    pPropsDb_->loadDb(repoFolder + "/repository_state.xml");
}

//----< saves the version manager and propsdb to specified folder >---------------------

void RepoCore::saveRepo(const SourceLocation &repoFolder)
{
    pVersionMgr_->saveDb(repoFolder + "/versionmgr_state.xml");
    pPropsDb_->saveDb(repoFolder + "/repository_state.xml");
}

///////////////////////////////////////////////////////////////////////
// test functions

using namespace SoftwareRepositoryTests;
using namespace TestCore;

// Test Helper functions

//----< populates a test properties db with test data >----------------------

RepoCore::FileResources populatePropertiesDb(IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb)
{
    RepoCore::FileResources fileResources;
    fileResources.push_back(FileResource("test_ns", "test-repo-core-1.h"));
    fileResources.push_back(FileResource("test_ns", "test-repo-core-2.h"));
    fileResources.push_back(FileResource("test_ns2", "test-repo-core-3.h"));
    fileResources.push_back(FileResource("test_ns2", "test-repo-core-4.h"));
    fileResources.push_back(FileResource("test_ns2", "test-repo-core-5.h"));
    fileResources.push_back(FileResource("test_ns3", "test-repo-core-notfound.h"));
    fileResources[3].setPackageName("test_ns2_package");

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

// Test Functor Implementations

//----< tests checkin operation using checkin manager >----------------------

bool TestCheckInMgr::operator()()
{
    SingleDigitVersionMgr versionMgr;
    IVersionMgr *pVersionMgr = &versionMgr;

    ResourcePropertiesDb propsDb(pVersionMgr);
    IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb = &propsDb;

    FileSystemStore store;
    IRepoStore<FileResource> *pStore = &store;

    RepoCore::FileResources fileResources = populatePropertiesDb(pPropsDb);
    RepoCore repo(pVersionMgr, pPropsDb, pStore);

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
        
    bool result = repo.checkIn(fileResources[2], "test_ns2_owner");
    if (!result)
    {
        setMessage("check in failed");
        FailureReasons reasons = repo.whyItFailed();
        if (!reasons.empty())
        {
            std::cout << "\n  Reason(s) - ";
            for (std::pair<FailureCode, FailureReason> elem : reasons)
                std::cout << "\n    " << elem.second;
            std::cout << "\n";
        }
        return false;
    }

    setMessage("checkin");
    return true;
}

//----< tests checkout operation using checkout manager >----------------------

bool TestCheckOutMgr::operator()()
{
    SingleDigitVersionMgr versionMgr;
    IVersionMgr *pVersionMgr = &versionMgr;
    ResourcePropertiesDb propsDb(pVersionMgr);
    IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb = &propsDb;
    FileSystemStore store;
    IRepoStore<FileResource> *pStore = &store;

    RepoCore::FileResources fileResources = populatePropertiesDb(pPropsDb);
    RepoCore repo(pVersionMgr, pPropsDb, pStore);

    std::string repoPathToFile = store.getRootFolder() + '\\' +
        FileSystemStore::getFilenameForSave(fileResources[3].getIdentity(), 1);
    if (!FileSystem::File::exists(repoPathToFile))
    {
        setMessage("test file not found in repository");
        return false;
    }

    std::string checkoutDirectory = "../Demo/checkouts";
    bool result = repo.checkout(fileResources[3].getIdentity(), 1, "doesnotmatter", checkoutDirectory);
    if (!result)
    {
        setMessage("checkout failed");
        FailureReasons reasons = repo.whyItFailed();
        if (!reasons.empty())
        {
            std::cout << "\n  Reason(s) - ";
            for (std::pair<FailureCode, FailureReason> elem : reasons)
                std::cout << "\n    " << elem.second;
            std::cout << "\n";
        }
        return false;
    }

    std::string checkedoutFilePath = checkoutDirectory + '\\' +
        fileResources[3].getPackageName() + '\\' + fileResources[3].getResourceName();
    if (!FileSystem::File::exists(checkedoutFilePath))
    {
        setMessage("checked file not available under checkout directory");
        return false;
    }
    std::cout << "\n  >> Successfully checked-out file '" << fileResources[3].getResourceName() <<
        "' to location '" << checkedoutFilePath << "'\n";

    setMessage("checkout");
    return true;
}

//----< tests commit operation using checkin manager >----------------------

bool TestCheckInMgrCommit::operator()()
{
    SingleDigitVersionMgr versionMgr;
    IVersionMgr *pVersionMgr = &versionMgr;

    ResourcePropertiesDb propsDb(pVersionMgr);
    IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb = &propsDb;

    FileSystemStore store;
    IRepoStore<FileResource> *pStore = &store;

    RepoCore::FileResources fileResources = populatePropertiesDb(pPropsDb);
    RepoCore repo(pVersionMgr, pPropsDb, pStore);

    std::cout << "\n  >> Attempting to commit resource '" << fileResources[2].getResourceName();
    bool result = repo.commit(fileResources[2].getIdentity(), "test_ns2_owner");
    if (!result)
    {
        setMessage("commit failed");
        FailureReasons reasons = repo.whyItFailed();
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
    pPropsDb->showDb();
    std::cout << "\n";

    setMessage("commit");
    return true;
}

//----< tests validations on browsing operation >----------------------

bool TestRepoBrowserValidations::operator()()
{
    SingleDigitVersionMgr versionMgr;
    IVersionMgr *pVersionMgr = &versionMgr;

    ResourcePropertiesDb propsDb(pVersionMgr);
    IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb = &propsDb;

    FileSystemStore store;
    IRepoStore<FileResource> *pStore = &store;

    RepoCore::FileResources fileResources = populatePropertiesDb(pPropsDb);
    RepoCore repo(pVersionMgr, pPropsDb, pStore);

    std::cout << "\n  >> Attempting to browse resource '" << fileResources[5].getResourceName() <<
        "' which does not exist";
    bool result = repo.browse(fileResources[5], 1);
    if (result)
    {
        setMessage("browse on unknown file should have failed");
        return false;
    }

    FailureReasons reasons = repo.whyItFailed();
    if (reasons.size() != 1
        || reasons.find(FAILURE_CODE::RESOURCE_NOT_FOUND) == reasons.end())
    {
        setMessage("failure reason was not registered");
        return false;
    }

    std::cout << "\n  >> Validation failed and browse was not performed ";
    std::cout << "\n  Failure Reason = " << reasons[FAILURE_CODE::RESOURCE_NOT_FOUND] << "\n";

    setMessage("validations for browse");
    return true;
}

//----< tests repo browsing >----------------------

bool TestRepoBrowser::operator()()
{
    SingleDigitVersionMgr versionMgr;
    IVersionMgr *pVersionMgr = &versionMgr;

    ResourcePropertiesDb propsDb(pVersionMgr);
    IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb = &propsDb;

    FileSystemStore store;
    IRepoStore<FileResource> *pStore = &store;

    RepoCore::FileResources fileResources = populatePropertiesDb(pPropsDb);
    RepoCore repo(pVersionMgr, pPropsDb, pStore);

    std::cout << "\n  >> Attempting to browse resource '" << fileResources[0].getResourceName() << "'";
    bool result = repo.browse(fileResources[0], 1);
    if (!result)
    {
        setMessage("browse failed");
        FailureReasons reasons = repo.whyItFailed();
        if (!reasons.empty())
        {
            std::cout << "\n  Reason(s) - ";
            for (std::pair<FailureCode, FailureReason> elem : reasons)
                std::cout << "\n    " << elem.second;
            std::cout << "\n";
        }
        return false;
    }

    std::cout << "\n  >> Successfully browsed the file '" << fileResources[2].getResourceName();
    std::cout << "\n";

    setMessage("browse");
    return true;
}

//----< test stub >----------------------------------------------------

#ifdef TEST_REPOCORE

int main()
{
    TestCheckInMgr testCheckInMgr("valid check-in request");
    TestCheckOutMgr testCheckOutMgr("valid check-out request");
    TestCheckInMgrCommit testCheckInMgrCommit("valid commit request");
    TestRepoBrowserValidations testRepoBrowserValidations("browse requests should be validated");
    TestRepoBrowser testRepoBrowser("valid browse request");

    TestSuite repoCore("Testing Repo Core");
    repoCore.registerEx({
        testCheckInMgr, testCheckOutMgr, testCheckInMgrCommit,
        testRepoBrowserValidations, testRepoBrowser });
    repoCore.executeAll();

    return 0;
}

#endif // TEST_REPOCORE

