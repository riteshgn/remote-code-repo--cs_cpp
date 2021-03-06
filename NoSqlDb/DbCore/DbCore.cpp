///////////////////////////////////////////////////////////////////////
// DbCore.cpp - Implements NoSql database prototype                  //
// ver 1.5                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: NoSqlDb, CSE687 - Object Oriented Design             //
// Author:      Ritesh Nair                                          //
///////////////////////////////////////////////////////////////////////

/*
* Maintenance History:
* --------------------
* ver 1.5 : 16 Apr 2018
* - switched from std::string as payload to the IPayload implementation - StringPayload
* ver 1.4 : 15 Apr 2018
* - Added DbElementMetadata API implementation
* - Added Test Helper API implementation
* ver 1.3 : 9 Feb 2018
* - pulled out all the test function into the TesDbCore.h file
* ver 1.2 : 4 Feb 2018
* - added tests for requirements 3 and 4
*   i.e. addition/removal of key/values and editing of values
* ver 1.1 : 2 Feb 2018
* - fixed tests to accomodate the changes to DbElement class & introduction of DbElementMetadata class
* ver 1.0 : 10 Jan 2018
* - first release
*/

#include "TestDbCore.h"
#include "DbCore.h"

using namespace NoSqlDb;
using namespace NoSqlDbTests;

/////////////////////////////////////////////////////////////////////
// DbElementMetadata methods

//----< adds a child key to the child relationships of an element in db >-----
/*
*  - Adds a child relationship
*  - This is an idempotent function
*    i.e if relationship with a child key has already been added, calling this
*    function again with the same child key will have no effect.
*  - TODO:
*    1. Check if child element actually exists in database??
*/
DbElementMetadata& DbElementMetadata::addRelationship(const Key& childKey)
{
    Children::iterator found = std::find(children_.begin(), children_.end(), childKey);
    if (found == children_.end())
        children_.push_back(childKey);

    return *this;
}

//----< removes a child key from the child relationships of an element in db >-----
/*
*  - Scans the child relationships of the specified db key and remove the
*    supplied child key from its relationships.
*  - Returns a true if the operation was successfull; false otherwise.
*/
DbElementMetadata& DbElementMetadata::removeRelationship(const Key& childKey)
{
    // using the erase-remove idiom to efficiently remove the key
    // here std::remove will 
    //     - re-order the vector by pushing the matching key(s) to the end of the vector
    //     - returning an iterator to the end of the non-matching list
    // and the erase function of the vector "children_" will remove the matched keys
    // source: https://stackoverflow.com/a/3385251
    // more info: https://en.wikipedia.org/wiki/Erase%E2%80%93remove_idiom

    children_.erase(std::remove(children_.begin(), children_.end(), childKey), children_.end());
    return *this;
}

// test functions

static DbCore<StringPayload> testDb;

//----< adds child relationships to the provided Titan DB instance >----------------------

void DbTestHelper::_addChildrenInTitanDb(NoSqlDb::DbCore<StringPayload>& db)
{
    db["kronos"].addRelationship("zeus");
    db["zeus"]
        .addRelationship("artemis")
        .addRelationship("apollo");
}

//----< adds two characters to the provided Titan DB instance >----------------------

void DbTestHelper::_addPosiedonAndLeto(NoSqlDb::DbCore<StringPayload>& db)
{
    using namespace NoSqlDb;

    DateTime dt = DateTime();
    DateTime now = dt.now();
    const DateTime tenMinsAgo = now - tenMins;
    const DateTime oneAndHalfDaysAgo = now - oneAndHalfDays;

    DbElementMetadata metadata;
    metadata.name("Leto");
    metadata.descrip("Goddess of Motherhood");
    metadata.dateTime(oneAndHalfDaysAgo);

    DbElement<StringPayload> demoElement;
    demoElement.metadata(metadata);
    demoElement.payLoad(StringPayload("Lives at Delos"));
    db.add("leto", demoElement);

    metadata.name("Poseidon");
    metadata.descrip("God of the Sea");
    metadata.dateTime(tenMinsAgo);

    demoElement.metadata(metadata);
    demoElement.payLoad(StringPayload("Lives in the Sea"));
    db.add("poseidon", demoElement);

    db["leto"]
        .addRelationship("artemis")
        .addRelationship("apollo");
}

//----< creates a test DB instance >----------------------

void DbTestHelper::createTitanDb(
    DbCore<StringPayload>& db, bool includeRelationships,
    bool includePosiedonAndLeto)
{
    db.truncate();

    DateTime dt = DateTime();
    DateTime now = dt.now();
    const DateTime tenMinsAgo = now - tenMins;
    const DateTime oneAndHalfDaysAgo = now - oneAndHalfDays;
    const DateTime twoDaysAgo = now - twoDays;

    DbElementMetadata metadata;
    metadata.name("Kronos");
    metadata.descrip("Youngest Titan");
    metadata.dateTime(twoDaysAgo);

    DbElement<StringPayload> demoElement;
    demoElement.metadata(metadata);
    demoElement.payLoad(StringPayload("Lives at Mount Othrys"));
    db.add("kronos", demoElement);

    metadata.name("Zeus");
    metadata.descrip("King of Gods");
    metadata.dateTime(oneAndHalfDaysAgo);

    demoElement.metadata(metadata);
    demoElement.payLoad(StringPayload("Lives at Mount Olympus"));
    db.add("zeus", demoElement);

    metadata.name("Apollo");
    metadata.descrip("God of Sun");
    metadata.dateTime(tenMinsAgo);

    demoElement.metadata(metadata);
    demoElement.payLoad(StringPayload("Also lives at Mount Olympus"));
    db.add("apollo", demoElement);

    metadata.name("Artemis");
    metadata.descrip("Goddess of the Hunt");
    metadata.dateTime(tenMinsAgo);

    demoElement.metadata(metadata);
    demoElement.payLoad(StringPayload("Also lives at Mount Olympus"));
    db.add("artemis", demoElement);

    if (includeRelationships)
        _addChildrenInTitanDb(db);

    if (includePosiedonAndLeto)
        _addPosiedonAndLeto(db);
}

//----< demo requirement #1 >------------------------------------------

bool test1::operator()()
{
    std::cout << "\n  " << typeid(std::function<bool()>).name()
        << ", declared in this function, "
        << "\n  is only valid for C++11 and later versions.";
    std::cout << "\n\n";

    setMessage("Use C++11");
    return true; // would not compile unless C++11
}

//----< demo requirement #2 >------------------------------------------

bool test2::operator()()
{
    std::cout << "\n  A visual examination of all the submitted code "
        << "will show only\n  use of streams and operators new and delete.";
    std::cout << "\n\n";

    setMessage("Use streams and new and delete");
    return true;
}

//----< demo first part of requirement #3 >----------------------------

bool test3a::operator()()
{
    using namespace NoSqlDb;

    std::cout << "\n  Creating a db element with key \"Fawcett\":";

    // create database to hold StringPayload payload
    DbCore<StringPayload>& db = testDb;

    // create some demo elements and insert into db
    DbElementMetadata demoElemMeta;
    demoElemMeta.name("Jim");
    demoElemMeta.descrip("Instructor for CSE687");
    demoElemMeta.dateTime(DateTime().now());
    DbElement<StringPayload> demoElem;
    demoElem.metadata(demoElemMeta);
    demoElem.payLoad(StringPayload("The good news is ..."));

    if (demoElem.metadata().name() != "Jim")
    {
        setMessage("Name was not set");
        return false;
    }
    if (demoElem.metadata().descrip() != "Instructor for CSE687")
    {
        setMessage("Description was not set");
        return false;
    }
    if (demoElem.metadata().dateTime().now() != DateTime().now())
    {
        setMessage("DateTime was not set");
        return false;
    }
    if (demoElem.payLoad().toString() != "The good news is ...")
    {
        setMessage("Payload was not set");
        return false;
    }

    showHeader();
    showElem(demoElem);

    db["Fawcett"] = demoElem;
    std::cout << "\n\n";

    setMessage("Create DbElement<StringPayload>");
    return true;
}
//----< demo second part of requirement #3 >---------------------------

void _addEntriesToDb(NoSqlDb::DbCore<StringPayload>& db)
{
    using namespace NoSqlDb;

    DbElementMetadata demoElemMeta;
    demoElemMeta.name("Ammar");
    demoElemMeta.descrip("TA for CSE687");

    DbElement<StringPayload> demoElem = db["Fawcett"];
    demoElem.metadata(demoElemMeta);
    demoElem.payLoad(StringPayload("You should try ..."));
    db["Salman"] = demoElem;

    demoElemMeta.name("Jianan");

    demoElem.metadata(demoElemMeta);
    demoElem.payLoad(StringPayload("Dr. Fawcett said ..."));
    db["Sun"] = demoElem;

    demoElem.payLoad(StringPayload("You didn't demonstrate Requirement #4"));
    demoElemMeta.name("Nikhil");
    demoElem.metadata(demoElemMeta);
    db["Prashar"] = demoElem;

    demoElem.payLoad(StringPayload("You didn't demonstrate Requirement #5"));
    demoElemMeta.name("Pranjul");
    demoElem.metadata(demoElemMeta);
    db["Arora"] = demoElem;

    demoElem.payLoad(StringPayload("You didn't demonstrate Requirement #6"));
    demoElemMeta.name("Akash");
    demoElem.metadata(demoElemMeta);
    db["Anjanappa"] = demoElem;
}

void _addChildren(NoSqlDb::DbCore<StringPayload>& db)
{
    using namespace NoSqlDb;

    std::cout << "\n  make all the new elements children of element with key \"Fawcett\"";
    db["Fawcett"].metadata().children().push_back("Salman");
    db["Fawcett"].metadata().children().push_back("Sun");
    db["Fawcett"].metadata().children().push_back("Prashar");
    db["Fawcett"].metadata().children().push_back("Arora");
    db["Fawcett"].metadata().children().push_back("Anjanappa");

    showHeader();
    showElem(db["Fawcett"]);

    db["Salman"].metadata().children().push_back("Sun");
    db["Salman"].metadata().children().push_back("Prashar");
    db["Salman"].metadata().children().push_back("Arora");
    db["Salman"].metadata().children().push_back("Anjanappa");
}

bool test3b::operator()()
{
    using namespace NoSqlDb;

    DbCore<StringPayload>& db = testDb;

    _addEntriesToDb(db);

    if (!db.contains("Salman"))
    {
        setMessage("Element not found");
        return false;
    }

    if (db.size() != 6)
    {
        setMessage("Not all elements were added");
        return false;
    }

    std::cout << "\n  after adding elements with keys: ";
    DbCore<StringPayload>::Keys keys = db.keys();
    showKeys(db);
    std::cout << "\n";

    _addChildren(db);

    // display the results

    std::cout << "\n\n  showing all the database elements:";
    showDb(db);
    std::cout << "\n";

    std::cout << "\n  database keys are: ";
    showKeys(db);

    // clean up and return
    db.truncate();
    std::cout << "\n\n";

    setMessage("Create DbCore<StringPayload>");
    return true;
}

//----< demo add element requirement #4a >-----------------------

bool test4a::operator()()
{
    using namespace NoSqlDb;

    DbCore<StringPayload>& db = testDb;

    DbTestHelper::showInitialDbState(db);

    std::unordered_map<std::string, DbElement<StringPayload>> records;

    DbElementMetadata metadata;
    metadata.name("Kronos");
    metadata.descrip("The youngest Titan");
    metadata.dateTime(DateTime().now());

    DbElement<StringPayload> element1;
    element1.metadata(metadata);
    element1.payLoad(StringPayload("The leader"));
    records["kronos"] = element1;

    metadata.name("Zeus");
    metadata.descrip("The King of Gods");
    metadata.dateTime(DateTime().now());

    DbElement<StringPayload> element2;
    element2.metadata(metadata);
    element2.payLoad(StringPayload("God of Thunder"));
    records["zeus"] = element2;

    db.add(records);

    if (db.size() != 2)
    {
        setMessage("Addition of key value pairs did not work as expected");
        return false;
    }

    std::cout << "\n  showing all the database elements after adding Zeus";
    showDb(db);

    std::cout << "\n\n  database keys are: ";
    showKeys(db);

    std::cout << "\n\n";
    setMessage("Adding key/value pairs to DB");
    return true;
}

//----< demo add element requirement #4b >-----------------------

bool test4b::operator()()
{
    using namespace NoSqlDb;

    DbCore<StringPayload>& db = testDb;

    DbTestHelper::showInitialDbState(db);

    // expect test data set from #4a to be available
    if (db.size() != 2)
        return false;

    db.remove("kronos");

    if (db.size() != 1)
        return false;

    std::cout << "\n  showing all the database elements after removing Kronos: ";
    showDb(db);
    std::cout << "\n";

    db.remove("kronos");
    std::cout << "\n  trying to remove Kronos again does not cause the DB to crash ";
    std::cout << "\n";

    db.remove("zeus");

    if (db.size() != 0)
        return false;

    std::cout << "\n  showing all the database elements after removing Zeus: ";
    showDb(db);
    std::cout << "\n";

    // clean up and return
    std::cout << "\n\n";

    setMessage("Removing elements from DB");
    return true;
}

//----< demo add relationship requirement #5a >-----------------------

bool test5a::operator()()
{
    using namespace NoSqlDb;

    DbCore<StringPayload>& db = testDb;
    DbTestHelper::createTitanDb(db, false);

    if (db.size() != 4)
    {
        setMessage("Expected 4 elements in DB");
        return false;
    }
    DbTestHelper::showInitialDbState(db);

    // demo add relationship using db element
    db["zeus"]
        .addRelationship("apollo")
        .addRelationship("artemis");

    DbElement<StringPayload> actualElem = db["zeus"];
    if (actualElem.metadata().children().size() != 2)
    {
        setMessage("Expected two children");
        return false;
    }

    std::cout << "\n  showing all the database elements after adding Apollo & Artemis as children of Zeus: ";
    showDb(db);
    std::cout << "\n";

    // demo add relationship using db core
    db.addRelationship("kronos", "zeus");
    db.addRelationship("kronos", "zeus");

    actualElem = db["kronos"];
    if (actualElem.metadata().children().size() != 1)
    {
        setMessage("Expected only one child");
        return false;
    }

    std::cout << "\n  showing all the database elements after adding Zeus as a child of Kronos twice: ";
    showDb(db);
    std::cout << "\n";

    // clean up and return
    std::cout << "\n\n";
    setMessage("Adding relationships");
    return true;
}

//----< demo remove relationship requirement #5b >-----------------------

bool test5b::operator()()
{
    using namespace NoSqlDb;

    DbCore<StringPayload>& db = testDb;

    if (db.size() != 4)
    {
        setMessage("Expected only 4 elements in DB");
        return false;
    }

    DbTestHelper::showInitialDbState(db);

    db["kronos"].removeRelationship("apollo");
    std::cout << "\n  trying to remove a non-existent relationship (kronos -> apollo) does not cause the DB to crash ";
    std::cout << "\n";

    // demo remove relationship using db core
    db.removeRelationship("zeus", "apollo");
    // demove remove relationship using db element
    db["kronos"].removeRelationship("zeus");

    DbElement<StringPayload> actualElem = db["zeus"];
    if (actualElem.metadata().children().size() != 1)
    {
        setMessage("Expected only one child");
        return false;
    }

    std::cout << "\n  showing all the database elements after removing Apollo's " <<
        "relationship with Zeus and Zeus's relationship with Kronos: ";
    showDb(db);
    std::cout << "\n\n";

    setMessage("Removing relationships");
    return true;
}

//----< demo editing metadata requirement #5c >-----------------------

bool test5c::operator()()
{
    using namespace NoSqlDb;

    DbCore<StringPayload>& db = testDb;

    if (db.size() != 4)
        return false;

    DbTestHelper::showInitialDbState(db);

    db["zeus"].metadata().name("God of Thunder");
    db["kronos"].metadata().descrip("The leader of Titans");

    if (db["zeus"].metadata().name() != "God of Thunder")
    {
        setMessage("Name not edited");
        return false;
    }

    if (db["kronos"].metadata().descrip() != "The leader of Titans")
    {
        setMessage("Description not edited");
        return false;
    }

    std::cout << "\n  showing all the database elements after updating Zeus's name and Kronos' description : ";
    showDb(db);
    std::cout << "\n\n";

    setMessage("Editing metadata");
    return true;
}

//----< demo replacing payload #5d >-----------------------

bool test5d::operator()()
{
    using namespace NoSqlDb;

    DbCore<StringPayload>& db = testDb;

    if (db.size() != 4)
    {
        setMessage("Expected 4 items in DB");
        return false;
    }

    DbTestHelper::showInitialDbState(db);

    // demo of replacing payload using db core
    db.replacePayLoad("apollo", StringPayload("Now lives in the Vatican Museum"));
    // demo of replacing payload using db element
    db["artemis"].payLoad(StringPayload("Now lives in The Louvre"));

    if (db["apollo"].payLoad().toString() != "Now lives in the Vatican Museum")
    {
        setMessage("payload for apollo not changed");
        return false;
    }

    if (db["artemis"].payLoad().toString() != "Now lives in The Louvre")
    {
        setMessage("payload for artemis not changed");
        return false;
    }

    std::cout << "\n  showing all the database elements after updating Apollo's and Artemis' payload : ";
    showDb(db);
    std::cout << "\n\n";

    setMessage("Replacing payload");
    return true;
}

using namespace TestCore;

//----< test stub >----------------------------------------------------

#ifdef TEST_DBCORE

int main()
{
    TestSuite dbCoreTestSuite("Testing DbCore - He said, she said database");
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
    dbCoreTestSuite.registerEx(test1);
    dbCoreTestSuite.registerEx(test2);
    dbCoreTestSuite.registerEx(test3a);
    dbCoreTestSuite.registerEx(test3b);
    dbCoreTestSuite.registerEx(test4a);
    dbCoreTestSuite.registerEx(test4b);
    dbCoreTestSuite.registerEx(test5a);
    dbCoreTestSuite.registerEx(test5b);
    dbCoreTestSuite.registerEx(test5c);
    dbCoreTestSuite.registerEx(test5d);

    dbCoreTestSuite.executeAll();
}
#endif
