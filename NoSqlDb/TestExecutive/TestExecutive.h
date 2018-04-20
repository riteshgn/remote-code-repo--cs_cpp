#pragma once
//////////////////////////////////////////////////////////////////////////
// TestExecutive.h - Demonstrates the Software Repository system's      //
//                   core functionality                                 //
// ver 1.2                                                              //
// Language:    C++, Visual Studio 2017                                 //
// Application: SoftwareRepository, CSE687 - Object Oriented Design     //
// Author:      Ritesh Nair (rgnair@syr.edu)                            //
//////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* - This package prepates test suites from tests defined in the core modules.
* - It implements some non-functional tests suites
* - Accumlates all such suites and provides an executive which demonstrates
*   the core functionalities of the Repository
*
* Required Files:
* ---------------
* TestCore.h, TestCore.cpp
*
* Maintenance History:
* --------------------
* ver 1.2 : 15 Apr 2018
* - Refactored to make use of TestCore package
* ver 1.1 : 10 Feb 2018
* - added console colors for highlighting output
* - handling exceptions so that the executor can complete the full cycle
* ver 1.0 : 08 Feb 2018
* - first release
*/

#ifndef TEST_EXECUTIVE_H
#define TEST_EXECUTIVE_H

#include "../TestCore/TestCore.h"

namespace NoSqlDbTests
{
    /////////////////////////////////////////////////////////////////////
    // test functors
    // - Implements test cases to demonstrate the various requirements 
    //   on TestExecutive module

    class test10 : public TestCore::AbstractTest {
    public:
        test10(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };
    class test12 : public TestCore::AbstractTest {
    public:
        test12(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };
    class test13 : public TestCore::AbstractTest {
    public:
        test13(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };
}

#endif // !TEST_EXECUTIVE_H