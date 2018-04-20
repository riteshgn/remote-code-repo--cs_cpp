// RemoteCodeRepository.cpp : Defines the entry point for the console application.
//

#include "../NoSqlDb/DbCore/DbCore.h"
#include "../NoSqlDb/Payloads/StringPayload.h"
#include "../NoSqlDb/Query/Query.h"
#include "../NoSqlDb/Persistence/Persistence.h"

using namespace NoSqlDb;

void testNoSqlDb()
{
    DbCore<StringPayload> db;

    DbElementMetadata meta;
    meta.name("key1");
    meta.descrip("key1");
    DbElement<StringPayload> elem;
    elem.metadata(meta);
    elem.payLoad(StringPayload("key1-p"));

    db.add("key1", elem);

    meta.name("key2");
    meta.descrip("key2");
    meta.addRelationship("key1");
    elem.metadata(meta);
    elem.payLoad(StringPayload("key2-p"));

    db.add("key2", elem);

    showDb(db);
    std::cout << "\n\n";

    Query<StringPayload> query;
    DbCore<StringPayload> result = query.from(db).where.key.eqRegex("^key.*$").andWhere.child.eq("key1").end();

    showDb(result);
    std::cout << "\n\n";

    Persistence<StringPayload> persistence(result);
    persistence.exportDb(result.keys(), "test.xml");
    std::cout << "Saved to test.xml \n\n";
}

int main()
{
    testNoSqlDb();

    return 0;
}

