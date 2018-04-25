#pragma once
/////////////////////////////////////////////////////////////////////////////
// ResourcePropertiesDb.h - Implements the properties object and database  //
//                          for holding the properties                     //
// ver 1.1                                                                 //
// Language:    C++, Visual Studio 2017                                    //
// Application: SoftwareRepository, CSE687 - Object Oriented Design        //
// Author:      Ritesh Nair (rgnair@syr.edu)                               //
/////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the properties db using NoSqlDb. It contains below classes:
* - ResourcePropertiesDb which provides APIs to fetch properties from the DB
*
* Required Files:
* ---------------
* IResourcePropertiesDb.h
* FileResource.h, FileResource.cpp
* ResourceProperties.h, ResourceProperties.cpp
* DbCore.h, DbCore.cpp
* RepoBrowser.h, RepoBrowser.cpp
* ResultProcessors.h
* IVersionMgr.h
*
* Maintenance History:
* --------------------
* ver 1.1 : 23 Apr 2018
* - removed resource properties and put it into its own package
* ver 1.0 : 10 Mar 2018
* - first release
*/

#ifndef RESOURCE_PROP_DB_H
#define RESOURCE_PROP_DB_H

#include "IResourcePropertiesDb.h"
#include "../FileResource/FileResource.h"
#include "../ResourceProperties/ResourceProperties.h"
#include "../../NoSqlDb/DbCore/DbCore.h"
#include "../VersionMgr/IVersionMgr.h"
#include "../RepoBrowser/RepoBrowser.h"
#include "../RepoBrowser/ResultProcessors.h"

namespace SoftwareRepository
{
    
    /////////////////////////////////////////////////////////////////////
    // ResourcePropertiesDb class
    // - implements a properties database

    class ResourcePropertiesDb : public IResourcePropertiesDb<FileResource, FileResourcePayload>
    {
    public:
        using VisitedDeps = std::unordered_map<ResourcePropsDbKey, bool>;
        using ResultProcessor = IBrowserResultProcessor<FileResource>;
        using ResultProcessors = BrowseResultProcessors<ResultProcessor>;
        using Filter = IBrowserFilter<FileResourcePayload>;
        using Filters = BrowseFilters<Filter>;
        using FileResources = std::vector<FileResource>;

        ResourcePropertiesDb(IVersionMgr *pVersionMgr) : 
            pVersionMgr_(pVersionMgr), browser_(db_) {};

        virtual bool createEntry(FileResource, AuthorId) override;
        virtual bool exists(ResourceIdentity) override;
        virtual bool exists(ResourceIdentity, ResourceVersion) override;
        virtual ResourceProperties& get(ResourceIdentity) override;
        virtual ResourceProperties& get(ResourceIdentity, ResourceVersion) override;
        virtual NoSqlDb::DbCore<FileResourcePayload>& getDb() override { return db_; };
        virtual void executeQuery(FileResource, ResourceVersion, ResultProcessors = {}, 
            bool includeConsoleProcessor = DEFAULT_INCLUDE_CONSOLE_PROCESSOR) override;
        virtual void executeQuery(Filters, ResultProcessors = {}, 
            bool includeConsoleProcessor = DEFAULT_INCLUDE_CONSOLE_PROCESSOR) override;
        virtual ResourcePropsDbSize size() override { return db_.size(); }
        virtual void showKeys() override { NoSqlDb::showKeys(db_); }
        virtual void showDb() override { NoSqlDb::showDb(db_); }

    private:
        NoSqlDb::DbCore<FileResourcePayload> db_;
        IVersionMgr *pVersionMgr_;
        RepoBrowser browser_;
        ConsoleResultProcessor consoleProcessor_;

        ResourcePropsDbKey getDbKey(ResourceIdentity);
    };
}

#endif // !RESOURCE_PROP_DB_H