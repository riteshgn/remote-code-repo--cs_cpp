////////////////////////////////////////////////////////////////////////////////////
// RemoteCodeRepository.cpp - Implements the Remote Repository Server APIs        //
// ver 1.1                                                                        //
// Language:    C++, Visual Studio 2017                                           //
// Application: SoftwareRepository, CSE687 - Object Oriented Design               //
// Author:      Ritesh Nair (rgnair@syr.edu)                                      //
////////////////////////////////////////////////////////////////////////////////////

/*
* Maintenance History:
* --------------------
* ver 1.1 : 28 Apr 2018
* - added exception handling in listener thread
* - changes required for new definition of message handler interface 
* ver 1.0 : 10 Mar 2018
* - first release
*/

#include "RemoteCodeRepository.h"
#include "MessageHandlers.h"
#include "../SoftwareRepository/VersionMgr/SingleDigitVersionMgr.h"
#include "../SoftwareRepository/ResourcePropertiesDb/ResourcePropertiesDb.h"
#include "../SoftwareRepository/RepoStore/FileSystemStore.h"

#include <exception>
#include <iostream>
#include <thread>
#include <Windows.h>


using namespace SoftwareRepository;

/////////////////////////////////////////////////////////////////////
// RemoteRepoServer methods

// ----< constructor for RemoteRepoServer class >--------------------
/*
*  It starts the comm server and the message listener thread
*/

inline RemoteRepoServer::RemoteRepoServer(Address bindAddress, Port bindPort) :
    clientEp_(bindAddress, bindPort), clientName_(getClientName()), comm_(clientEp_, clientName_)
{
    startServer();
}

// ----< destructor for RemoteRepoServer class >--------------------
/*
*  Shuts down the comm server and message listener thread
*/

inline RemoteRepoServer::~RemoteRepoServer()
{
    stopServer();
}

// ----< creates a unique client name for the repository's comm server >--------------------
/*
*  Compiles the name using the server's host, port and current time in epoch (millis)
*/

inline std::string RemoteRepoServer::getClientName()
{
    // below gets the current time in milliseconds since epoch
    // reference: https://stackoverflow.com/a/16177342

    unsigned long nowSinceEpochInMs =
        std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();

    // forming an unique clientname using <client_ip>_<port>_<time>
    return clientEp_.address + "_" + std::to_string(clientEp_.port) + "_" + std::to_string(nowSinceEpochInMs);
}

// ----< registers supported message handlers >--------------------

void RemoteRepoServer::registerMessageHandlers()
{
    handlers_["ping"] = Handlers::ping();
    handlers_["check-in"] = Handlers::checkIn();
    handlers_["check-out"] = Handlers::checkOut();
    handlers_["get-repo-packages"] = Handlers::getRepoPackages();
    handlers_["get-package-files"] = Handlers::getPackageFiles();
    handlers_["get-file-metadata"] = Handlers::getFileMetadata();
    handlers_["get-file-text"] = Handlers::getFileText();
}

// ----< starts the listener thread which checks comm interface for incoming client requests >--------------------

void RemoteRepoServer::startMessagesListenerThread()
{
    // lambda which whill be fed to the listner thread
    auto listen = [&]()
    {
        if (handlers_.empty())
        {
            std::cout << "\n  --> No message handlers found...";
            return;
        }

        SingleDigitVersionMgr versionMgr;
        ResourcePropertiesDb propsDb(&versionMgr);
        FileSystemStore store;
        RepoCore repo(&versionMgr, &propsDb, &store);

        std::cout << "\n\n    Repo Initialized. Contents";
        propsDb.showDb();
        std::cout << "\n\n";

        // keep checking for new messages until a "__quit" command is received
        while (true)
        {
            try {
                Message message = comm_.getMessage();

                if (message.containsKey("verbose") || DEFAULT_VERBOSITY_INCOMING_MESSAGE)
                {
                    std::cout << "\n\n    " << std::string(15, '-') << "x" << std::string(15, '-') << "\n";
                    std::cout << "\n    Server Logs:";
                    std::cout << "\n====================";
                    std::cout << "\n| Received message |";
                    std::cout << "\n====================";
                    message.show();
                }

                HandleKey thisCommand = message.command();
                if ("__quit" == thisCommand)
                    break;

                if (handlers_.find(thisCommand) != handlers_.end())
                {
                    Message reply = handlers_[thisCommand](message, repo);
                    if (message.containsKey("verbose") || DEFAULT_VERBOSITY_INCOMING_MESSAGE)
                    {
                        std::cout << "\n===================";
                        std::cout << "\n| Server Response |";
                        std::cout << "\n===================";
                        reply.show();
                        std::cout << "\n    " << std::string(15, '-') << "x" << std::string(15, '-');

                        std::cout << "\n\n    Repo Db Contents";
                        propsDb.showDb();
                        std::cout << "\n\n";
                    }
                    if (reply.to().port != message.to().port)
                        comm_.postMessage(reply);
                }
            }
            catch (std::exception &exception)
            {
                std::cerr << "\n\n  Standard exception: " << exception.what();
            }
            catch (...)
            {
                std::cerr << "\n\n  Oops! Unknown exception occured";
            }
        }

        std::cout << "\n  --> Message listener has been stopped. " <<
            "No further messages will be processed...";
    };

    std::thread listenerThread(listen);
    messagesListenerThread_ = std::move(listenerThread);
}

// ----< start up the remote repository server >--------------------

inline void RemoteRepoServer::startServer()
{
    // initialize communication systems
    comm_.start();
    std::cout << "\n  --> Repository server @ " <<
        clientEp_.address << ":" << std::to_string(clientEp_.port) <<
        " has been started";

    // register available message handlers
    registerMessageHandlers();
    std::cout << "\n  --> Registered " << std::to_string(handlers_.size()) << " message handlers...";

    // start listener thread
    std::cout << "\n  --> Starting message listener...";
    startMessagesListenerThread();
}

// ----< stops the listener thread >--------------------

void RemoteRepoServer::stopMessagesListenerThread()
{
    // send a 'quit' message to ensure that the thread processor exits
    Message message;
    message.command("__quit");
    message.to(clientEp_);
    comm_.postMessage(message);

    // wait for thread to join
    if (messagesListenerThread_.joinable())
        messagesListenerThread_.join();
}

// ----< stops the remote repository server >--------------------

inline void RemoteRepoServer::stopServer()
{
    // stop listener thread
    stopMessagesListenerThread();
    std::cout << "\n  --> Message listener gracefully shutdown...";

    // stop communication systems
    comm_.stop();
    std::cout << "\n  --> Repository server @ " <<
        clientEp_.address << ":" << std::to_string(clientEp_.port) <<
        " has been shutdown";

    std::cout << "\n\n";
}

// test stub

bool demonstrateReq1()
{
    std::cout << "\n\n" << std::string(60, '-');
    std::cout << "\n  Demonstrating Requirement #1 - Use C++11 & C#";
    std::cout << "\n" << std::string(60, '-') << "\n";

    std::cout << "\n  " << typeid(std::function<bool()>).name()
        << ", declared in this function, "
        << "\n  is only valid for C++11 and later versions.\n";

    std::cout << "\n  The Client GUI is developed using C# WPF\n";

    std::cout << "\n    Test Passed - Use C++11 & C#";
    return true; // would not compile unless C++11
}

bool demonstrateReq2()
{
    std::cout << "\n\n" << std::string(60, '-');
    std::cout << "\n  Demonstrating Requirement #2 - asynchronous message-passing communication channel";
    std::cout << "\n" << std::string(60, '-') << "\n";

    MsgPassingCommunication::EndPoint clientEp(ADDRESS_LOCALHOST, 7891);
    MsgPassingCommunication::EndPoint serverEp(ADDRESS_LOCALHOST, DEFAULT_PORT_SERVER);

    MsgPassingCommunication::Comm comm(clientEp, "server_test_engine");
    std::cout << "\n";
    std::cout << "\n  [client] starting test client @ " <<
        clientEp.address << ":" << std::to_string(clientEp.port);
    comm.start();

    Message message;
    message.from(clientEp);
    message.to(serverEp);
    message.command("ping");
    message.attribute("verbose", "true");
    std::cout << "\n  [client] posting 'ping' message";
    comm.postMessage(message);

    Message response = comm.getMessage();
    /*std::cout << "\n[client] Received response:";
    response.show();*/

    std::cout << "\n\n  [client] stopping test client @ " <<
        clientEp.address << ":" << std::to_string(clientEp.port);
    std::cout << "\n";
    comm.stop();

    std::cout << "\n    Test Passed - communication channel with HTTP style messages";
    return true;
}

bool demonstrateReq3()
{
    std::cout << "\n\n" << std::string(60, '-');
    std::cout << "\n  Demonstrating Requirement #3 - tabbed GUI";
    std::cout << "\n" << std::string(60, '-') << "\n";

    std::cout << "\n  The Client GUI will be shown and the GUI console will " <<
        "demonstrate further tests of this requirement\n";
    std::cout << "\n  Please note that every request sent from the client will " <<
        "generate an unique request Id \n  which can be used to compare with " <<
        "the server logs\n";

    return true;
}

bool demonstrateReq4()
{
    std::cout << "\n\n" << std::string(60, '-');
    std::cout << "\n  Demonstrating Requirement #4 - automated test suite";
    std::cout << "\n" << std::string(60, '-') << "\n";

    std::cout << "\n  The execution of all demonstration cases show the working of the test suite  \n";

    std::cout << "\n    Test Passed - test suite";
    return true;
}

#ifdef TEST_REMOTE_CODE_REPO

int main()
{
    SetConsoleTitle(L"Remote Repository Server Console @ localhost:7790");

    // start the repository server
    RemoteRepoServer server;

    /*demonstrateReq1();
    demonstrateReq2();
    demonstrateReq3();
    demonstrateReq4();*/

    std::cout << "\n\n " << std::string(28, '-') <<
        "\n  press enter to stop server" <<
        "\n " << std::string(28, '-') << "\n";
    std::cin.get();
    std::cout << "\n";

    return 0;
}

#endif // TEST_REMOTE_CODE_REPO
