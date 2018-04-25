#pragma once
//////////////////////////////////////////////////////////////////////////
// BrowserFilters.h - Implements various filters for the RepoBrowser    //
// ver 1.0                                                              //
// Language:    C++, Visual Studio 2017                                 //
// Application: SoftwareRepository, CSE687 - Object Oriented Design     //
// Author:      Ritesh Nair (rgnair@syr.edu)                            //
//////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the various instances of IBrowserFilter interface to 
* provide query builders which are used to exeute queries against the resource
* properties db.
* It contains below classes:
* - FilenameFilter      : queries for the given filename
* - FileVersionFilter   : queries for given file version
* - PackageFilter       : queries for given package name
*
* Required Files:
* ---------------
* IRepoBrowser.h
* FileResourcePayload.h, FileResourcePayload.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 20 Apr 2018
* - first release
*/

#include "IRepoBrowser.h"
#include "../ResourceProperties/FileResourcePayload.h"

#ifndef BROWSERFILTERS_H
#define BROWSERFILTERS_H

namespace SoftwareRepository
{
    /////////////////////////////////////////////////////////////////////
    // FilenameFilter
    // - queries for the given filename

    class FilenameFilter : public IBrowserFilter<FileResourcePayload>
    {
    public:
        virtual NoSqlDb::Query<FileResourcePayload> apply(const NoSqlDb::Query<FileResourcePayload>&) override;
        static FilenameFilter create(const ResourceName& filename) { return FilenameFilter(filename); }

    private:
        FilenameFilter(const ResourceName& filename) : filename_(filename) {}
        ResourceName filename_;
    };

    /////////////////////////////////////////////////////////////////////
    // FileVersionFilter
    // - queries for given file version

    class FileVersionFilter : public IBrowserFilter<FileResourcePayload>
    {
    public:
        virtual NoSqlDb::Query<FileResourcePayload> apply(const NoSqlDb::Query<FileResourcePayload>&) override;
        static FileVersionFilter create(const ResourceVersion& version) { return FileVersionFilter(version); }

    private:
        FileVersionFilter(const ResourceVersion& version) : version_(version) {}
        ResourceVersion version_;
    };

    /////////////////////////////////////////////////////////////////////
    // PackageFilter
    // - queries for given package name

    class PackageFilter : public IBrowserFilter<FileResourcePayload>
    {
    public:
        virtual NoSqlDb::Query<FileResourcePayload> apply(const NoSqlDb::Query<FileResourcePayload>&) override;
        static PackageFilter create(const PackageName& package) { return PackageFilter(package); }

    private:
        PackageFilter(const PackageName& package) : package_(package) {}
        PackageName package_;
    };
}

#endif // !BROWSERFILTERS_H
