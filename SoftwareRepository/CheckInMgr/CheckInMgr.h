#pragma once
//////////////////////////////////////////////////////////////////////////
// CheckInMgr.h - Interacts with Properties DB and Repository Store to  //
//                provide repository's checkin functionality            //
// ver 1.0                                                              //
// Language:    C++, Visual Studio 2017                                 //
// Application: SoftwareRepository, CSE687 - Object Oriented Design     //
// Author:      Ritesh Nair (rgnair@syr.edu)                            //
//////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the checkin semantics. It contains below classes:
* - CheckInMgr which provides APIs to check-in new resource and commit a particular resource version.
*
* Required Files:
* ---------------
* RepoCoreDefinitions.h
* RepoUtilities.h, RepoUtilities.cpp
* FileResource.h, FileResource.cpp
* IResourcePropertiesDb.h
* IRepoStore.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 10 Mar 2018
* - first release
*/

#ifndef CHECKIN_MGR_H
#define CHECKIN_MGR_H

#include "../RepoCore/RepoCoreDefinitions.h"
#include "../RepoUtilities/RepoUtilities.h"
#include "../FileResource/FileResource.h"
#include "../VersionMgr/IVersionMgr.h"
#include "../ResourceProperties/FileResourcePayload.h"
#include "../ResourcePropertiesDb/IResourcePropertiesDb.h"
#include "../RepoStore/IRepoStore.h"

namespace SoftwareRepository
{
    /////////////////////////////////////////////////////////////////////
    // CheckInMgr
    // - provides check-in and commit semantics
    // - requires pointers to a properties db and repository store
    // - It maintains a collection of any failures which occur during 
    //   the check-in or commit operation. If any API returns a false
    //   as the result, the 'whyItFailed()' API will return the saved 
    //   list

    class CheckInMgr
    {
    public:
        using FileResources = std::vector<FileResource>;

        CheckInMgr(IVersionMgr *pVersionMgr, 
            IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb,
            IRepoStore<FileResource> *pStore) :
            pVersionMgr_(pVersionMgr), pPropsDb_(pPropsDb), pStore_(pStore) {}

        bool checkIn(FileResource res, AuthorId requestorId, bool autoCommit = DEFAULT_AUTO_COMMIT);

        bool commit(ResourceIdentity, AuthorId);

        FailureReasons whyItFailed() { return reasons_; }

    private:
        IVersionMgr *pVersionMgr_;
        IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb_;
        IRepoStore<FileResource> *pStore_;
        FailureReasons reasons_;

        void clearFailures() { reasons_.clear(); }
        void addFailure(FAILURE_CODE code) { reasons_[code] = getFailureReason(code); }
    };
}

#endif // !CHECKIN_MGR_H
