#pragma once
///////////////////////////////////////////////////////////////////////
// CheckOutMgrTests.h - Implements all test cases for CheckOutMgr    //
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

#ifndef CHECKOUT_MGR_TESTS_H
#define CHECKOUT_MGR_TESTS_H

#include "../TestCore/TestCore.h"

namespace SoftwareRepositoryTests
{
    /////////////////////////////////////////////////////////////////////
    // test functors
    // - Implements test cases to demonstrate the various requirements 
    //   on CheckOutMgr module

    class TestCheckOutValidations : public TestCore::AbstractTest {
    public:
        TestCheckOutValidations(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };

    class TestCheckOut : public TestCore::AbstractTest {
    public:
        TestCheckOut(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };
}

#endif // !CHECKOUT_MGR_TESTS_H
