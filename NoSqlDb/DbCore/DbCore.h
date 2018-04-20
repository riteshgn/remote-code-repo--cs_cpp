#pragma once
///////////////////////////////////////////////////////////////////////
// DbCore.h - Implements NoSql database prototype                    //
// ver 1.5                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: NoSqlDb, CSE687 - Object Oriented Design             //
// Author:      Ritesh Nair                                          //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides two classes:
* - DbCore implements core NoSql database operations, but does not
*   provide editing, querying, or persisting.  Those are left as
*   exercises for students.
* - DbElement provides the value part of our key-value database.
*   It contains a metadata field & a payload field of the template type.
* - DbElementMetadata stores the metadata part of the DbElement.
*   It contains fields for name, description, date, child collection.
* The package also provides functions for displaying:
* - set of all database keys
* - database elements
* - all records in the database

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
*
* Maintenance History:
* --------------------
* ver 1.5 : 16 Apr 2018
* - Fixed bug in add function
* ver 1.4 : 15 Apr 2018
* - Moved implementation of DbElementMetadata to DbCore.cpp file
* ver 1.3 : 4 Feb 2018
* - added implementation for requirements 3 and 4
*   i.e. addition/removal of key/values and editing of values
* ver 1.2 : 2 Feb 2018
* - pulled metadata fields from DbElement into a separate DbElementMetadata class
* ver 1.1 : 19 Jan 2018
* - added code to throw exception in index operators if input key is not in database
* ver 1.0 : 10 Jan 2018
* - first release
*/

#ifndef DBCORE_H
#define DBCORE_H

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "../DateTime/DateTime.h"

namespace NoSqlDb
{
    /////////////////////////////////////////////////////////////////////
    // DbElementMetadata class
    // - provides the metadata part of a NoSql key-value database element

    class DbElementMetadata
    {
    public:
        using Key = std::string;
        using Children = std::vector<Key>;

    private:
        std::string name_;
        std::string descrip_;
        DateTime dateTime_;
        Children children_;

    public:
        std::string& name() { return name_; }
        std::string name() const { return name_; }
        void name(const std::string& name) { name_ = name; }


        std::string& descrip() { return descrip_; }
        std::string descrip() const { return descrip_; }
        void descrip(const std::string& name) { descrip_ = name; }

        DateTime& dateTime() { return dateTime_; }
        DateTime dateTime() const { return dateTime_; }
        void dateTime(const DateTime& dateTime) { dateTime_ = dateTime; }

        Children& children() { return children_; }
        Children children() const { return children_; }
        void children(const Children& children) { children_ = children; }

        DbElementMetadata& addRelationship(const Key& child);
        DbElementMetadata& removeRelationship(const Key& child);
    };

    /////////////////////////////////////////////////////////////////////
    // DbElement class
    // - provides the value part of a NoSql key-value database

    template<typename T>
    class DbElement
    {
    public:
        using Key = std::string;

    private:
        DbElementMetadata metadata_;
        T payLoad_;

    public:
        // methods to get and set DbElement fields
        DbElementMetadata& metadata() { return metadata_; }
        DbElementMetadata metadata() const { return metadata_; }
        void metadata(const DbElementMetadata& metadata) { metadata_ = metadata; }
        
        T& payLoad() { return payLoad_; }
        T payLoad() const { return payLoad_; }
        void payLoad(const T& payLoad) { payLoad_ = payLoad; }

        DbElement<T>& addRelationship(const Key& childKey) 
        { 
            metadata_.addRelationship(childKey); 
            return *this; 
        }
        DbElement<T>& removeRelationship(const Key& childKey) { metadata_.removeRelationship(childKey); return *this; }
    };

    /////////////////////////////////////////////////////////////////////
    // DbCore class
    // - provides core NoSql db operations
    // - does not provide editing, querying, or persistance operations

    template <typename T>
    class DbCore
    {
    public:
        using Key = std::string;
        using Keys = std::vector<Key>;
        using Children = Keys;
        using DbStore = std::unordered_map<Key, DbElement<T>>;
        using Pairs = std::unordered_map<Key, DbElement<T>>;
        using iterator = typename DbStore::iterator;

        // methods to access database elements

        Keys keys();
        bool contains(const Key& key);
        size_t size();
        void throwOnIndexNotFound(bool doThrow) { doThrow_ = doThrow; }
        DbElement<T>& operator[](const Key& key);
        DbElement<T> operator[](const Key& key) const;

        // methods for CRUD operations
        bool add(const Key& key, const DbElement<T>& element);
        bool add(const Pairs& keyValuePairs);
        bool remove(const Key& key);
        bool truncate();

        DbCore<T>& addRelationship(const Key& dbKey, const Key& childKey)
        { 
            dbStore_[dbKey].addRelationship(childKey);
            return *this;
        }
        DbCore<T>& removeRelationship(const Key& dbKey, const Key& childKey)
        { 
            dbStore_[dbKey].removeRelationship(childKey);
            return *this;
        }
        DbCore<T>& replacePayLoad(const Key& key, const T& payLoad)
        { 
            dbStore_[key].payLoad(payLoad); 
            return *this;  
        }

        // iterator implementation
        typename iterator begin() { return dbStore_.begin(); }
        typename iterator end() { return dbStore_.end(); }

        // methods to get and set the private database hash-map storage

        DbStore& dbStore() { return dbStore_; }
        DbStore dbStore() const { return dbStore_; }
        void dbStore(const DbStore& dbStore) { dbStore_ = dbStore; }

    private:
        DbStore dbStore_;
        bool doThrow_ = false;
    };

    /////////////////////////////////////////////////////////////////////
    // DbCore<T> methods

    //----< does db contain this key? >----------------------------------

    template<typename T>
    bool DbCore<T>::contains(const Key& key)
    {
        iterator iter = dbStore_.find(key);
        return iter != dbStore_.end();
    }
    //----< returns current key set for db >-----------------------------

    template<typename T>
    typename DbCore<T>::Keys DbCore<T>::keys()
    {
        DbCore<T>::Keys dbKeys;
        DbStore& dbs = dbStore();
        size_t size = dbs.size();
        dbKeys.reserve(size);
        for (auto item : dbs)
        {
            dbKeys.push_back(item.first);
        }
        return dbKeys;
    }
    //----< return number of db elements >-------------------------------

    template<typename T>
    size_t DbCore<T>::size()
    {
        return dbStore_.size();
    }
    //----< extracts value from db with key >----------------------------
    /*
    *  - indexes non-const db objects
    *  - In order to create a key-value pair in the database like this:
    *      db[newKey] = newDbElement
    *    we need to index with the new key and assign a default element.
    *    That will be replaced by newDbElement when we return from operator[]
    *  - However, if we want to index without creating new elements, we need
    *    to throw an exception if the key does not exist in the database.
    *  - The behavior we get is determined by doThrow_.  If false we create
    *    a new element, if true, we throw. Creating new elements is the default
    *    behavior.
    */
    template<typename T>
    DbElement<T>& DbCore<T>::operator[](const Key& key)
    {
        if (!contains(key))
        {
            if (doThrow_)
                throw(std::exception("key does not exist in db"));
            else
                return (dbStore_[key] = DbElement<T>());
        }
        return dbStore_[key];
    }
    //----< extracts value from db with key >----------------------------
    /*
    *  - indexes const db objects
    */
    template<typename T>
    DbElement<T> DbCore<T>::operator[](const Key& key) const
    {
        if (!contains(key))
        {
            throw(std::exception("key does not exist in db"));
        }
        return dbStore_[key];
    }

    //----< adds a value to db with key >----------------------------
    /*
    *  - This is simply a convenience wrapper function around the indexing operator
    *    to add elements into the database.
    *  - So instead of writing 
    *       db[newKey] = newDbElement;
    *    you can write
    *       db.add(newKey, newDbElement);
    *  - If the key exists then the metadata and the payload wil be overridden.
    */
    template<typename T>
    bool DbCore<T>::add(const Key& key, const DbElement<T>& element)
    {
        dbStore_[key] = element;
        return true;
    }

    //----< adds a value to db with key >----------------------------
    /*
    *  - This functions allows to submit multiple key values to the DB.
    *  - If the key exists then the metadata and the payload wil be overridden.
    */
    template<typename T>
    bool DbCore<T>::add(const Pairs& keyValuePairs)
    {
        for (std::pair<std::string, DbElement<T>> record : keyValuePairs)
        {
            add(record.first, record.second);
        }
        return true;
    }

    //----< removes a value from db with key >----------------------------
    /*
    *  - Erases a key and thereby its corresponding value from the database.
    *  - It returns a boolean where true indicates that the element was
    *    successfully removed from the Db and false otherwise.
    *  - A false could also mean that the key does not exist in the database.
    *    However, the behavior can be customized to throw an exception instead.
    *  - The behavior we get is determined by doThrow_.  If false return then
    *    return the boolean false, if true, we throw. Returning boolean is the
    *    default behavior.
    */
    template<typename T>
    bool DbCore<T>::remove(const Key& key)
    {
        if (!contains(key))
        {
            if (doThrow_)
                throw(std::exception("key does not exist in db"));
            else
                return false;
        }
        return (dbStore_.erase(key) == 1);
    }

    //----< truncates the db >----------------------------
    /*
    *  - Removes all entries from the database.
    *  - Returns a boolean which indicates if the operation was successful.
    */
    template<typename T>
    bool DbCore<T>::truncate()
    {
        dbStore_.clear();
        return true;
    }

    /////////////////////////////////////////////////////////////////////
    // display functions

    //----< display database key set >-----------------------------------

    template<typename T>
    void showKeys(DbCore<T>& db, std::ostream& out = std::cout)
    {
        out << "\n  ";
        for (auto key : db.keys())
        {
            out << key << " ";
        }
    }
    //----< show record header items >-----------------------------------

    inline void showHeader(std::ostream& out = std::cout)
    {
        out << "\n  ";
        out << std::setw(26) << std::left << "DateTime";
        out << std::setw(17) << std::left << "Name";
        out << std::setw(35) << std::left << "Description";
        out << std::setw(25) << std::left << "Payload";
        out << "\n  ";
        out << std::setw(26) << std::left << "------------------------";
        out << std::setw(17) << std::left << "---------------";
        out << std::setw(35) << std::left << "---------------------------------";
        out << std::setw(25) << std::left << "-----------------------";
    }
    //----< display DbElements >-----------------------------------------

    template<typename T>
    void showElem(const DbElement<T>& el, std::ostream& out = std::cout)
    {
        out << "\n  ";
        out << std::setw(26) << std::left << std::string(el.metadata().dateTime());
        out << std::setw(17) << std::left << el.metadata().name();
        out << std::setw(35) << std::left << el.metadata().descrip();
        out << std::setw(25) << std::left << el.payLoad();
        typename DbElementMetadata::Children children = el.metadata().children();
        if (children.size() > 0)
        {
            out << "\n    child keys: ";
            for (auto key : children)
            {
                out << " " << key;
            }
        }
    }
    //----< display all records in database >----------------------------

    template<typename T>
    void showDb(const DbCore<T>& db, std::ostream& out = std::cout)
    {
        showHeader(out);
        typename DbCore<T>::DbStore dbs = db.dbStore();
        for (auto item : dbs)
        {
            showElem(item.second, out);
        }
    }
}

#endif // !DBCORE_H