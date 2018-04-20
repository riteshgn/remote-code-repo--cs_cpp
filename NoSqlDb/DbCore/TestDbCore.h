#pragma once
///////////////////////////////////////////////////////////////////////
// TestDbCore.h - Implements all test cases for DbCore               //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: NoSqlDb, CSE687 - Object Oriented Design             //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This file implements the test functors for testing the DbCore

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DbTestHelper.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 09 Feb 2018
* - first release
*/

#ifndef TEST_DBCORE_H
#define TEST_DBCORE_H

#include "../TestCore/TestCore.h"
#include "DbCoreTestHelper.h"

namespace NoSqlDbTests {

    // Helpers for time manipulation which will be used in testing queries
    using Duration = std::chrono::system_clock::duration;

    const Duration tenMins = DateTime::makeDuration(0, 10, 0, 0);
    const Duration oneDay = DateTime::makeDuration(24, 0, 0, 0);
    const Duration oneAndHalfDays = DateTime::makeDuration(36, 0, 0, 0);
    const Duration twoDays = DateTime::makeDuration(48, 0, 0, 0);
    const Duration threeDays = DateTime::makeDuration(72, 0, 0, 0);

    /////////////////////////////////////////////////////////////////////
    // test functors
    // - Implements test cases to demonstrate the various requirements 
    //   on DbCore module

    class test1 : public TestCore::AbstractTest {
    public:
        test1(TestCore::AbstractTest::TestTitle title) : TestCore::AbstractTest(title) {  }
        virtual bool operator()();
    };
    class test2 : public TestCore::AbstractTest {
    public:
        test2(TestCore::AbstractTest::TestTitle title) : TestCore::AbstractTest(title) {  }
        virtual bool operator()();
    };
    class test3a : public TestCore::AbstractTest {
    public:
        test3a(TestCore::AbstractTest::TestTitle title) : TestCore::AbstractTest(title) {  }
        virtual bool operator()();
    };
    class test3b : public TestCore::AbstractTest {
    public:
        test3b(TestCore::AbstractTest::TestTitle title) : TestCore::AbstractTest(title) {  }
        virtual bool operator()();
    };
    class test4a : public TestCore::AbstractTest {
    public:
        test4a(TestCore::AbstractTest::TestTitle title) : TestCore::AbstractTest(title) {  }
        virtual bool operator()();
    };
    class test4b : public TestCore::AbstractTest {
    public:
        test4b(TestCore::AbstractTest::TestTitle title) : TestCore::AbstractTest(title) {  }
        virtual bool operator()();
    };
    class test5a : public TestCore::AbstractTest {
    public:
        test5a(TestCore::AbstractTest::TestTitle title) : TestCore::AbstractTest(title) {  }
        virtual bool operator()();
    };
    class test5b : public TestCore::AbstractTest {
    public:
        test5b(TestCore::AbstractTest::TestTitle title) : TestCore::AbstractTest(title) {  }
        virtual bool operator()();
    };
    class test5c : public TestCore::AbstractTest {
    public:
        test5c(TestCore::AbstractTest::TestTitle title) : TestCore::AbstractTest(title) {  }
        virtual bool operator()();
    };
    class test5d : public TestCore::AbstractTest {
    public:
        test5d(TestCore::AbstractTest::TestTitle title) : TestCore::AbstractTest(title) {  }
        virtual bool operator()();
    };

}

#endif // !TEST_DBCORE_H
