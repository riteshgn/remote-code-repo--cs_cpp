#pragma once
///////////////////////////////////////////////////////////////////////
// CheckInMgrTests.h - Implements all test cases for CheckInMgr      //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: SoftwareRepository, CSE687 - Object Oriented Design  //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This file implements the test functors for testing the CheckInMgr

* Required Files:
* ---------------
* TestCore.h, TestCore.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 10 Mar 2018
* - first release
*/

#ifndef CHECKIN_MGR_TESTS_H
#define CHECKIN_MGR_TESTS_H

#include "../TestCore/TestCore.h"

namespace SoftwareRepositoryTests
{
    /////////////////////////////////////////////////////////////////////
    // test functors
    // - Implements test cases to demonstrate the various requirements 
    //   on CheckInMgr module

    class TestCheckInValidations : public TestCore::AbstractTest {
    public:
        TestCheckInValidations(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };

    class TestCheckIn : public TestCore::AbstractTest {
    public:
        TestCheckIn(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };

    class TestCommitValidations : public TestCore::AbstractTest {
    public:
        TestCommitValidations(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };

    class TestCommit : public TestCore::AbstractTest {
    public:
        TestCommit(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };
}

#endif // !CHECKIN_MGR_TESTS_H
