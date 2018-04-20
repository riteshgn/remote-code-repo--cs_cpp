#pragma once
/////////////////////////////////////////////////////////////////////////////////////////
// ResourcePropertiesDbTests.h - Implements all test cases for ResourcePropertiesDb    //
// ver 1.0                                                                             //
// Language:    C++, Visual Studio 2017                                                //
// Application: SoftwareRepository, CSE687 - Object Oriented Design                    //
// Author:      Ritesh Nair (rgnair@syr.edu)                                           //
/////////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This file implements the test functors for testing the ResourcePropertiesDb

* Required Files:
* ---------------
* TestCore.h, TestCore.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 10 Mar 2018
* - first release
*/

#ifndef RESOURCE_PROPS_DB_TESTS_H
#define RESOURCE_PROPS_DB_TESTS_H

#include "../TestCore/TestCore.h"

namespace SoftwareRepositoryTests
{
    /////////////////////////////////////////////////////////////////////
    // test functors
    // - Implements test cases to demonstrate the various requirements 
    //   on ResourcePropertiesDb module

    class TestResourceExistsAndCreateNewPropertiesEntry : public TestCore::AbstractTest {
    public:
        TestResourceExistsAndCreateNewPropertiesEntry(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };

    class TestFetchPropertiesForAFileResource : public TestCore::AbstractTest {
    public:
        TestFetchPropertiesForAFileResource(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };

    class TestModifyFileResourceProperties : public TestCore::AbstractTest {
    public:
        TestModifyFileResourceProperties(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };

    class TestExecuteQuery : public TestCore::AbstractTest {
    public:
        TestExecuteQuery(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };
}

#endif // !RESOURCE_PROPS_DB_TESTS_H
