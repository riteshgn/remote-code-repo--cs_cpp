#pragma once
/////////////////////////////////////////////////////////////////////////////
// ResourceProperties.h - Implements the properties object                 //
// ver 1.0                                                                 //
// Language:    C++, Visual Studio 2017                                    //
// Application: SoftwareRepository, CSE687 - Object Oriented Design        //
// Author:      Ritesh Nair (rgnair@syr.edu)                               //
/////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the properties db using NoSqlDb. It contains below classes:
* - ResourceProperties which provides APIs to interact with a Db Object
*
* Required Files:
* ---------------
* IResourceProperties.h
* DbCore.h, DbCore.cpp
* FileResourcePayload.h, FileResourcePayload.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 23 Apr 2018
* - first release
*/

#ifndef RESOURCEPROPERTIES_H
#define RESOURCEPROPERTIES_H

#include "IResourceProperties.h"
#include "../ResourceProperties/FileResourcePayload.h"
#include "../../NoSqlDb/DbCore/DbCore.h"

namespace SoftwareRepository
{
    /////////////////////////////////////////////////////////////////////
    // ResourceProperties class
    // - The Db object

    class ResourceProperties : public IResourceProperties<FileResourcePayload>
    {
    public:
        ResourceProperties(NoSqlDb::DbCore<FileResourcePayload>& db)
            : db_(db), dbKey_("__dummy__") {}

        ResourceProperties(NoSqlDb::DbCore<FileResourcePayload>& db,
            ResourcePropsDbKey dbKey) : db_(db), dbKey_(dbKey) {}

        ResourceProperties& operator=(const ResourceProperties& props)
        {
            if (&props == this)
                return *this;

            db_ = props.db_;
            dbKey_ = props.dbKey_;

            return *this;
        }

        // methods to access data from the db element

        AuthorId& getAuthorId() { return db_[dbKey_].payLoad().getAuthor(); }
        AuthorId getAuthorId() const { return db_[dbKey_].payLoad().getAuthor(); }

        Categories& getCategories() { return db_[dbKey_].payLoad().getCategories(); }
        Categories getCategories() const { return db_[dbKey_].payLoad().getCategories(); }

        Dependencies getDependencies() const;

        ResourceDescription& getDescription() { return db_[dbKey_].metadata().descrip(); }
        ResourceDescription getDescription() const { return db_[dbKey_].metadata().descrip(); }

        ResourceName& getName() { return db_[dbKey_].metadata().name(); }
        ResourceName getName() const { return db_[dbKey_].metadata().name(); }

        Namespace& getNamespace() { return db_[dbKey_].payLoad().getNamespace(); }
        Namespace getNamespace() const { return db_[dbKey_].payLoad().getNamespace(); }

        FileResourcePayload getRawPayload() { return db_[dbKey_].payLoad(); }

        // methods to set data to the db element

        ResourceProperties& addCategory(Category);
        ResourceProperties& addCategory(Categories);

        ResourceProperties& addDependency(ResourceIdentity, ResourceVersion);
        ResourceProperties& addDependency(Dependencies);

        ResourceProperties& markClosed() { return mark(RESOURCE_STATE::CLOSED); };
        ResourceProperties& markPendingClose() { return mark(RESOURCE_STATE::CLOSE_PENDING); };

        // methods to query the db element

        bool isOpen() const;
        bool areDependenciesClosed() const;
        Dependencies getOpenDependencies() const;

        // methods to remove values from the db element

        ResourceProperties& removeCategory(Category);
        ResourceProperties& removeCategory(Categories);

        ResourceProperties& removeDependency(ResourceIdentity, ResourceVersion);
        ResourceProperties& removeDependency(Dependencies);

        // stringify to view in human readable form

        std::string toString() const;

    private:
        NoSqlDb::DbCore<FileResourcePayload>& db_;
        ResourcePropsDbKey dbKey_;
        AuthorId requestorId_;

        ResourceIdentitiesWithVersion convertDepStringToMap(std::string depStr) const;
        ResourceProperties& mark(State);
    };
}

#endif // !RESOURCEPROPERTIES_H