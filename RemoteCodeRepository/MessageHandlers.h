#pragma once
/////////////////////////////////////////////////////////////////////////////
// MessageHandlers.h - Implements the Remote Repository Server             //
// ver 1.0                                                                 //
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
*
* Maintenance History:
* --------------------
* ver 1.0 : 28 Apr 2018
* - first release
*/

#ifndef MESSAGE_HANDLERS_H
#define MESSAGE_HANDLERS_H

#include "RemoteCodeRepositoryDefinitions.h"
#include "../SoftwareRepository/FileResource/FileResource.h"

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
                Message reply;
                reply.to(message.from());
                reply.from(message.to());
                if (message.containsKey("requestId"))
                    reply.attribute("responseId", message.value("requestId"));
                reply.attribute("package-1", "FileSystemDemo");
                reply.attribute("package-2", "RepoCore");
                return reply;
            };
        }

        // ----< handles a request to fetch list of files for a given package >--------------------

        static HandlerFn getPackageFiles()
        {
            return [](Message& message, RepoCore& repo) {
                Message reply;
                reply.to(message.from());
                reply.from(message.to());
                if (message.containsKey("requestId"))
                    reply.attribute("responseId", message.value("requestId"));

                if ("FileSystemDemo" == message.value("package"))
                {
                    reply.attribute("file-1", "FileSystemDemo#FileSystem#FileSystem.h.1");
                    reply.attribute("file-2", "FileSystemDemo#FileSystem#FileSystem.cpp.1");
                    reply.attribute("file-3", "FileSystemDemo#FileSystem#FileSystem.h.2");
                    reply.attribute("file-4", "FileSystemDemo#FileSystem#FileSystem.cpp.2");
                }

                if ("RepoCore" == message.value("package"))
                {
                    reply.attribute("file-1", "RepoCore#SoftwareRepository#RepoCore.h.1");
                    reply.attribute("file-2", "RepoCore#SoftwareRepository#RepoCore.cpp.1");
                }

                return reply;
            };
        }

        // ----< handles request to send given file's metadata >--------------------

        static HandlerFn getFileMetadata()
        {
            return [](Message& message, RepoCore& repo) {
                Message reply;
                reply.to(message.from());
                reply.from(message.to());
                if (message.containsKey("requestId"))
                    reply.attribute("responseId", message.value("requestId"));

                std::string ns = message.value("namespace");
                std::string filename = message.value("filename");

                if ("FileSystem.h" == filename
                    || "FileSystem.cpp" == filename)
                {
                    reply.attribute("description", "Utilities to interact with Filesystem");
                    reply.attribute("author", "jfawcett");
                    reply.attribute("dependency-1", "Utilities#Utilities#Utilities.h.1");
                    reply.attribute("dependency-2", "Utilities#Utilities#Utilities.cpp.1");
                }

                if ("RepoCore.h" == filename
                    || "RepoCore.cpp" == filename)
                {
                    reply.attribute("description", "Provides core repository semantics");
                    reply.attribute("author", "rgnair");
                    reply.attribute("dependency-1", "FileSystemDemo#FileSystem#FileSystem.h.2");
                    reply.attribute("dependency-2", "FileSystemDemo#FileSystem#FileSystem.cpp.2");
                }

                return reply;
            };
        }

        // ----< handles requests to send given file's content >--------------------

        static HandlerFn getFileText()
        {
            return [](Message& message, RepoCore& repo) {
                Message reply;
                reply.to(message.from());
                reply.from(message.to());
                if (message.containsKey("requestId"))
                    reply.attribute("responseId", message.value("requestId"));

                std::string ns = message.value("namespace");
                std::string filename = message.value("filename");
                std::string version = message.value("version");

                reply.file(filename);

                return reply;
            };
        }
    };
}

#endif // !MESSAGE_HANDLERS_H