////////////////////////////////////////////////////////////////////////////////////
// RemoteCodeRepository.cpp - Implements the Remote Repository Server APIs        //
// ver 1.2                                                                        //
// Language:    C++, Visual Studio 2017                                           //
// Application: SoftwareRepository, CSE687 - Object Oriented Design               //
// Author:      Ritesh Nair (rgnair@syr.edu)                                      //
////////////////////////////////////////////////////////////////////////////////////

/*
* Maintenance History:
* --------------------
* ver 1.2 : 30 Apr 2018
* - backup and restore repository
* - explicity specify repository root directory (this is required for the file text handler)
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
#include "DemoRepository.h"

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

void RemoteRepoServer::processMessages(RepoCore &repo)
{
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
        FileSystemStore store(DEFAULT_FILE_STORE_ROOT_FOLDER);
        RepoCore repo(&versionMgr, &propsDb, &store);
        repo.loadRepo("../Demo");

        // this function will keep processing messages
        // until a 'quit' message is received
        processMessages(repo);

        repo.saveRepo("../Demo");
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



#ifdef TEST_REMOTE_CODE_REPO

int main()
{
    SetConsoleTitle(L"Remote Repository Server Console @ localhost:7790");

    // start the repository server
    RemoteRepoServer server;

    SingleDigitVersionMgr versionMgr;
    ResourcePropertiesDb propsDb(&versionMgr);
    FileSystemStore store(DEFAULT_FILE_STORE_ROOT_FOLDER);
    RepoCore testRepo(&versionMgr, &propsDb, &store);
    testRepo.loadRepo("../Demo/testing");

    demonstrateReq1();
    demonstrateReq2a(testRepo, propsDb);
    demonstrateReq2b(testRepo, propsDb);
    demonstrateReq2c(testRepo, propsDb);
    demonstrateReq3();
    demonstrateReq4();
    demonstrateReq5();
    demonstrateReq6();
    demonstrateReq7();

    std::cout << "\n\n " << std::string(28, '-') <<
        "\n  press enter to stop server" <<
        "\n " << std::string(28, '-') << "\n";
    std::cin.get();
    std::cout << "\n";

    return 0;
}

#endif // TEST_REMOTE_CODE_REPO
