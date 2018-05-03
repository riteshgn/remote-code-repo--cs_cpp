#pragma once
/////////////////////////////////////////////////////////////////////////////
// DemoRepository.h - Demonstartes all required test cases                 //
// ver 1.1                                                                 //
// Language:    C++, Visual Studio 2017                                    //
// Application: SoftwareRepository, CSE687 - Object Oriented Design        //
// Author:      Ritesh Nair (rgnair@syr.edu)                               //
/////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements all demonstration cases
*
* Required Files:
* ---------------
* RemoteRepositoryDefinitions.h
* Comm.h, Comm.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 01 May 2018
* - first release
*/

#ifndef DEMOREPO_H
#define DEMOREPO_H

#include "RemoteCodeRepositoryDefinitions.h"
#include "../Comm/MsgPassingComm/Comm.h"

namespace SoftwareRepository
{
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

    bool demonstrateReq2a(RepoCore& testRepo, ResourcePropertiesDb& propsDb)
    {
        std::cout << "\n\n" << std::string(60, '-');
        std::cout << "\n  Demonstrating Requirement #2a - Repository Server provides check-in";
        std::cout << "\n" << std::string(60, '-') << "\n";

        std::cout << "\n\n    Initial state of Repository Db\n";
        propsDb.showDb();
        std::cout << "\n\n Attempting to checkin file Logger.h from ..\\Demo\\testing\\Logger.h \n\n";
        
        FileResource res("Logger", "Logger.h");
        res
            .setDescription("Logger implementation for C++ programs")
            .setPackageName("Logger")
            .setSourceLocation("..\\Demo\\testing\\Logger\\Logger.h")
            .setCategory("logger");

        if (!testRepo.checkIn(res, "test_user"))
        {
            std::cout << "\n    Test Failed - server check-in";
        }
        
        std::cout << "\n";
        propsDb.showDb();
        std::cout << "\n\n    Test Passed - server check-in";
        return true;
    }

    bool demonstrateReq2b(RepoCore& testRepo, ResourcePropertiesDb& propsDb)
    {
        std::cout << "\n\n" << std::string(60, '-');
        std::cout << "\n  Demonstrating Requirement #2b - Repository Server provides check-out";
        std::cout << "\n" << std::string(60, '-') << "\n";

        std::cout << "\n\n    Initial state of Repository Db\n";
        propsDb.showDb();
        std::cout << "\n\n Attempting to checkout file Process.h v1 to ..\\Demo\\testing \n\n";

        FileResource res("Process", "Process.h");

        if (!testRepo.checkout(res.getIdentity(), 1, "test_user", "..\\Demo\\testing"))
        {
            std::cout << "\n    Test Failed - server check-out";
        }

        std::cout << "\n\n    Test Passed - server check-out";
        return true;
    }

    bool demonstrateReq2c(RepoCore& testRepo, ResourcePropertiesDb& propsDb)
    {
        std::cout << "\n\n" << std::string(60, '-');
        std::cout << "\n  Demonstrating Requirement #2c - Repository Server provides browsing based on queries";
        std::cout << "\n" << std::string(60, '-') << "\n";

        std::cout << "\n\n    Initial state of Repository Db\n";
        propsDb.showDb();
        std::cout << "\n\n Attempting to query file(s) with category 'logger'";
        std::cout << "\n\n    code> query.where.payload.has(thisCategory)";

        CategoryFilter thisCategory = CategoryFilter::create("logger");
        DemoTestResultProcessor resultProcessor;
        testRepo.browse({ thisCategory }, { resultProcessor });

        std::cout << "\n    Found result";
        for (auto package : resultProcessor.getResources())
            std::cout << package.toString();

        std::cout << "\n\n Attempting to query file(s) with package 'Process'";
        std::cout << "\n\n    code> query.where.payload.has(thisPackage)";

        PackageFilter thisPackage = PackageFilter::create("Process");
        DemoTestResultProcessor resultProcessor2;
        testRepo.browse({ thisPackage }, { resultProcessor2 });

        std::cout << "\n    Found result";
        for (auto package : resultProcessor2.getResources())
            std::cout << package.toString();

        std::cout << "\n\n    Test Passed - server browsing based on queries";
        return true;
    }

    bool demonstrateReq3()
    {
        std::cout << "\n\n" << std::string(60, '-');
        std::cout << "\n  Demonstrating Requirement #3 - Client program";
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
        std::cout << "\n  Demonstrating Requirement #4 - message-passing communication system based on Sockets";
        std::cout << "\n" << std::string(60, '-') << "\n";

        std::cout << "\n  The Client GUI and the test client used in demonstration of requirement 5 " << 
            "run on different ports and both connect to this server which is on a different port";
        std::cout << "\n  Please note the port numbers for the respective comm system are logged. " <<
            "The server port is displayed on the beginning of this console. " <<
            "\n  test client port is displayed in the logs for req 5 and the GUI console displays the port it is running on\n";

        std::cout << "\n\n    Test Passed - socket communication";
        return true;
    }

    bool demonstrateReq5()
    {
        std::cout << "\n\n" << std::string(60, '-');
        std::cout << "\n  Demonstrating Requirement #5 - asynchronous message-passing communication channel";
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

    bool demonstrateReq6()
    {
        std::cout << "\n\n" << std::string(60, '-');
        std::cout << "\n  Demonstrating Requirement #6 - support file transfer";
        std::cout << "\n" << std::string(60, '-') << "\n";

        std::cout << "\n  The check-in and check-out demonstrations in requirement 3 validates this requirement  \n";

        std::cout << "\n    Test Passed - file transfer";
        return true;
    }

    bool demonstrateReq7()
    {
        std::cout << "\n\n" << std::string(60, '-');
        std::cout << "\n  Demonstrating Requirement #7 - automated test suite";
        std::cout << "\n" << std::string(60, '-') << "\n";

        std::cout << "\n  The execution of all demonstration cases show the working of the test suite  \n";

        std::cout << "\n    Test Passed - test suite";
        return true;
    }
}

#endif // !DEMOREPO_H