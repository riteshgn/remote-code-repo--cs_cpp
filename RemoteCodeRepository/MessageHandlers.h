#pragma once
/////////////////////////////////////////////////////////////////////////////
// MessageHandlers.h - Implements the Remote Repository Server             //
// ver 1.1                                                                 //
// Language:    C++, Visual Studio 2017                                    //
// Application: SoftwareRepository, CSE687 - Object Oriented Design        //
// Author:      Ritesh Nair (rgnair@syr.edu)                               //
/////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements all message handlers for the remote software repository server.
* It contains below classes
* - Handlers which is a utility class that provides Message Handlers for the
*   different types of client requests supported by the server.
*
* Required Files:
* ---------------
* RemoteRepositoryDefinitions.h
* FileResource.h, FileResource.cpp
* BrowseResultProcessors.h
* BrowserFilters.h, RepoBrowser.cpp
* FileSystem.h, FileSystem.cpp
* FileSystemStore.h, FileSystemStore.cpp
*
* Maintenance History:
* --------------------
* ver 1.1 : 30 Apr 2018
* - message handlers use repocore for browsing packages, package files, file metadata
*   and file text
* ver 1.0 : 28 Apr 2018
* - first release
*/

#ifndef MESSAGE_HANDLERS_H
#define MESSAGE_HANDLERS_H

#include "RemoteCodeRepositoryDefinitions.h"
#include "BrowseResultProcessors.h"
#include "../SoftwareRepository/FileResource/FileResource.h"
#include "../SoftwareRepository/RepoBrowser/BrowserFilters.h"
#include "../SoftwareRepository/RepoStore/FileSystemStore.h"
#include "../SoftwareRepository/FileSystemDemo/FileSystem.h"

namespace SoftwareRepository
{

    /////////////////////////////////////////////////////////////////////
    // Handlers
    // utility class that provides Message Handlers

    class Handlers
    {
    public:

        //----< handles a ping request >---------------------
        /*
        *  Responds back with a "alive" attribute in the response
        *  to indicate that the server is healthy.
        */

        static HandlerFn ping()
        {
            return [](Message& message, RepoCore& repo) {
                Message reply;
                reply.to(message.from());
                reply.from(message.to());
                if (message.containsKey("requestId"))
                    reply.attribute("responseId", message.value("requestId"));
                reply.attribute("alive", "true");
                return reply;
            };
        }

        // ----< handles a check-in request >--------------------

        static HandlerFn checkIn()
        {
            return [](Message& message, RepoCore& repo) {
                FileResource res(message.value("namespace"), message.file());
                res
                    .setDescription(message.value("description"))
                    .setPackageName(message.value("package"))
                    .setSourceLocation("..\\SaveFiles\\" + message.file());

                if (message.containsKey("category"))
                    res.setCategory(message.value("category"));

                bool success = repo.checkIn(res, message.value("userId"));

                Message reply;
                reply.to(message.from());
                reply.from(message.to());
                if (message.containsKey("requestId"))
                    reply.attribute("responseId", message.value("requestId"));
                reply.attribute("success", std::to_string(1));
                return reply;
            };
        }

        // ----< handles a check-out request >--------------------

        static HandlerFn checkOut()
        {
            return [](Message& message, RepoCore& repo) {
                Message reply;
                reply.to(message.from());
                reply.from(message.to());
                if (message.containsKey("requestId"))
                    reply.attribute("responseId", message.value("requestId"));
                //reply.file("Logger.h");
                reply.attribute("success", "true");
                return reply;
            };
        }

        // ----< handles a request to fetch list of packages available in repository >--------------------

        static HandlerFn getRepoPackages()
        {
            return [](Message& message, RepoCore& repo) {

                CategoryFilter thisCategory = CategoryFilter::create(message.value("category"));
                PackagesProcessor packagesProcessor;
                repo.browse({ thisCategory }, { packagesProcessor });

                Message reply;
                reply.to(message.from());
                reply.from(message.to());
                if (message.containsKey("requestId"))
                    reply.attribute("responseId", message.value("requestId"));

                int count = 1;
                for (PackageName packageName : packagesProcessor.getPackages())
                {
                    reply.attribute("package-" + std::to_string(count), packageName);
                    count++;
                }

                return reply;
            };
        }

        // ----< handles a request to fetch list of files for a given package >--------------------

        static HandlerFn getPackageFiles()
        {
            return [](Message& message, RepoCore& repo) {

                PackageFilter thisPackage = PackageFilter::create(message.value("package"));
                PackageFilesProcessor packageFilesProcessor;
                repo.browse({ thisPackage }, { packageFilesProcessor });

                Message reply;
                reply.to(message.from());
                reply.from(message.to());
                if (message.containsKey("requestId"))
                    reply.attribute("responseId", message.value("requestId"));

                int count = 1;
                for (std::string filename : packageFilesProcessor.getPackageFiles())
                {
                    reply.attribute("file-" + std::to_string(count), filename);
                    count++;
                }

                return reply;
            };
        }

        // ----< handles request to send given file's metadata >--------------------

        static HandlerFn getFileMetadata()
        {
            return [](Message& message, RepoCore& repo) {
                FileResource res(message.value("namespace"), message.value("filename"));
                ResourceVersion version = std::stoi(message.value("version"));
                FileMetadataProcessor metadataProcessor;
                repo.browse(res, version, { metadataProcessor });

                Message reply;
                reply.to(message.from());
                reply.from(message.to());
                if (message.containsKey("requestId"))
                    reply.attribute("responseId", message.value("requestId"));

                std::string ns = message.value("namespace");
                std::string filename = message.value("filename");

                reply.attribute("author", metadataProcessor.getAuthor());
                reply.attribute("description", metadataProcessor.getDescription());
                int count = 1;
                for (std::string depFilename : metadataProcessor.getDependencies())
                {
                    reply.attribute("dependency-" + std::to_string(count), depFilename);
                    count++;
                }

                return reply;
            };
        }

        // ----< handles requests to send given file's content >--------------------

        static HandlerFn getFileText()
        {
            return [](Message& message, RepoCore& repo) {
                FileResource res(message.value("namespace"), message.value("filename"));
                ResourceVersion version = std::stoi(message.value("version"));
                std::string sourceFilename = FileSystemStore::getFilenameForSave(res.getIdentity(), version);
                std::string sourceFilePath = FileSystem::Path::getFullFileSpec(
                    DEFAULT_FILE_STORE_ROOT_FOLDER + '\\' + sourceFilename);
                std::string destFilename = res.getResourceName();
                std::string destPath = FileSystem::Path::getFullFileSpec("../SendFiles/" + destFilename);

                FileSystem::File::copy(sourceFilePath, destPath);

                Message reply;
                reply.to(message.from());
                reply.from(message.to());
                if (message.containsKey("requestId"))
                    reply.attribute("responseId", message.value("requestId"));

                reply.file(destFilename);

                return reply;
            };
        }
    };
}

#endif // !MESSAGE_HANDLERS_H