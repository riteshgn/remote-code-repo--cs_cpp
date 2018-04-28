#pragma once
/////////////////////////////////////////////////////////////////////////////
// RemoteCodeRepository.h - Implements the Remote Repository Server        //
// ver 1.1                                                                 //
// Language:    C++, Visual Studio 2017                                    //
// Application: SoftwareRepository, CSE687 - Object Oriented Design        //
// Author:      Ritesh Nair (rgnair@syr.edu)                               //
/////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements a remote software repository server.
* It contains below classes
* - RemoteRepoServer which encapsuates a Comm interface. It starts a socket server
and a socket listener which are used to accept requests from remote clients
and respond back to them.
*
* Required Files:
* ---------------
* RemoteRepositoryDefinitions.h
* RepoCoreDefinitions.h
* Comm.h, Comm.cpp
*
* Maintenance History:
* --------------------
* ver 1.1 : 28 Apr 2018
* - message handlers removed from this header file
* ver 1.0 : 06 Apr 2018
* - first release
*/

#ifndef REMOTE_REPO_SERVER_H
#define REMOTE_REPO_SERVER_H

#include "RemoteCodeRepositoryDefinitions.h"
#include "../SoftwareRepository/RepoCore/RepoCoreDefinitions.h"
#include "../Comm/MsgPassingComm/Comm.h"

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

}

#endif // !REMOTE_REPO_SERVER_H
