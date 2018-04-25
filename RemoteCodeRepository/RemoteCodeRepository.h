#pragma once
/////////////////////////////////////////////////////////////////////////////
// RemoteCodeRepository.h - Implements the Remote Repository Server        //
// ver 1.0                                                                 //
// Language:    C++, Visual Studio 2017                                    //
// Application: SoftwareRepository, CSE687 - Object Oriented Design        //
// Author:      Ritesh Nair (rgnair@syr.edu)                               //
/////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements prototype for a remote software repository server.
* It contains below classes
* - RemoteRepoServer which encapsuates a Comm interface. It starts a socket server
and a socket listener which are used to accept requests from remote clients
and respond back to them.
* - Handlers which is a utility class that provides Message Handlers for the
*   different types of client requests supported by the server.
*
* Required Files:
* ---------------
* RemoteRepositoryDefinitions.h
* Comm.h, Comm.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 06 Apr 2018
* - first release
*/

#ifndef REMOTE_REPO_SERVER_H
#define REMOTE_REPO_SERVER_H

#include "RemoteCodeRepositoryDefinitions.h"
#include "../SoftwareRepository/RepoCore/RepoCoreDefinitions.h"
#include "../Comm/MsgPassingComm/Comm.h"

#include <thread>

namespace SoftwareRepository
{
    /////////////////////////////////////////////////////////////////////
    // RepoCore
    // Implements the remote repository server features.
    // When the startServer() funtion is called, it starts a socket server 
    // and a socket listener which are used to accept requests from remote 
    // clients and respond back to them.

    class RemoteRepoServer
    {
    public:
        RemoteRepoServer(Address bindAddress = ADDRESS_LOCALHOST,
            Port bindPort = DEFAULT_PORT_SERVER);
        ~RemoteRepoServer();

    private:
        MsgPassingCommunication::EndPoint clientEp_;
        std::string clientName_;
        MsgPassingCommunication::Comm comm_;
        MessageHandlers handlers_;
        std::thread messagesListenerThread_;

        std::string getClientName();
        void registerMessageHandlers();
        void startMessagesListenerThread();
        void startServer();
        void stopMessagesListenerThread();
        void stopServer();
    };

    /////////////////////////////////////////////////////////////////////
    // Handlers
    // utility class that provides Message Handlers

    class Handlers
    {
    public:

        //----< handles a ping request >---------------------
        /*
        *  Respons back with a "alive" attribute in the response
        *  to indicate that the server is healthy.
        */

        static HandlerFn ping()
        {
            return [](Message message) {
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
            return [](Message message) {
                Message reply;
                reply.to(message.from());
                reply.from(message.to());
                if (message.containsKey("requestId"))
                    reply.attribute("responseId", message.value("requestId"));
                reply.attribute("success", "true");
                return reply;
            };
        }

        // ----< handles a check-out request >--------------------

        static HandlerFn checkOut()
        {
            return [](Message message) {
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
            return [](Message message) {
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
            return [](Message message) {
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
            return [](Message message) {
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
            return [](Message message) {
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

#endif // !REMOTE_REPO_SERVER_H
