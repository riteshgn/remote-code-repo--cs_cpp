#pragma once
///////////////////////////////////////////////////////////////////////
// FileResourceTests.h - Implements all test cases for FileResource  //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: NoSqlDb, CSE687 - Object Oriented Design             //
// Application: SoftwareRepository, CSE687 - Object Oriented Design  //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This file implements the test functors for testing the FileResource

* Required Files:
* ---------------
* TestCore.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 24 Feb 2018
* - first release
*/

#ifndef FILE_RESOURCE_TESTS_H
#define FILE_RESOURCE_TESTS_H

#include "../TestCore/TestCore.h"

namespace SoftwareRepositoryTests
{
    /////////////////////////////////////////////////////////////////////
    // test functors
    // - Implements test cases to demonstrate the various requirements 
    //   on FileResource module

    class TestCreateFileResource : public TestCore::AbstractTest {
    public:
        TestCreateFileResource(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };

    class TestAddCategoryAndDepToFileResource : public TestCore::AbstractTest {
    public:
        TestAddCategoryAndDepToFileResource(AbstractTest::TestTitle title) : AbstractTest(title) {  }
        virtual bool operator()();
    };
}

#endif // !FILE_RESOURCE_TESTS_H
