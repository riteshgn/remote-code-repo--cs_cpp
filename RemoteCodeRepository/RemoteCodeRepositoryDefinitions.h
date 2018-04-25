#pragma once

#ifndef REMOTEREPOSITORY_DEFINITIONS_H
#define REMOTEREPOSITORY_DEFINITIONS_H

#include "../Comm/Message/Message.h"

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
    using HandlerFn = std::function<Message(Message)>;
    using MessageHandlers = std::unordered_map<HandleKey, HandlerFn>;

    // CONSTANTS
    const std::string ADDRESS_LOCALHOST = "localhost";
    const size_t DEFAULT_PORT_SERVER = 7790;
    const size_t DEFAULT_PORT_CLIENT = 7890;
    const bool DEFAULT_VERBOSITY_INCOMING_MESSAGE = false;
}

#endif // !REMOTEREPOSITORY_DEFINITIONS_H
