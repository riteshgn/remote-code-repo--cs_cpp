#pragma once
///////////////////////////////////////////////////////////////////////
// TestCore.h - Implements the Test Executive framework              //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: SoftwareRepository, CSE687 - Object Oriented Design  //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides the core framework to execute Tests. It contains below classes:
* - TestSuite which provides APIs to execute a collection of Test Functors. 
*   The Test Functors must implement the AbstractTest class.
* - AbstractTest provides the contract which all Test Functors must implement so that
*   they can be executed using the TestSuite.
* - TestExecutor which allows to execute a collection of Test Suites.
*
* Required Files:
* ---------------
* StringUtilities.h
* Console.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 23 Feb 2018
* - first release
*/

#ifndef TESTCORE_H
#define TESTCORE_H

#include "../Utilities/StringUtilities/StringUtilities.h"

#include <iostream>
#include <string>
#include <vector>

namespace TestCore {

    /////////////////////////////////////////////////////////////////////
    // AbstractTest functor
    // - defines the contract for all test functors
    // - implements helper apis for getting the test title and 
    //   setting/getting the message which indicates the status of a test.

    class AbstractTest
    {
    public:
        using TestMsg = std::string;
        using TestTitle = std::string;
        virtual bool operator()() = 0;
        TestMsg getTestTitle() { return title_; };
        TestMsg getMessage() { return msg_; };
        void setMessage(TestMsg msg) { msg_ = msg; };

    protected:
        AbstractTest(TestTitle title) : title_(title) {};

    private:
        TestTitle title_;
        TestMsg msg_ = "";
    };

    /////////////////////////////////////////////////////////////////////
    // TestSuite class
    // - stores a collection of test functors and executes them when invoked

    class TestSuite
    {
    public:
        using SuiteTitle = std::string;
        using TestFunctions = std::vector<std::reference_wrapper<AbstractTest>>;

        TestSuite(SuiteTitle title) : title_(title) {};

        SuiteTitle getTitle() { return title_; };
        virtual void registerEx(AbstractTest& test) { tests_.push_back(test); };
        void registerEx(TestFunctions tests);
        virtual bool executeAll() const;

    private:
        TestFunctions tests_;
        SuiteTitle title_;
    };

    /////////////////////////////////////////////////////////////////////
    // TestExecutor class
    // - stores a collection of test suites and executes them when invoked

    class TestExecutor
    {
    public:
        using TestSuites = std::vector<std::reference_wrapper<TestSuite>>;

        virtual void registerEx(TestSuite& suite) { suites_.push_back(suite); };
        void registerEx(TestSuites suites);
        virtual bool executeAll() const;

    private:
        TestSuites suites_;
    };

} // ! -- ns:Test

#endif // !TESTCORE_H