#pragma once
//////////////////////////////////////////////////////////////////////////
// RepoBrowser.h - Implements a Browser for the Software Repository     //
// ver 1.1                                                              //
// Language:    C++, Visual Studio 2017                                 //
// Application: SoftwareRepository, CSE687 - Object Oriented Design     //
// Author:      Ritesh Nair (rgnair@syr.edu)                            //
//////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the IRepoBrowser interface to provide browser semantics. 
* It contains below classes:
* - RepoBrowser which implements a browser for file-based resources
* - ConsoleResultProcessor which implements a console output result processor
*
* Required Files:
* ---------------
* IRepoBrowser.h
* IResourcePropertiesDb.h
* FileResource.h, FileResource.cpp
*
* Maintenance History:
* --------------------
* ver 1.1 : 24 Apr 2018
* - implemented new browser semnatics
* ver 1.0 : 10 Mar 2018
* - first release
*/

#ifndef REPOBROWSER_H
#define REPOBROWSER_H

#include "IRepoBrowser.h"
#include "../FileResource/FileResource.h"
#include "../ResourceProperties/FileResourcePayload.h"
#include "../ResourceProperties/ResourceProperties.h"
#include "../../NoSqlDb/DbCore/DbCore.h"

namespace SoftwareRepository
{

    /////////////////////////////////////////////////////////////////////
    // RepoBrowser
    // - implements a browser for file resource based repository
    // - requires db associated with the repository

    class RepoBrowser : public IBrowseable<FileResource, FileResourcePayload>
    {
    public:
        using VisitedDeps = std::unordered_map<ResourcePropsDbKey, bool>;
        using ResultProcessor = IBrowserResultProcessor<FileResource>;
        using ResultProcessors = BrowseResultProcessors<ResultProcessor>;
        using Filter = IBrowserFilter<FileResourcePayload>;
        using Filters = BrowseFilters<Filter>;

        RepoBrowser(NoSqlDb::DbCore<FileResourcePayload>& db) : db_(db) {};

        virtual bool exists(ResourceIdentity, ResourceVersion) override;
        virtual ResourceProperties& get(ResourceIdentity, ResourceVersion) override;
        virtual void executeQuery(FileResource, ResourceVersion, ResultProcessors) override;
        virtual void executeQuery(Filters, ResultProcessors) override;

    private:
        NoSqlDb::DbCore<FileResourcePayload>& db_;
        bool includeConsoleProcessor_;
        VisitedDeps visited_;
        ResourceProperties currProp_ = ResourceProperties(db_);

        void processResource(ResourceIdentity resourceId,
            ResourceVersion version, Level level,
            ResultProcessors processors);
        void clearVisitedDeps() { visited_.clear(); };
    };
}

#endif // !REPOBROWSER_H
