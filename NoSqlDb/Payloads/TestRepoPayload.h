#pragma once
///////////////////////////////////////////////////////////////////////
// TestRepoPayload.h - Implements all test cases for RepoPayload     //
// ver 1.2                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: NoSqlDb, CSE687 - Object Oriented Design             //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This file implements the test functors for testing the RepoPayload

* Required Files:
* ---------------
* RepoPayload.h, RepoPayload.cpp
* DbCore.h, DbCore.cpp
* Query.h, Query.cpp
* Persistence.h, Persistence.cpp
* DateTime.h, DateTime.cpp
* DbTestHelper.h
*
* Maintenance History:
* --------------------
* ver 1.2 : 15 Apr 2018
* - Moved implementation into cpp file
* ver 1.1 : 10 Feb 2018
* - added test case for query by category
* ver 1.0 : 09 Feb 2018
* - first release
*/

#ifndef TEST_REPO_PAYLOAD_H
#define TEST_REPO_PAYLOAD_H

#include "../TestCore/TestCore.h"

namespace NoSqlDbTests
{
    /////////////////////////////////////////////////////////////////////
    // test9 functor
    // - Implements test case to demonstrate the Repository Payload requirement

    class test9 : public TestCore::AbstractTest
    {
    public:
        test9(AbstractTest::TestTitle title) : AbstractTest(title) {};
        virtual bool operator()();
    };
}

#endif // !TEST_REPO_PAYLOAD_H
