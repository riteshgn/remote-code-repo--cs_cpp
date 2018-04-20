#pragma once
///////////////////////////////////////////////////////////////////////
// RepoCoreTests.h - Implements all test cases for RepoCore          //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: SoftwareRepository, CSE687 - Object Oriented Design  //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This file implements the test functors for testing the CheckOutMgr

* Required Files:
* ---------------
* TestCore.h, TestCore.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 10 Mar 2018
* - first release
*/

#ifndef REPOCORE_TESTS_H
#define REPOCORE_TESTS_H

#include "../TestCore/TestCore.h"

namespace SoftwareRepositoryTests
{
    /////////////////////////////////////////////////////////////////////
    // test functors
    // - Implements test cases to demonstrate the various requirements 
    //   on RepoCore module

    class TestCheckInMgr : public TestCore::AbstractTest {
    public:
        TestCheckInMgr(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };

    class TestCheckOutMgr : public TestCore::AbstractTest {
    public:
        TestCheckOutMgr(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };

    class TestCheckInMgrCommit : public TestCore::AbstractTest {
    public:
        TestCheckInMgrCommit(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };

    class TestRepoBrowserValidations : public TestCore::AbstractTest {
    public:
        TestRepoBrowserValidations(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };

    class TestRepoBrowser : public TestCore::AbstractTest {
    public:
        TestRepoBrowser(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };
}

#endif // !REPOCORE_TESTS_H
