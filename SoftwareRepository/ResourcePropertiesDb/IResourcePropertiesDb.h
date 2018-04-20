#pragma once
//////////////////////////////////////////////////////////////////////////
// IResourcePropertiesDb.h - Defines the Properties DB interface        //
// ver 1.0                                                              //
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
* ver 1.0 : 10 Mar 2018
* - first release
*/

#ifndef IRESOURCE_PROPS_DB_H
#define IRESOURCE_PROPS_DB_H

#include "../RepoCore/RepoCoreDefinitions.h"
#include "../RepoBrowser/IRepoBrowser.h"
#include "../../NoSqlDb/DbCore/DbCore.h"
#include "../../NoSqlDb/Query/Query.h"

namespace SoftwareRepository
{
    /////////////////////////////////////////////////////////////////////
    // IResourceProperties interface
    // - defines the contract for a properties object

    template <typename P>
    class IResourceProperties
    {
    public:
        // methods to access data from the db element

        virtual AuthorId& getAuthorId() = 0;
        virtual AuthorId getAuthorId() const = 0;

        virtual Categories& getCategories() = 0;
        virtual Categories getCategories() const = 0;

        virtual Dependencies getDependencies() const = 0;

        virtual ResourceDescription& getDescription() = 0;
        virtual ResourceDescription getDescription() const = 0;

        virtual ResourceName& getName() = 0;
        virtual ResourceName getName() const = 0;

        virtual Namespace& getNamespace() = 0;
        virtual Namespace getNamespace() const = 0;

        virtual P getRawPayload() = 0;

        // methods to set data to the db element

        virtual IResourceProperties& addCategory(Category) = 0;
        virtual IResourceProperties& addCategory(Categories) = 0;

        virtual IResourceProperties& addDependency(ResourceIdentity, ResourceVersion) = 0;
        virtual IResourceProperties& addDependency(Dependencies) = 0;

        virtual IResourceProperties& markClosed() = 0;
        virtual IResourceProperties& markPendingClose() = 0;

        // methods to query the db element

        virtual bool isOpen() const = 0;
        virtual bool areDependenciesClosed() const = 0;
        virtual Dependencies getOpenDependencies() const = 0;

        // methods to remove values from the db element

        virtual IResourceProperties& removeCategory(Category) = 0;
        virtual IResourceProperties& removeCategory(Categories) = 0;

        virtual IResourceProperties& removeDependency(ResourceIdentity, ResourceVersion) = 0;
        virtual IResourceProperties& removeDependency(Dependencies) = 0;

        // stringify to view in human readable form

        virtual std::string toString() const = 0;
    };

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
    };
}

#endif // !IRESOURCE_PROPS_DB_H
