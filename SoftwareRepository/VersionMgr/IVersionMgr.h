#pragma once
/////////////////////////////////////////////////////////////////////////////
// IVersionMgr.h - Defines contract for a Version Manager which maintains  //
//                 the Respositorie's resource versioning and ownership    //
//                 policy                                                  //
// ver 1.0                                                                 //
// Language:    C++, Visual Studio 2017                                    //
// Application: SoftwareRepository, CSE687 - Object Oriented Design        //
// Author:      Ritesh Nair (rgnair@syr.edu)                               //
/////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* - For the Software Repository, Resource is an object which will be versioned
*   and maintainted by the RepoCore.
* - The Repository requires a Version Manager which will be responsible
*   for maintaining Resource versions and provide features like fetching the latest 
*   version for a resource, getting the next version number for the resource, etc.
* - It also requires the manager to take care of ownership for the resource.
* - The IVersionMgr interface defined in this pacakage is responsible for laying out
*   the API contract to be provided by such a Version Manager implementation.
*
* Required Files:
* ---------------
* RepoCoreDefinitions.h
*
* Maintenance History:
* --------------------
* ver 1.2 : 30 Apr 2018
* - added backup and restore functionality
* ver 1.0 : 24 Feb 2018
* - first release
*/

#ifndef IVERSIONMGR_H
#define IVERSIONMGR_H

#include "../RepoCore/RepoCoreDefinitions.h"

namespace SoftwareRepository
{
    /////////////////////////////////////////////////////////////////////
    // IVersionMgr interface
    // - defines the contract for a Version Manager

    class IVersionMgr
    {
    public:
        virtual ResourceVersion getCurrentVersion(ResourceIdentity) = 0;
        virtual ResourceVersion getNextVersion(ResourceIdentity) = 0;
        virtual ResourceVersion incrementVersionAndSave(ResourceIdentity, AuthorId) = 0;
        virtual bool isValidVersion(ResourceIdentity, ResourceVersion) = 0;
        virtual bool hasPermission(ResourceIdentity, AuthorId, Action = DEFAULT_RESOURCE_MODIFY_ACTION) = 0;
        virtual void loadDb(const SourceLocation&) = 0;
        virtual void saveDb(const SourceLocation&) = 0;
    };
}

#endif // !IVERSIONMGR_H