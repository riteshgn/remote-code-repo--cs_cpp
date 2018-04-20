#pragma once
///////////////////////////////////////////////////////////////////////
// Query.h - Implements the Query module for the NoSql database      //
// ver 1.4                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: NoSqlDb, CSE687 - Object Oriented Design             //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
////////////
///////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements below class for querying the NoSqlDb:
* - Query class provides APIs to load a DBCore object or a partial result set
*   and query on the keys, its metadata, children and date-time interval.

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
*
* Maintenance History:
* --------------------
* ver 1.4 : 19 Apr 2018
* - payload query uses lambda for criteria definition instead of functor
* - Remove payload criteria interface
* - Added query type for metadata to search by name - both exact match and using regex
* ver 1.3 : 10 Feb 2018
* - added searching over payloads
* - made querytype references constant
* ver 1.2 : 07 Feb 2018
* - introduced QueryTypes class
* ver 1.1 : 06 Feb 2018
* - made the query language concrete
* - added support for querying metadata and children
* - added support for 'OR' queries
* ver 1.0 : 04 Feb 2018
* - first release
*/

#ifndef QUERY_H
#define QUERY_H

#include <functional>
#include <regex>
#include <string>
#include <vector>
#include "../DbCore/DbCore.h"

namespace NoSqlDb {

    template<typename T>
    class Query;

    /////////////////////////////////////////////////////////////////////
    // KeyQuery class
    // - Provides APIs to query on a key in the NoSQlDb

    template<typename T>
    class KeyQuery 
    {
    public:
        using Key = std::string;
        using Keys = std::vector<Key>;
        using Regex = std::string;

        KeyQuery(Query<T>& cursor) : cursor_(cursor) {}

        Query<T> eq(const Key& key) const;
        Query<T> eqRegex(const Regex& regexStr) const;

    private:
        Query<T>& cursor_;
    };

    /////////////////////////////////////////////////////////////////////
    // DateQuery class
    // - Provides APIs to query on DateTime ranges in the NoSQlDb

    template<typename T>
    class DateQuery
    {
    public:
        using Key = std::string;
        using Keys = std::vector<Key>;

        DateQuery(Query<T>& cursor) : cursor_(cursor) {}

        Query<T> between(DateTime from, DateTime to) const;
        Query<T> gt(DateTime value) const;
        Query<T> lt(DateTime value) const;

    private:
        Query<T>& cursor_;
    };

    /////////////////////////////////////////////////////////////////////
    // MetadataQuery class
    // - Provides APIs to query on Metadata fields in the NoSQlDb

    template<typename T>
    class MetadataQuery
    {
    public:
        using Key = std::string;
        using Keys = std::vector<Key>;
        using Regex = std::string;

        MetadataQuery(Query<T>& cursor) : cursor_(cursor) {}

        Query<T> eqRegex(const Regex& regexStr) const;
        Query<T> eqNameRegex(const Regex& regexStr) const;
        Query<T> eqName(const Key& name) const;

    private:
        Query<T>& cursor_;
    };

    /////////////////////////////////////////////////////////////////////
    // ChildrenQuery class
    // - Provides APIs to query the children of a record in the NoSQlDb

    template<typename T>
    class ChildrenQuery
    {
    public:
        using Key = std::string;
        using Keys = std::vector<Key>;

        ChildrenQuery(Query<T>& cursor) : cursor_(cursor) {}

        Query<T> eq(const Key& key) const;

    private:
        Query<T>& cursor_;
    };

    /////////////////////////////////////////////////////////////////////
    // PayloadQuery class
    // - Provides APIs to query the payload of a record in the NoSQlDb

    template <typename T>
    class PayloadQuery
    {
    public:
        using Key = std::string;
        using Keys = std::vector<Key>;
        using Criteria = std::function<bool(T)>;

        PayloadQuery(Query<T>& cursor) : cursor_(cursor) {}

        Query<T> has(const Criteria& check) const;

    private:
        Query<T>& cursor_;
    };

    /////////////////////////////////////////////////////////////////////
    // QueryTypes class
    // - Provides access to the various queryable fields in the NoSqlDb

    template <typename T>
    class QueryTypes
    {
    public:
        const KeyQuery<T>& key = key_;
        const DateQuery<T>& dateTime = dateTime_;
        const MetadataQuery<T>& metadata = metadata_;
        const ChildrenQuery<T>& child = child_;
        const PayloadQuery<T>& payload = payload_;

        QueryTypes(Query<T>& cursor) :
            key_(cursor), dateTime_(cursor),
            metadata_(cursor), child_(cursor),
            payload_(cursor) {}

    private:
        KeyQuery<T> key_;
        DateQuery<T> dateTime_;
        MetadataQuery<T> metadata_;
        ChildrenQuery<T> child_;
        PayloadQuery<T> payload_;
    };

    /////////////////////////////////////////////////////////////////////
    // Query class
    // - Provides APIs to query the NoSqlDb
    // - Supported queries are on keys, metadata, datetime and child
    //   
    // - Example queries:
    //   DbCore<MyType> db = fetchMyDb();
    //   Query<MyType> query;
    //   DbCore<MyType> result = query.from(db).where.key.eq(DbKey).end();
    //      -- Make sure that end() is called to return your result set
    //      -- More complex queries can be formed like so
    //   result = query.from(db)
    //              .where.key.eqRegex(SomeRegex)
    //              .andWhere.dateTime.between(TwoDaysAgo, OneDayAgo)
    //              .end();
    //      -- Simply chain the queries and call end() when you need the results
    //      -- You can also "OR" queries like so
    //   result = query.or({
    //                  query.from(db).where.key.eqRegex(SomeRegex),
    //                  query.from(db).where.dateTime.between(TwoDaysAgo, OneDayAgo),
    //                  ...
    //              })
    //              .end();
    //      -- You can chain any number of queries like above and call end() 
    //         to get the final results.
    //      -- You can query on the previous result like so
    //    DbCore<MyType> filteredResult = query.from(result).where.child.eq(ChildKey).end();
    //
    //  Enjoy querying!

    template <typename T>
    class Query {
    public:
        using Key = std::string;
        using Keys = std::vector<Key>;
        using ResultSet = DbCore<T>;
        using ResultSets = std::vector<ResultSet>;

        const QueryTypes<T>& where = where_;
        const QueryTypes<T>& andWhere = where_;

        Query() : where_(*this) {}

        Query<T> from(DbCore<T>& db) { db_ = db; return *this; };
        Query<T> orThese(ResultSets queries);
        DbCore<T> end();

    private:
        DbCore<T> db_;
        QueryTypes<T> where_;

        Keys& keys() { return keys_; }
        void db(const DbCore<T>& db) { db_ = db; };
        void save(const Keys& keys);
        void saveOne(const Key& key) { save({ key }); };

        friend class KeyQuery<T>;
        friend class DateQuery<T>;
        friend class MetadataQuery<T>;
        friend class ChildrenQuery<T>;
        friend class PayloadQuery<T>;
    };
    
    /////////////////////////////////////////////////////////////////////
    // KeyQuery<T> methods

    //----< searches for a key by value and returns a cursor >---------------------

    template <typename T>
    Query<T> KeyQuery<T>::eq(const Key& key) const
    {
        if (cursor_.db_.contains(key)) {
            cursor_.saveOne(key);
        }
        return cursor_;
    }

    //----< searches for a key by regex and returns a cursor >---------------------

    template <typename T>
    Query<T> KeyQuery<T>::eqRegex(const Regex& regexStr) const
    {
        Keys matchedKeys;
        for (Key key : cursor_.db_.keys())
        {
            std::regex regex(regexStr);
            if (std::regex_match(key, regex))
                matchedKeys.push_back(key);
        }

        cursor_.save(matchedKeys);
        return cursor_;
    }

    /////////////////////////////////////////////////////////////////////
    // DateQuery<T> methods

    //----< searches by date range and returns a cursor >---------------------

    template <typename T>
    Query<T> DateQuery<T>::between(DateTime from, DateTime to) const
    {
        Keys matchedKeys;
        DbCore<T>& db = cursor_.db_;
        for (Key key : db.keys())
        {
            DateTime dt = db[key].metadata().dateTime();
            if (dt > from && dt < to)
                matchedKeys.push_back(key);
        }

        cursor_.save(matchedKeys);
        return cursor_;
    }

    //----< searches by date greater than and returns a cursor >-----------------

    template <typename T>
    Query<T> DateQuery<T>::gt(DateTime value) const
    {
        return between(value, DateTime().now());
    }

    //----< searches by date less than and returns a cursor >-----------------

    template <typename T>
    Query<T> DateQuery<T>::lt(DateTime value) const
    {
        Keys matchedKeys;
        DbCore<T>& db = cursor_.query().db();
        for (Key key : db.keys())
        {
            DateTime dt = db[key].metadata().dateTime();
            if (dt < value)
                matchedKeys.push_back(key);
        }

        cursor_.save(matchedKeys);
        return cursor_;
    }

    /////////////////////////////////////////////////////////////////////
    // MetadataQuery<T> methods

    //----< searches metadata by regex and returns a cursor >-----------------

    template <typename T>
    Query<T> MetadataQuery<T>::eqRegex(const Regex& regexStr) const
    {
        Keys matchedKeys;
        DbCore<T>& db = cursor_.db_;
        for (Key key : db.keys())
        {
            std::regex regex(regexStr);
            std::string name = db[key].metadata().name();
            std::string description = db[key].metadata().descrip();
            if (std::regex_match(name, regex)
                || std::regex_match(description, regex))
                matchedKeys.push_back(key);
        }

        cursor_.save(matchedKeys);
        return cursor_;
    }

    //----< searches name in metadata by regex and returns a cursor >-----------------

    template <typename T>
    Query<T> MetadataQuery<T>::eqNameRegex(const Regex& regexStr) const
    {
        Keys matchedKeys;
        DbCore<T>& db = cursor_.db_;
        for (Key key : db.keys())
        {
            std::regex regex(regexStr);
            std::string name = db[key].metadata().name();
            if (std::regex_match(name, regex))
                matchedKeys.push_back(key);
        }

        cursor_.save(matchedKeys);
        return cursor_;
    }

    //----< searches name metadata and returns a cursor >-----------------

    template <typename T>
    Query<T> MetadataQuery<T>::eqName(const Key& name) const
    {
        Keys matchedKeys;
        DbCore<T>& db = cursor_.db_;
        for (Key key : db.keys())
        {
            if (db[key].metadata().name() == name)
                matchedKeys.push_back(key);
        }

        cursor_.save(matchedKeys);
        return cursor_;
    }

    /////////////////////////////////////////////////////////////////////
    // ChildrenQuery<T> methods

    //----< searches by metadata using regex and returns a cursor >-----------------

    template <typename T>
    Query<T> ChildrenQuery<T>::eq(const Key& key) const
    {
        Keys matchedKeys;
        DbCore<T>& db = cursor_.db_;
        for (Key dbKey : db.keys())
        {
            Keys children = db[dbKey].metadata().children();
            for (Key childKey : children)
            {
                if (childKey == key)
                {
                    matchedKeys.push_back(dbKey);
                    break;
                }
            }
        }

        cursor_.save(matchedKeys);
        return cursor_;
    }

    /////////////////////////////////////////////////////////////////////
    // PayloadQuery<T> methods

    //----< searches by custom functor provided by user >-----------------

    template <typename T>
    Query<T> PayloadQuery<T>::has(const Criteria& check) const
    {
        Keys matchedKeys;
        for (Key key : cursor_.db_.keys())
        {
            if (check(cursor_.db_[key].payLoad()))
                matchedKeys.push_back(key);
        }

        cursor_.save(matchedKeys);
        return cursor_;
    }

    /////////////////////////////////////////////////////////////////////
    // Query<T> methods

    //----< performs a union on the list of result sets and returns a cursor >----------

    template <typename T>
    Query<T> Query<T>::orThese(ResultSets resultSets)
    {
        for (ResultSet db : resultSets)
        {
            for (Key key : db.keys())
            {
                db_[key] = db[key];
            }
        }

        return *this;
    }

    //----< saves the result set based on the filtered keys >----------

    template <typename T>
    void Query<T>::save(const Keys& keys)
    {
        DbCore<T> resultDb;
        for (Key key : keys)
        {
            resultDb[key] = db_[key];
        }
        db_ = resultDb;
    }

    //----< ends the query and returns the result >----------

    template <typename T>
    DbCore<T> Query<T>::end() {
        DbCore<T> result = db_;
        db_.truncate();
        return result;
    }


} // -- end of namespace NoSqlDb

#endif // !QUERY_H