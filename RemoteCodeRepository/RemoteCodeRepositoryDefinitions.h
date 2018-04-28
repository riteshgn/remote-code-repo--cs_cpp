#pragma once
////////////////////////////////////////////////////////////////////////////////////
// RemoteCodeRepositoryDefinitions.h - Define aliases & constants used throughout //
//                                     the SoftwareRepository namespace           //
// ver 1.1                                                                        //
// Language:    C++, Visual Studio 2017                                           //
// Application: SoftwareRepository, CSE687 - Object Oriented Design               //
// Author:      Ritesh Nair (rgnair@syr.edu)                                      //
////////////////////////////////////////////////////////////////////////////////////
/*
* Maintenance History:
* --------------------
* ver 1.1 : 28 Apr 2018
* - changed definition of Handler function
* ver 1.0 : 06 Apr 2018
* - first release
*/

#ifndef REMOTEREPOSITORY_DEFINITIONS_H
#define REMOTEREPOSITORY_DEFINITIONS_H

#include "../Comm/Message/Message.h"
#include "../SoftwareRepository/RepoCore/RepoCore.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace SoftwareRepository
{
    // ALIASES
    using ClientName = std::string;

    using Address = MsgPassingCommunication::EndPoint::Address;
    using Port = MsgPassingCommunication::EndPoint::Port;
    using Message = MsgPassingCommunication::Message;

    using HandleKey = std::string;
    using HandlerFn = std::function<Message(Message&, RepoCore&)>;
    using MessageHandlers = std::unordered_map<HandleKey, HandlerFn>;

    // CONSTANTS
    const std::string ADDRESS_LOCALHOST = "localhost";
    const size_t DEFAULT_PORT_SERVER = 7790;
    const size_t DEFAULT_PORT_CLIENT = 7890;
    const bool DEFAULT_VERBOSITY_INCOMING_MESSAGE = false;
}

#endif // !REMOTEREPOSITORY_DEFINITIONS_H
