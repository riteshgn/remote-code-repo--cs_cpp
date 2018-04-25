#pragma once
//////////////////////////////////////////////////////////////////////////
// CheckOutMgr.h - Interacts with Properties DB and Repository Store to //
//                 provide repository's checkout functionality          //
// ver 1.0                                                              //
// Language:    C++, Visual Studio 2017                                 //
// Application: SoftwareRepository, CSE687 - Object Oriented Design     //
// Author:      Ritesh Nair (rgnair@syr.edu)                            //
//////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the checkout semantics. It contains below classes:
* - CheckOutMgr which provides APIs to checkout a resource to specified location
*
* Required Files:
* ---------------
* RepoCoreDefinitions.h
* RepoUtilities.h, RepoUtilities.cpp
* FileResource, FileResource.cpp
* IResourcePropertiesDb.h
* IRepoStore.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 10 Mar 2018
* - first release
*/

#ifndef CHECKOUT_MGR_H
#define CHECKOUT_MGR_H

#include "../RepoCore/RepoCoreDefinitions.h"
#include "../ResourcePropertiesDb/IResourcePropertiesDb.h"
#include "../ResourceProperties/FileResourcePayload.h"
#include "../RepoStore/IRepoStore.h"
#include "../RepoBrowser/RepoBrowser.h"
#include "../RepoUtilities/RepoUtilities.h"
#include "../FileResource/FileResource.h"

namespace SoftwareRepository
{
    /////////////////////////////////////////////////////////////////////
    // CheckOutMgr class
    // - provides checkout semantics
    // - requires pointers to a properties db and repository store
    // - It maintains a collection of any failures which occur during 
    //   the checkout operation. If any API returns a false as the result, 
    //   the 'whyItFailed()' API will return the saved list.

    class CheckOutMgr
    {
    public:
        using FileResources = std::vector<FileResource>;

        CheckOutMgr(IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb,
            IRepoStore<FileResource> *pStore) :
            pPropsDb_(pPropsDb), pStore_(pStore) {}

        bool checkout(ResourceIdentity, ResourceVersion, AuthorId,
            CheckoutLocation, bool fetchDependencies = DEFAULT_FETCH_DEPENDENCIES);
        FailureReasons whyItFailed() { return reasons_; }

    private:
        class CheckoutResultProcessor;

        IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb_;
        IRepoStore<FileResource> *pStore_;
        FailureReasons reasons_;

        void clearFailures() { reasons_.clear(); }
        void addFailure(FAILURE_CODE code) { reasons_[code] = getFailureReason(code); }
    };

    /////////////////////////////////////////////////////////////////////
    // CheckoutResultProcessor class
    // - this class implements the browser's result processor interface
    // - it is a private class for CheckOutMgr which utilizes the browser
    //   to traverse the dependency graph and checkout all dependencies 
    //   to the designated folder

    class CheckOutMgr::CheckoutResultProcessor : public IBrowserResultProcessor<FileResource>
    {
    public:
        CheckoutResultProcessor(CheckOutMgr& mgr, AuthorId requestorId, 
            CheckoutLocation location) :
            mgr_(mgr), requestorId_(requestorId), location_(location) {}

        virtual bool operator()(FileResource, ResourceVersion, Level) override;

    private:
        CheckOutMgr& mgr_;
        AuthorId requestorId_;
        CheckoutLocation location_;
    };
}

#endif // !CHECKOUT_MGR_H
