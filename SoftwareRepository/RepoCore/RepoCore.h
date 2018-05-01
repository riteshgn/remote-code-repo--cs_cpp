#pragma once
///////////////////////////////////////////////////////////////////////
// RepoCore.h - Implements the Software repository prototype         //
// ver 1.1                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: SoftwareRepository, CSE687 - Object Oriented Design  //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides the core framework for the software repository. It contains
* - RepoCore which provides all repository functionalities including
*   checkin, checkout, commit and browsing.
* - It also exposes the Properties DB APIs which can be used to set useful
*   metadata like category and dependeycy information.
*
* Required Files:
* ---------------
* RepoCoreDefinitions.h
* FileResource.h, FileResource.cpp
* IResourcePropertiesDb.h
* IRepoStore.h
* IVersionMgr.h
* IRepoBrowser.h
* CheckInMgr.h, CheckInMgr.cpp
* CheckOutMgr.h, CheckOutMgr.cpp
*
* Maintenance History:
* --------------------
* ver 1.1 : 30 Apr 2018
* - added backup and restore functionality
* ver 1.0 : 23 Feb 2018
* - first release
*/

#ifndef REPO_CORE_H
#define REPO_CORE_H

#include "RepoCoreDefinitions.h"
#include "../FileResource/FileResource.h"
#include "../ResourceProperties/FileResourcePayload.h"
#include "../ResourcePropertiesDb/IResourcePropertiesDb.h"
#include "../RepoStore/IRepoStore.h"
#include "../VersionMgr/IVersionMgr.h"
#include "../RepoBrowser/IRepoBrowser.h"
#include "../CheckInMgr/CheckInMgr.h"
#include "../CheckOutMgr/CheckOutMgr.h"

namespace SoftwareRepository
{
    /////////////////////////////////////////////////////////////////////
    // RepoCore
    // Implements the core repository features

    class RepoCore
    {
    public:
        using FileResources = std::vector<FileResource>;
        using ResultProcessor = IBrowserResultProcessor<FileResource>;
        using ResultProcessors = BrowseResultProcessors<ResultProcessor>;

        RepoCore(IVersionMgr *pVersionMgr, 
            IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb,
            IRepoStore<FileResource> *pStore) :
            pVersionMgr_(pVersionMgr),
            pPropsDb_(pPropsDb),
            pStore_(pStore),
            checkInMgr_(pVersionMgr, pPropsDb, pStore),
            checkOutMgr_(pPropsDb, pStore) {}

        bool browse(FileResource, 
            BrowseResultProcessors<IBrowserResultProcessor<FileResource>> = {});
        bool browse(FileResource, ResourceVersion, 
            BrowseResultProcessors<IBrowserResultProcessor<FileResource>> = {});

        bool checkIn(FileResource, AuthorId, bool autoCommit = DEFAULT_AUTO_COMMIT);

        bool checkout(ResourceIdentity, ResourceVersion, AuthorId,
            CheckoutLocation, bool fetchDependencies = DEFAULT_FETCH_DEPENDENCIES);

        bool commit(ResourceIdentity, AuthorId);

        IResourceProperties<FileResourcePayload>& modify(ResourceIdentity, AuthorId);

        void loadRepo(const SourceLocation&);
        void saveRepo(const SourceLocation&);

        FailureReasons whyItFailed() { return reasons_; };

    private:
        IVersionMgr *pVersionMgr_;
        IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb_;
        IRepoStore<FileResource> *pStore_;
        CheckInMgr checkInMgr_;
        CheckOutMgr checkOutMgr_;
        FailureReasons reasons_;

        void clearFailures() { reasons_.clear(); };
        void addFailure(FAILURE_CODE code) { reasons_[code] = getFailureReason(code); };
    };
}

#endif // !REPO_CORE_H
