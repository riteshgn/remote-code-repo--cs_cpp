#pragma once
//////////////////////////////////////////////////////////////////////////
// IRepoBrowser.h - Defines the RepoBrowser interface                   //
// ver 1.1                                                              //
// Language:    C++, Visual Studio 2017                                 //
// Application: SoftwareRepository, CSE687 - Object Oriented Design     //
// Author:      Ritesh Nair (rgnair@syr.edu)                            //
//////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides interface definition for
* - Browseable
* - Browser Filters
* - Browser Result Processors
*
* Required Files:
* ---------------
* RepoCoreDefinitions.h
*
* Maintenance History:
* --------------------
* ver 1.1 : 20 Apr 2018
* - modified signature of all interfaces
* ver 1.0 : 10 Mar 2018
* - first release
*/

#ifndef IREPO_BROWSER_H
#define IREPO_BROWSER_H

#include "../RepoCore/RepoCoreDefinitions.h"
#include "../../NoSqlDb/Query/Query.h"
#include "../ResourceProperties/IResourceProperties.h"

namespace SoftwareRepository
{
    /////////////////////////////////////////////////////////////////////
    // IBrowserFilter interface
    // - defines the contract for a Browser Filter
    // - a browser filter will be used to refine the output of the browser

    template <typename Payload>
    class IBrowserFilter
    {
    public:
        virtual NoSqlDb::Query<Payload> apply(const NoSqlDb::Query<Payload>&) = 0;
    };

    /////////////////////////////////////////////////////////////////////
    // IBrowserResultProcessor interface
    // - defines the contract for a Browser's Result Processor
    // - a browser result processor will be used to process the result of 
    //   the browser's navigation system

    template <typename Resource>
    class IBrowserResultProcessor
    {
    public:
        virtual bool operator()(Resource, ResourceVersion, Level) = 0;
    };

    /////////////////////////////////////////////////////////////////////
    // IRepoBrowser interface
    // - defines the contract for the browser semantics

    template <typename Resource, typename Payload>
    class IBrowseable
    {
    public:
        virtual bool exists(ResourceIdentity, ResourceVersion) = 0;
        virtual IResourceProperties<Payload>& get(ResourceIdentity, ResourceVersion) = 0;
        virtual void executeQuery(Resource, ResourceVersion,
            BrowseResultProcessors<IBrowserResultProcessor<Resource>>) = 0;
        virtual void executeQuery(BrowseFilters<IBrowserFilter<Payload>>,
            BrowseResultProcessors<IBrowserResultProcessor<Resource>>) = 0;
    };


}

#endif // !IREPO_BROWSER_H
