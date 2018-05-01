#pragma once
//////////////////////////////////////////////////////////////////////////
// IResourcePropertiesDb.h - Defines the Properties DB interface        //
// ver 1.2                                                              //
// Language:    C++, Visual Studio 2017                                 //
// Application: SoftwareRepository, CSE687 - Object Oriented Design     //
// Author:      Ritesh Nair (rgnair@syr.edu)                            //
//////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides interface definition for
* - Resource Properties Database
* - Resource Properties
*
* Required Files:
* ---------------
* RepoCoreDefinitions.h
*
* Maintenance History:
* --------------------
* ver 1.2 : 30 Apr 2018
* - added backup and restore functionality
* ver 1.1 : 23 Apr 2018
* - removed resource properties and put it into its own package
* ver 1.0 : 10 Mar 2018
* - first release
*/

#ifndef IRESOURCE_PROPS_DB_H
#define IRESOURCE_PROPS_DB_H

#include "../RepoCore/RepoCoreDefinitions.h"
#include "../ResourceProperties/IResourceProperties.h"
#include "../RepoBrowser/IRepoBrowser.h"
#include "../../NoSqlDb/Query/Query.h"

namespace SoftwareRepository
{
    /////////////////////////////////////////////////////////////////////
    // IResourcePropertiesDb interface
    // - defines the contract for a Resource Properties DB

    template <typename T, typename P>
    class IResourcePropertiesDb
    {
    public:
        virtual bool createEntry(T, AuthorId) = 0;
        virtual bool exists(ResourceIdentity) = 0;
        virtual bool exists(ResourceIdentity, ResourceVersion) = 0;
        virtual IResourceProperties<P>& get(ResourceIdentity) = 0;
        virtual IResourceProperties<P>& get(ResourceIdentity, ResourceVersion) = 0;
        virtual NoSqlDb::DbCore<P>& getDb() = 0;
        virtual void executeQuery(T, ResourceVersion,
            BrowseResultProcessors<IBrowserResultProcessor<T>>,
            bool includeConsoleProcessor = false) = 0;
        virtual void executeQuery(BrowseFilters<IBrowserFilter<P>>, 
            BrowseResultProcessors<IBrowserResultProcessor<T>>,
            bool includeConsoleProcessor = false) = 0;
        virtual ResourcePropsDbSize size() = 0;
        virtual void showKeys() = 0;
        virtual void showDb() = 0;
        virtual void loadDb(const SourceLocation&) = 0;
        virtual void saveDb(const SourceLocation&) = 0;
    };
}

#endif // !IRESOURCE_PROPS_DB_H
