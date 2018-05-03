#pragma once
////////////////////////////////////////////////////////////////////////////////////
// BrowseResultProcessors.h - Implements result processors for the remote server  //
// ver 1.0                                                                        //
// Language:    C++, Visual Studio 2017                                           //
// Application: SoftwareRepository, CSE687 - Object Oriented Design               //
// Author:      Ritesh Nair (rgnair@syr.edu)                                      //
////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements all result processors for the remote software repository server.
* Result processors are fed to the repository browser. They collect search results and 
* process the results in desired form.
* It contains below classes
* - PackagesProcessor       : Returns list of unique packages from the browse result (no dependent information)
* - PackageFilesProcessor   : Returns list of files within a package (no dependent information)
* - FileMetadataProcessor   : Returns a file's metadata (author, description & dependencies)
*
* Required Files:
* ---------------
* RemoteRepositoryDefinitions.h
* IRepoBrowser.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 01 May 2018
* - first release
*/

#ifndef BROWSE_RESULT_PROCESSORS_H
#define BROWSE_RESULT_PROCESSORS_H

#include "RemoteCodeRepositoryDefinitions.h"
#include "../SoftwareRepository/RepoBrowser/IRepoBrowser.h"

namespace SoftwareRepository
{
    //----< computes the name is a format which the client understands >---------------------
    /*
    *  format: <package>#<namespace>#<filename>.<version>
    */

    std::string getFormattedName(FileResource res, ResourceVersion version)
    {
        return res.getPackageName() + "#" + res.getNamespace() +
            "#" + res.getResourceName() + "." + std::to_string(version);
    }

    /////////////////////////////////////////////////////////////////////
    // PackagesProcessor
    // - collects packages from the search result

    class PackagesProcessor : public IBrowserResultProcessor<FileResource>
    {
    public:
        std::vector<PackageName> getPackages() { return packages_; }
        virtual bool operator()(FileResource res, ResourceVersion version, Level level) override {
            std::vector<PackageName>::iterator found = std::find(packages_.begin(), packages_.end(), res.getPackageName());

            if (level == 0 && found == packages_.end())
                packages_.push_back(res.getPackageName());

            return true;
        };
    private:
        std::vector<PackageName> packages_;
    };

    /////////////////////////////////////////////////////////////////////
    // PackageFilesProcessor
    // - collects package files from the search result

    class PackageFilesProcessor : public IBrowserResultProcessor<FileResource>
    {
    public:
        std::vector<std::string> getPackageFiles() { return filenames_; }

        virtual bool operator()(FileResource res, ResourceVersion version, Level level) override {
            if (level == 0)
            {
                std::string formattedName = getFormattedName(res, version);
                std::vector<std::string>::iterator found = std::find(filenames_.begin(), filenames_.end(), formattedName);
                if (found == filenames_.end())
                    filenames_.push_back(formattedName);
            }

            return true;
        };
    private:
        std::vector<std::string> filenames_;
    };

    /////////////////////////////////////////////////////////////////////
    // FileMetadataProcessor
    // - collects file metadata from the search result

    class FileMetadataProcessor : public IBrowserResultProcessor<FileResource>
    {
    public:
        AuthorId getAuthor() { return author_; }
        ResourceDescription getDescription() { return description_; }
        std::vector<std::string> getDependencies() { return dependencies_; }

        virtual bool operator()(FileResource res, ResourceVersion version, Level level) override {
            std::string formattedName = getFormattedName(res, version);
            std::vector<std::string>::iterator found = std::find(dependencies_.begin(), dependencies_.end(), formattedName);

            if (level == 0)
            {
                author_ = res.getAuthor();
                description_ = res.getDescription();
            }
            else if (found == dependencies_.end())
            {
                dependencies_.push_back(formattedName);
            }

            return true;
        };
    private:
        AuthorId author_;
        ResourceDescription description_;
        std::vector<std::string> dependencies_;
    };

    /////////////////////////////////////////////////////////////////////
    // DemoTestResultProcessor
    // - collects package files from the search result

    class DemoTestResultProcessor : public IBrowserResultProcessor<FileResource>
    {
    public:
        std::vector<FileResource> getResources() { return resources_; }

        virtual bool operator()(FileResource res, ResourceVersion version, Level level) override {
            if (level == 0)
            {
                resources_.push_back(res);
            }

            return true;
        };
    private:
        std::vector<FileResource> resources_;
    };
}

#endif // !BROWSE_RESULT_PROCESSORS_H
