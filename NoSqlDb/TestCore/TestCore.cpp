///////////////////////////////////////////////////////////////////////
// TestCore.cpp - Implements the Test Core APIs                      //
//                & Demonstrates the usage                           //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: SoftwareRepository, CSE687 - Object Oriented Design  //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
///////////////////////////////////////////////////////////////////////

/*
* Maintenance History:
* --------------------
* ver 1.0 : 23 Feb 2018
* - first release
*/

#include "TestCore.h"
#include "../ConsoleColor/ConsoleColor.h"

using namespace TestCore;

/////////////////////////////////////////////////////////////////////
// TestExecutor methods

//----< convenience function for registering a list of test suites >---------------------

void TestExecutor::registerEx(TestSuites suites)
{
    for (TestSuite& suite : suites)
    {
        registerEx(suite);
    }
}

// Test Helper functions

//----< reduce the number of characters to type >----------------------

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
    Utilities::putline(n, out);
};

//----< executes all test suites and return true if all of them pass >---------------------

bool TestExecutor::executeAll() const
{
    bool executorResult = true;
    for (TestSuite& suite : suites_)
    {
        Console::setConsoleColor(Console::COLOR_PURPLE);
        Utilities::Title(suite.getTitle());
        Console::setConsoleColor(Console::COLOR_WHITE);
        putLine();

        if (!suite.executeAll())
            executorResult = false;
    }

    if (executorResult)
    {
        Console::setConsoleColor(Console::COLOR_GREEN);
        std::cout << "\n    All Test Suites have PASSED... \n\n";
    }
    else
    {
        Console::setConsoleColor(Console::COLOR_RED);
        std::cout << "\n  Atleast one of the Test Suites has FAILED... \n\n";
    }

    Console::setConsoleColor(Console::COLOR_WHITE);
    return executorResult;
}

/////////////////////////////////////////////////////////////////////
// TestSuite methods

//----< convenience function for registering a list of test functors >---------------------

void TestSuite::registerEx(TestFunctions tests)
{
    for (AbstractTest& test : tests)
    {
        registerEx(test);
    }
}

//----< executes all test functors and return true if all of them pass >---------------------

bool TestSuite::executeAll() const
{
    bool suiteResult = true;
    for (AbstractTest& test : tests_)
    {
        Console::setConsoleColor(Console::COLOR_GREY);
        Utilities::title(test.getTestTitle());
        Console::setConsoleColor(Console::COLOR_WHITE);

        bool result = false;
        try
        {
            result = test();
        }
        catch (std::exception &exception)
        {
            std::string expStr = exception.what();
            test.setMessage("Exception: " + expStr);
        }
        catch (...)
        {
            test.setMessage("Unexpected error occured...");
        }

        std::string resultStr = "PASSED";
        if (result)
        {
            Console::setConsoleColor(Console::COLOR_GREEN);
        }
        else
        {
            Console::setConsoleColor(Console::COLOR_RED);
            resultStr = "FAILED";
        }

        std::cout << "\n    Test " << resultStr << " - \"" << test.getMessage() << "\"\n\n";

        if (!result)
            suiteResult = false;

        Console::setConsoleColor(Console::COLOR_WHITE);
    }

    return suiteResult;
}

//----< test helpers >-------------------------------------------------

class Test1OnAmazingPackage1 : public AbstractTest
{
public:
    Test1OnAmazingPackage1(TestTitle title) : AbstractTest(title) {  }
    virtual bool operator()() override
    {
        setMessage("Everything looks great!");
        return true;
    }
};

class Test2OnAmazingPackage1 : public AbstractTest
{
public:
    Test2OnAmazingPackage1(TestTitle title) : AbstractTest(title) {  }
    virtual bool operator()() override
    {
        setMessage("Oops something went wrong!");
        return false;
    }
};

class Test1OnAmazingPackage2 : public AbstractTest
{
public:
    Test1OnAmazingPackage2(TestTitle title) : AbstractTest(title) {  }
    virtual bool operator()() override
    {
        setMessage("Everything looks great here too!");
        return true;
    }
};


//----< test stub >----------------------------------------------------

#ifdef TEST_TESTCORE

int main()
{
    Utilities::title("[1] Demonstrating execution of a Test Suite");
    std::cout << "\n>>  Add 2 tests for amazing package #1 to a Test Suite and execute the suite";
    std::cout << "\n>>  Note: One test passes and the other fails...\n\n";

    Test1OnAmazingPackage1 pkg1Test1("Testing a passing functionality in amazing package 1");
    Test2OnAmazingPackage1 pkg1Test2("Testing a failing functionality in amazing package 1");
    TestSuite testAmazingPackage1("Executing Test Suite for Amazing Package 1");
    testAmazingPackage1.registerEx({ pkg1Test1, pkg1Test2 });
    testAmazingPackage1.executeAll();

    std::cout << "\n>>  Successfully displays the execution status of the tests in the suite for package #1\n\n";

    Utilities::title("[2] Demonstrating execution of multiple Test Suites");
    std::cout << "\n>>  Create another suite for amazing package #2 and add a test to it (The test passes).";
    std::cout << "\n>>  Add test suites for package #1 & #2 to the test executor and execute it.\n\n";

    Test1OnAmazingPackage2 pkg2Test1("Testing a passing functionality in amazing package 2");
    TestSuite testAmazingPackage2("Executing Test Suite for Amazing Package 2");
    testAmazingPackage2.registerEx({ pkg2Test1 });

    TestExecutor executor;
    executor.registerEx({ testAmazingPackage1, testAmazingPackage2 });
    executor.executeAll();

    std::cout << "\n>>  Successfully displays the execution status of both the test suites.";
    std::cout << "\n>>  It also prints a message with the final status\n\n";

    return 0;
}

#endif // TESTCORE

