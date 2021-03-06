///////////////////////////////////////////////////////////////////////
// CheckOutMgr.cpp - Implements the CheckOutMgr APIs                 //
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

#include "CheckOutMgr.h"
#include "CheckOutMgrTests.h"
#include "../VersionMgr/SingleDigitVersionMgr.h"
#include "../ResourcePropertiesDb/ResourcePropertiesDb.h"
#include "../RepoStore/FileSystemStore.h"
#include "../FileSystemDemo/FileSystem.h"

using namespace SoftwareRepository;

/////////////////////////////////////////////////////////////////////
// CheckOutMgr methods

//----< validates a checkout request and loads the file from repo store >---------------------

bool CheckOutMgr::checkout(ResourceIdentity resourceId, ResourceVersion version,
    AuthorId requestorId, CheckoutLocation location, bool fetchDependencies)
{
    clearFailures();

    // if resource does not exist; return false
    if (!pPropsDb_->exists(resourceId))
    {
        addFailure(FAILURE_CODE::RESOURCE_NOT_FOUND);
        return false;
    }

    // get the resource properties from props DB
    // and create a FileResource with the namespace and name details
    IResourceProperties<FileResourcePayload>& props = pPropsDb_->get(resourceId, version);
    FileResource res(props.getNamespace(), props.getName());
    res.setPackageName(props.getRawPayload().getPackageName());

    // load the file to specified location
    // if it fails; return false
    if (!pStore_->load(res, version, location))
    {
        addFailure(FAILURE_CODE::STORE_LOAD_FAILED);
        return false;
    }

    // this flag will be used to keep track of any failures when loading dependencies
    bool successful = true;

    // if dependencies are required then checkout the dependencies
    if (fetchDependencies)
    {
        CheckoutResultProcessor checkoutProcessor(*this, requestorId, location);
        pPropsDb_->executeQuery(res, version, { checkoutProcessor });
    }

    // if any failures in loading dependencies; return false
    if (!successful)
    {
        addFailure(FAILURE_CODE::STORE_LOAD_DEP_FAILED);
        return false;
    }

    // all is good; return true
    return true;
}

/////////////////////////////////////////////////////////////////////
// CheckoutResultProcessor methods

//----< functor which calls checkout on the provided resource version >---------------------
/*
*  - It does not do any processing for the root element [level 0; the element on which
*    checkout is originally performed] because it is expected that the caller has 
*    already taken care of it.
*/

bool CheckOutMgr::CheckoutResultProcessor::operator()(
    FileResource res, ResourceVersion version, Level level)
{
    if (level != 0)
    {
        mgr_.checkout(res.getIdentity(), version, requestorId_, location_, false);
    }
    return true;
}


///////////////////////////////////////////////////////////////////////
// test functions

using namespace SoftwareRepositoryTests;
using namespace TestCore;

// Test Helper functions

//----< populates a test properties db with test data >----------------------

CheckOutMgr::FileResources populatePropertiesDbForCheckOutTests(
    IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb)
{
    CheckOutMgr::FileResources fileResources;
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

//----< tests validations on checkout request >----------------------

bool TestCheckOutValidations::operator()()
{
    SingleDigitVersionMgr versionMgr;
    IVersionMgr *pVersionMgr = &versionMgr;

    ResourcePropertiesDb propsDb(pVersionMgr);
    IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb = &propsDb;

    FileSystemStore store;
    IRepoStore<FileResource> *pStore = &store;

    CheckOutMgr::FileResources fileResources = populatePropertiesDbForCheckOutTests(pPropsDb);
    CheckOutMgr checkoutMgr(pPropsDb, pStore);

    std::cout << "\n  >> Attempting to checkout resource '" << fileResources[5].getResourceName() <<
        "' which does not exist";
    std::string checkoutDirectory = "../Demo/checkouts";
    bool result = checkoutMgr.checkout(fileResources[5].getIdentity(), 1, "doesnotmatter", checkoutDirectory);
    if (result)
    {
        setMessage("checkout on unknown file should have failed");
        return false;
    }

    FailureReasons reasons = checkoutMgr.whyItFailed();
    if (reasons.size() != 1
        || reasons.find(FAILURE_CODE::RESOURCE_NOT_FOUND) == reasons.end())
    {
        setMessage("failure reason was not registered");
        return false;
    }

    std::cout << "\n  >> Validation failed and checkout was not performed ";
    std::cout << "\n  Failure Reason = " << reasons[FAILURE_CODE::RESOURCE_NOT_FOUND] << "\n";

    setMessage("validations for checkout");
    return true;
}

//----< tests a valid checkout request >----------------------

bool TestCheckOut::operator()()
{
    SingleDigitVersionMgr versionMgr;
    ResourcePropertiesDb propsDb(&versionMgr);
    FileSystemStore store;

    CheckOutMgr::FileResources fileResources = populatePropertiesDbForCheckOutTests(&propsDb);
    CheckOutMgr checkoutMgr(&propsDb, &store);

    std::string repoPathToFile = store.getRootFolder() + '\\' +
        FileSystemStore::getFilenameForSave(fileResources[3].getIdentity(), 1);
    if (!FileSystem::File::exists(repoPathToFile))
    {
        setMessage("test file not found in repository");
        return false;
    }

    std::cout << "\n";
    std::string checkoutDirectory = "../Demo/checkouts";
    bool result = checkoutMgr.checkout(fileResources[3].getIdentity(), 1, "doesnotmatter", checkoutDirectory);
    if (!result)
    {
        setMessage("checkout failed");
        FailureReasons reasons = checkoutMgr.whyItFailed();
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

//----< test stub >----------------------------------------------------

#ifdef TEST_CHECKOUT_MGR

int main()
{
    TestCheckOutValidations testCheckOutValidations("check-out requests should be validated");
    TestCheckOut testCheckOut("valid check-out request");

    TestSuite checkOutMgr("Testing Repo Core");
    checkOutMgr.registerEx({ testCheckOutValidations, testCheckOut });
    checkOutMgr.executeAll();

    return 0;
}

#endif // TEST_CHECKOUT_MGR