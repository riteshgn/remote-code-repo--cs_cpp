#pragma once
/////////////////////////////////////////////////////////////////////////////////////////
// ResourcePropertiesTests.h - Implements all test cases for ResourceProperties        //
// ver 1.1                                                                             //
// Language:    C++, Visual Studio 2017                                                //
// Application: SoftwareRepository, CSE687 - Object Oriented Design                    //
// Author:      Ritesh Nair (rgnair@syr.edu)                                           //
/////////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This file implements the test functors for testing the ResourceProperties

* Required Files:
* ---------------
* TestCore.h, TestCore.cpp
*
* Maintenance History:
* --------------------
* ver 1.1 : 23 Apr 2018
* - first release
*/

#ifndef RESOURCEPROPERTIES_TESTS_H
#define RESOURCEPROPERTIES_TESTS_H

#include "../TestCore/TestCore.h"

namespace SoftwareRepositoryTests
{
    /////////////////////////////////////////////////////////////////////
    // test functors
    // - Implements test cases to demonstrate the various requirements 
    //   on ResourceProperties module

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
}

#endif // !RESOURCEPROPERTIES_TESTS_H