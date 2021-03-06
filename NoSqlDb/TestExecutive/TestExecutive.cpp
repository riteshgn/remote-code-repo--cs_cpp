///////////////////////////////////////////////////////////////////////////
// TestExecutive.cpp - Executes all Software Repository Test Suites      //
// ver 1.1                                                               //
// Language:    C++, Visual Studio 2017                                  //
// Application: SoftwareRepository, CSE687 - Object Oriented Design      //
// Author:      Ritesh Nair (rgnair@syr.edu)                             //
///////////////////////////////////////////////////////////////////////////

/*
* Maintenance History:
* --------------------
* ver 1.1 : 15 Apr 2018
* - Refactored to make use of TestCore package
* ver 1.0 : 08 Feb 2018
* - first release
*/
#include "TestExecutive.h"
#include "../DbCore/TestDbCore.h"
#include "../Query/TestQuery.h"
#include "../Persistence/TestPersistence.h"
#include "../Payloads/TestRepoPayload.h"

using namespace TestCore;
using namespace NoSqlDbTests;

// test functions

//----< demo requirement #10 - Use of packages >------------------------------------------

bool test10::operator()()
{
    std::cout << "\n  A visual examination of all the submitted code will show the " <<
        "use of Executive, DbCore, Query & Test packages along with Persistence and DbPayloads packages. \n\n";

    setMessage("Use of packages");
    return true;
}

//----< demo requirement #12 - Test executive >------------------------------------------

bool test12::operator()()
{
    std::cout << "\n  The execution of these Test Suites proves the working of the Test executive.\n\n";

    setMessage("Test executive");
    return true;
}

//----< demo requirement #13 - Package diagram >------------------------------------------

bool test13::operator()()
{
    std::cout << "\n  The package diagram can be found in the root of the project folder " <<
        "[../package_diagram.pdf]\n\n";

    setMessage("Package diagram");
    return true;
}

//----< test stub >----------------------------------------------------

#ifdef TEST_TESTEXECUTIVE

int main()
{

    TestSuite dbCoreTestSuite("Testing DbCore - He said, she said database / The Titans database");
    test1 test1("Demonstrating Requirement #1");
    test2 test2("Demonstrating Requirement #2");
    test3a test3a("Demonstrating Requirement #3a - creating DbElement");
    test3b test3b("Demonstrating Requirement #3b - creating DbCore");
    test4a test4a("Demonstrating Requirement #4a - adding key/value pairs to DB");
    test4b test4b("Demonstrating Requirement #4b - removing key/value pairs from DB");
    test5a test5a("Demonstrating Requirement #5a - adding relationships");
    test5b test5b("Demonstrating Requirement #5b - removing relationships");
    test5c test5c("Demonstrating Requirement #5c - editing metadata");
    test5d test5d("Demonstrating Requirement #5d - replacing the payload");
    dbCoreTestSuite.registerEx({ test1, test2, test3a, test3b, test4a, test4b,
        test5a, test5b, test5c, test5d });

    TestSuite queryTestSuite("Testing Queries - The Titans database");
    test6 test6("Demonstrating Requirement #6 - simple querying");
    test7 test7("Demonstrating Requirement #7 - advanced querying");
    queryTestSuite.registerEx(test6);
    queryTestSuite.registerEx(test7);

    TestSuite persistenceTestSuite("Testing Persistence - The Titans database");
    test8a test8a("Demonstrating Requirement #8a - persist to XML File");
    test8b test8b("Demonstrating Requirement #8b - restoring from XML File");
    persistenceTestSuite.registerEx({ test8a, test8b });

    TestSuite repoPayloadTestSuite("Testing Custom Payload - The Repository database");
    test9 test9("Demonstrating Requirement #9 - custom payload");
    repoPayloadTestSuite.registerEx(test9);

    TestSuite packageStructureTestSuite("Testing Package Structure");
    test11a test11a("Demonstrating Requirement #11a - creating an XML with package structure");
    test11b test11b("Demonstrating Requirement #11b - loading the package structure XML to DB");
    packageStructureTestSuite.registerEx({ test11a, test11b });

    TestSuite nonFunctionalTestSuite("Other non-functional requirements");
    test10 test10("Demonstrating Requirement #10");
    test12 test12("Demonstrating Requirement #12");
    test13 test13("Demonstrating Requirement #13");
    nonFunctionalTestSuite.registerEx({ test10, test12, test13 });

    TestExecutor executor;
    executor.registerEx({ dbCoreTestSuite, persistenceTestSuite, queryTestSuite,
        repoPayloadTestSuite, packageStructureTestSuite, nonFunctionalTestSuite });
    executor.executeAll();

    return 0;
}

#endif // TEST_TESTEXECUTIVE

