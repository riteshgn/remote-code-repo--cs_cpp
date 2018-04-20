#pragma once
//////////////////////////////////////////////////////////////////////////
// RepoBrowser.h - Implements a Browser for the Software Repository     //
// ver 1.0                                                              //
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
* ver 1.0 : 10 Mar 2018
* - first release
*/

#ifndef REPOBROWSER_H
#define REPOBROWSER_H

#include "IRepoBrowser.h"
#include "../ResourcePropertiesDb/IResourcePropertiesDb.h"
#include "../ResourcePropertiesDb/FileResourcePayload.h"
#include "../FileResource/FileResource.h"

namespace SoftwareRepository
{

    /////////////////////////////////////////////////////////////////////
    // RepoBrowser
    // - implements a browser for file resource based repository
    // - requires pointer to a properties db
    // - if no result processors are provided then prints to console

    /*class RepoBrowser : public IRepoBrowser<FileResource, FileResourcePayload>
    {
    public:
        using VisitedDeps = std::unordered_map<ResourcePropsDbKey, bool>;
        using ResultProcessor = IBrowserResultProcessor<FileResource>;
        using ResultProcessors = BrowseResultProcessors<ResultProcessor>;
        using Filter = IBrowserFilter<FileResourcePayload>;
        using Filters = BrowseFilters<Filter>;
        using FileResources = std::vector<FileResource>;

        RepoBrowser(IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb,
            bool includeConsoleProcessor = DEFAULT_INCLUDE_CONSOLE_PROCESSOR) : 
            pPropsDb_(pPropsDb), includeConsoleProcessor_(includeConsoleProcessor) {};

        virtual void browse(FileResource, ResourceVersion, ResultProcessors = {}) override;

    private:
        IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb_;
        bool includeConsoleProcessor_;
        VisitedDeps visited_;
        ConsoleResultProcessor consoleProcessor_;

        void processResource(ResourceIdentity, ResourceVersion, Level, ResultProcessors);
        void clearVisitedDeps() { visited_.clear(); };
    };*/
}

#endif // !REPOBROWSER_H
