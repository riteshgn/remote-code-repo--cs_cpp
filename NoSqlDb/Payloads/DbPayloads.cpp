///////////////////////////////////////////////////////////////////////
// RepoPayload.cpp - Implements the persistence APIs                 //
// ver 1.3                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: NoSqlDb, CSE687 - Object Oriented Design             //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
///////////////////////////////////////////////////////////////////////

/*
* Maintenance History:
* --------------------
* ver 1.3 : 19 Apr 2018
* - payload query uses lambda for criteria definition instead of functor
* - modified test case to support this change
* ver 1.2 : 15 Apr 2018
* - Added implementations for the StringPayload and RepoPayload classes
* ver 1.1 : 15 Apr 2018
* - Added tests
* ver 1.0 : 09 Feb 2018
* - first release
*/

#include "TestRepoPayload.h"
#include "../DbCore/DbCoreTestHelper.h"
#include "RepoPayload.h"
#include "StringPayload.h"
#include "../Query/Query.h"

#include <iostream>

using namespace NoSqlDbTests;
using namespace NoSqlDb;
using namespace XmlProcessing;
using namespace Repository;

//----< converts payload to an xml element >---------------------

IPayload<StringPayload>::Sptr StringPayload::toXmlElement() 
{
    return makeTaggedElement("payload", toString());
}

//----< constructs payload from an xml element >---------------------

StringPayload StringPayload::fromXmlElement(IPayload<StringPayload>::Sptr pPayloadElem)
{
    if (pPayloadElem->children().size() > 0)
    {
        return StringPayload(pPayloadElem->children()[0]->value());
    }
    else
    {
        return StringPayload("");
    }
}

//----< converts payload to an xml element >---------------------

IPayload<RepoPayload>::Sptr RepoPayload::toXmlElement()
{
    IPayload<RepoPayload>::Sptr pPayload = makeTaggedElement("payload");

    pPayload->addChild(makeTaggedElement("filepath", filePath_));

    IPayload<RepoPayload>::Sptr pCategories = makeTaggedElement("categories");
    for (Category category : categories_)
    {
        pCategories->addChild(makeTaggedElement("category", category));
    }
    pPayload->addChild(pCategories);

    return pPayload;
}

//----< constructs payload from an xml element >---------------------

RepoPayload RepoPayload::fromXmlElement(IPayload<RepoPayload>::Sptr pPayloadElem)
{
    RepoPayload payload;

    for (auto pValueChild : pPayloadElem->children())
    {
        if (pValueChild->tag() == "filepath"
            && pValueChild->children().size() > 0)
        {
            payload.filePath(pValueChild->children()[0]->value());
        }
        else if (pValueChild->tag() == "categories")
        {
            for (auto pCategory : pValueChild->children())
            {
                payload.categories().push_back(pCategory->children()[0]->value());
            }
        }
    }

    return payload;
}

//----< serializes the payload for writing to an output stream >---------------------

std::ostream& Repository::operator<<(std::ostream& outputStream, const RepoPayload& payload)
{
    return outputStream << payload.toString();
}

//----< stringifies the payload >---------------------

std::string RepoPayload::toString() const
{
    return "FilePath: [" + filePath() + "], Categories: [" + stringifyCategories() + "]";
}

//----< stringifies the categories >---------------------

std::string RepoPayload::stringifyCategories() const
{
    return toCommaSeparatedString<Category>(categories_);
}

// test functions

std::function<bool(RepoPayload)> categoryHeader = [](RepoPayload payload) {
    using Category = std::string;
    for (Category category : payload.categories())
    {
        if (category == "Header")
            return true;
    }
    return false;
};

//----< generates data in db for performing the test >---------------------

void _populateTestData(NoSqlDb::DbCore<Repository::RepoPayload>& db)
{
    using namespace NoSqlDb;
    using namespace Repository;

    DbElementMetadata metadata;
    metadata.name("DateTime.h");
    metadata.descrip("The DateTime header file");

    RepoPayload payload;
    payload.filePath("/home/user/NoSqlDb/DateTime/");
    payload.categories({ "Header" });

    DbElement<RepoPayload> element;
    element.metadata(metadata);
    element.payLoad(payload);
    db["DateTime.h"] = element;

    metadata.name("DateTime.cpp");
    metadata.descrip("The DateTime application file");

    payload.filePath("/home/user/NoSqlDb/DateTime/");
    payload.categories({ "StaticLibrary" });

    element.metadata(metadata);
    element.payLoad(payload);
    db["DateTime.cpp"] = element;

    metadata.name("DbCore.h");
    metadata.descrip("The DbCore header file");

    payload.filePath("/home/user/NoSqlDb/DbCore/");
    payload.categories({ "Header" });

    element.metadata(metadata);
    element.payLoad(payload);
    db["DbCore.h"] = element;

    metadata.name("DbCore.cpp");
    metadata.descrip("The DbCore application file");

    payload.filePath("/home/user/NoSqlDb/DbCore/");
    payload.categories({ "Application" });

    element.metadata(metadata);
    element.payLoad(payload);
    db["DbCore.cpp"] = element;
}

//----< demo custom payload requirement #9 >---------------------
bool test9::operator()()
{
    DbCore<RepoPayload> db;
    DbTestHelper::showInitialDbState(db);

    _populateTestData(db);
    if (db.size() != 4)
    {
        setMessage("Expected db to have 4 elements");
        return false;
    }

    std::cout << "\n  after adding elements with keys: ";
    DbCore<RepoPayload>::Keys keys = db.keys();
    showKeys(db);
    std::cout << "\n";

    std::cout << "\n  make all header files children of respective cpp files";
    db["DateTime.cpp"].addRelationship("DateTime.h");
    db["DbCore.cpp"]
        .addRelationship("Date.h")
        .addRelationship("DbCore.h");

    std::cout << "\n\n  showing all the database elements:";
    showDb(db);
    std::cout << "\n";

    Query<RepoPayload> query;
    DbCore<RepoPayload> result = query.from(db).where.payload.has(categoryHeader).end();

    std::cout << "\n\n  Demonstrating custom payload query: ";
    std::cout << "\n  code> query.from(db).where.payload.has(categoryHeader).end()\n";
    showDb(result);
    std::cout << "\n";

    /*Persistence<RepoPayload> persistence(db);
    persistence.exportDb(db.keys(), "../db_shards/test_repopayload.xml");*/

    // clean up and return
    std::cout << "\n\n";

    setMessage("Adding custom payload to NoSQlDb");
    return true;
}


//----< test stub >----------------------------------------------------

using namespace TestCore;

#ifdef TEST_DBPAYLOADS

int main()
{
    TestSuite repoPayloadTestSuite("Testing Queries - The Titans database");
    test9 test9("Demonstrating Requirement #9 - custom payload");
    repoPayloadTestSuite.registerEx(test9);

    repoPayloadTestSuite.executeAll();

    return 0;
}

#endif // TEST_DBPAYLOADS

