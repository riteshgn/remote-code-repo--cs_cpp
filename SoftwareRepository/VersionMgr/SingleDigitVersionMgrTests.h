#pragma once
///////////////////////////////////////////////////////////////////////////////////////////
// SingleDigitVersionMgrTests.h - Implements all test cases for SingleDigitVersionMgr    //
// ver 1.0                                                                               //
// Language:    C++, Visual Studio 2017                                                  //
// Application: SoftwareRepository, CSE687 - Object Oriented Design                      //
// Author:      Ritesh Nair (rgnair@syr.edu)                                             //
///////////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This file implements the test functors for testing the SingleDigitVersionMgr

* Required Files:
* ---------------
* TestCore.h, TestCore.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 10 Mar 2018
* - first release
*/

#ifndef INTEGER_VERSION_MGR_TESTS_H
#define INTEGER_VERSION_MGR_TESTS_H

#include "../TestCore/TestCore.h"

namespace SoftwareRepositoryTests
{
    /////////////////////////////////////////////////////////////////////
    // test functors
    // - Implements test cases to demonstrate the various requirements 
    //   on SingleDigitVersionMgr module

    class TestAddingNewEntryAndFetchingCurrentVersion : public TestCore::AbstractTest {
    public:
        TestAddingNewEntryAndFetchingCurrentVersion(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };

    class TestFetchingNextVersion : public TestCore::AbstractTest {
    public:
        TestFetchingNextVersion(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };

    class TestCheckingValidVersion : public TestCore::AbstractTest {
    public:
        TestCheckingValidVersion(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };
}

#endif // !INTEGER_VERSION_MGR_TESTS_H
