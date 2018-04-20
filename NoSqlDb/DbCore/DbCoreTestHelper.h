#pragma once
////////////////////////////////////////////////////////////////////////
// DbCoreTestHelper.h - Implements helper utilities which are used by //
//                  the testing framework                             //
// ver 1.2                                                            //
// Language:    C++, Visual Studio 2017                               //
// Application: NoSqlDb, CSE687 - Object Oriented Design              //
// Author:      Ritesh Nair (rgnair@syr.edu)                          //
////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the test utilities

* Required Files:
* ---------------
* DateTime.h, DateTime.cpp
* DbCore.h, DbCore.cpp
*
* Maintenance History:
* --------------------
* ver 1.2 : 16 Apr 2018
* - switched from std::string as payload to the IPayload implementation - StringPayload
* ver 1.1 : 15 Apr 2018
* - Moved into DbCore package
* - Move implementation into DbCore.cpp file
* ver 1.0 : 08 Feb 2018
* - first release
*/

#ifndef DBTEST_HELPER_H
#define DBTEST_HELPER_H

#include "../DateTime/DateTime.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../DbCore/DbCore.h"
#include "../Payloads/StringPayload.h"

#include <functional>

namespace NoSqlDbTests {

    ///////////////////////////////////////////////////////////////////////
    // test helper functions

    class DbTestHelper {
    public:
        //static void putLine(size_t n = 1, std::ostream& out = std::cout);

        template <typename T>
        static void showInitialDbState(NoSqlDb::DbCore<T>& db);

        static void createTitanDb(NoSqlDb::DbCore<NoSqlDb::StringPayload>& db,
            bool includeRelationships = true, bool includePosiedonAndLeto = false);

    private:
        static void _addChildrenInTitanDb(NoSqlDb::DbCore<NoSqlDb::StringPayload>& db);
        static void _addPosiedonAndLeto(NoSqlDb::DbCore<NoSqlDb::StringPayload>& db);
    };

    //----< prints the db state during initialization of a test >----------------------

    template <typename T>
    void DbTestHelper::showInitialDbState(NoSqlDb::DbCore<T>& db)
    {
        std::cout << "\n  showing all the database elements before beginning test: ";

        if (db.size() == 0)
        {
            std::cout << "\n    - DATABASE IS EMPTY -\n\n";
        }
        else
        {
            showDb(db);
            Utilities::putline(1, std::cout);

            std::cout << "\n  database keys are: ";
            showKeys(db);
            Utilities::putline(1, std::cout);
        }
    }

}

#endif // !DBTEST_HELPER_H