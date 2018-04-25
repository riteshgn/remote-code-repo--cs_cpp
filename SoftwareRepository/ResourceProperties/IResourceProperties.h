#pragma once
//////////////////////////////////////////////////////////////////////////
// IResourceProperties.h - Defines the Resource Properties interface    //
// ver 1.0                                                              //
// Language:    C++, Visual Studio 2017                                 //
// Application: SoftwareRepository, CSE687 - Object Oriented Design     //
// Author:      Ritesh Nair (rgnair@syr.edu)                            //
//////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides interface definition for
* - Resource Properties
*
* Required Files:
* ---------------
* RepoCoreDefinitions.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 20 Apr 2018
* - removed resource properties and put it into its own package
*/

#ifndef IRESOURCEPROPERTIES_H
#define IRESOURCEPROPERTIES_H

#include "../RepoCore/RepoCoreDefinitions.h"

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
}

#endif // !IRESOURCEPROPERTIES_H