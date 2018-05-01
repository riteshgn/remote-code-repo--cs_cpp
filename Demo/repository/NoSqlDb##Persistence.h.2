#pragma once
///////////////////////////////////////////////////////////////////////
// Persistence.h - Implements the persistence layer for NoSqlDb       //
// ver 1.2                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: NoSqlDb, CSE687 - Object Oriented Design             //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides an interface and its implementation:
* - IPersistence interface defines the behavior to be implemented by Persistence providers.
* - Persistence is an implementation of IPersistence interface which, as per the contract,
    provides an API to exports selected DB records to a file. It also provides an API to 
    restore/augment DB records from a file.

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* XmlDocument.h, XmlDocument.cpp
* XmlElement.h, XmlElement.cpp
*
* Maintenance History:
* --------------------
* ver 1.2 : 16 Apr 2018
* - restricts Payload to be of type IPayload
* - uses the interface's methods to serialize the payload to and from XML
* ver 1.1 : 06 Feb 2018
* - defined the IPersistence interface and Persistence class implements this interface
* - made output/input serialization format extensible; this release only supports xml
* ver 1.0 : 05 Feb 2018
* - first release
*/

#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "../DbCore/DbCore.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"

#include <fstream>
#include <string>
#include <vector>

const std::string DEFAULT_SHARD_NAME = "New Shard";
const bool DEFAULT_PRESERVE_ORIGINAL = true;

namespace NoSqlDb {

    /////////////////////////////////////////////////////////////////////
    // IPersistence interface
    // - defines the contract for all persistence providers
    // - lists the supported serialzation format (specified by enum StoreType)

    template <typename T>
    class IPersistence
    {
    public:
        using FilePath = std::string;
        using Key = std::string;
        using Keys = std::vector<Key>;

        enum StoreType { xml };

        virtual bool exportDb(const Keys& keys, const FilePath& filePath, StoreType storeType) const = 0;
        virtual Keys importDb(const FilePath& filePath, bool preserveOriginal, StoreType storeType) const = 0;
    };

    /////////////////////////////////////////////////////////////////////
    // Persistence class
    // - Implements the IPersistence interface and provides APIs to import/export database
    // - Supports only XML serialization in below format
    //      <shard name="SHARD_NAME">
    //          <record>
    //              <key>A DB KEY</key>
    //              <value>
    //                  <metadata>
    //                      <name>A NAME</name>
    //                      <description>SOME DESCRIPTION</description>
    //                      <datetime>SOME DATETIME</datetime>
    //                      <relationships>
    //                          <childkey>A CHILD KEY</childkey>
    //                          ...
    //                      </relationships>
    //                  </metadata>
    //                  <payload>DEFINED BY TEMPLATE T</payload>
    //              </value>
    //          </record>
    //          ...
    //      </shard>
    // - Supported datetime format (example: Tue Feb  6 02:32:54 2018)

    template <typename T>
    class Persistence: public IPersistence<T>
    {
    public:
        using ShardName = std::string;
        using FilePath = std::string;
        using Key = std::string;
        using Keys = std::vector<Key>;
        using Sptr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;

    private:
        DbCore<T>& db_;
        ShardName shardName_ = DEFAULT_SHARD_NAME;

        DbElement<T> createDbElement(std::vector<Sptr> pValue) const;
        DbElementMetadata createDbElementMetadata(std::vector<Sptr> pMetadata) const;
        XmlProcessing::XmlDocument makeXml(const Keys& keys, ShardName shardName) const;
        Keys parseXmlAndSaveToDb(const FilePath& filePath, bool preserverOriginal) const;
        Key saveRecordToDb(std::vector<Sptr> pXmlElem, bool preserverOriginal) const;
        bool validateXml(XmlProcessing::XmlDocument* xmlDoc) const;
        bool writeXmlToFile(const FilePath& filePath, XmlProcessing::XmlDocument xmlDoc) const;

    public:
        Persistence(DbCore<T>& db) : db_(db) {}
        Persistence(DbCore<T>& db, ShardName shardName) : db_(db), shardName_(shardName) {}

        virtual bool exportDb(const Keys& keys, const FilePath& filePath, 
            IPersistence<T>::StoreType storeType = IPersistence<T>::xml) const override
        {
            return writeXmlToFile(filePath, makeXml(keys, shardName_));
        }

        virtual Keys importDb(const FilePath& filePath, 
            bool preserveOriginal = DEFAULT_PRESERVE_ORIGINAL,
            IPersistence<T>::StoreType storeType = IPersistence<T>::xml) const override
        {
            return parseXmlAndSaveToDb(filePath, preserveOriginal);
        }
    };

    /////////////////////////////////////////////////////////////////////
    // Persistence<T> methods

    //----< returns db element object by parsing an Xml record >---------------------

    template <typename T>
    DbElement<T> Persistence<T>::createDbElement(std::vector<Sptr> pValue) const
    {
        DbElement<T> dbElem;

        for (auto pValueChild : pValue)
        {
            if (pValueChild->tag() == "metadata")
            {
                std::vector<Sptr> pMetadata = pValueChild->children();
                DbElementMetadata metadata = createDbElementMetadata(pMetadata);
                dbElem.metadata(metadata);
            }
            else if (pValueChild->tag() == "payload")
            {
                dbElem.payLoad(T::fromXmlElement(pValueChild));
            }
        }

        return dbElem;
    }

    //----< returns db element metadata object by parsing an Xml record >---------------------

    template <typename T>
    DbElementMetadata Persistence<T>::createDbElementMetadata(std::vector<Sptr> pMetadata) const
    {
        DbElementMetadata metadata;

        for (auto pMetadataChild : pMetadata)
        {
            if (pMetadataChild->children().size() > 0)
            {
                if (pMetadataChild->tag() == "name")
                {
                    metadata.name(pMetadataChild->children()[0]->value());
                }
                else if (pMetadataChild->tag() == "description")
                {
                    metadata.descrip(pMetadataChild->children()[0]->value());
                }
                else if (pMetadataChild->tag() == "datetime")
                {
                    DateTime dt(pMetadataChild->children()[0]->value());
                    metadata.dateTime(dt);
                }
                else if (pMetadataChild->tag() == "relationships")
                {
                    std::vector<Sptr> pRelationships = pMetadataChild->children();
                    for (auto pChild : pRelationships)
                    {
                        if (pChild->children().size() > 0)
                            metadata.children().push_back(pChild->children()[0]->value());
                    }
                }
            }
        }

        return metadata;
    }

    //----< helper function for Persistence<T>::makeXml >---------------------

    template <typename T>
    void _addToXml(std::shared_ptr<XmlProcessing::AbstractXmlElement> pShard, 
        std::string dbKey, DbCore<T>& db)
    {
        using namespace XmlProcessing;
        using Sptr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;
        using Key = std::string;
        using Keys = std::vector<Key>;

        // create a "record" tag
        // this will hold the key and value of the DB record
        Sptr pRecord = makeTaggedElement("record");
        pShard->addChild(pRecord);

        // add the db key
        Sptr pKey = makeTaggedElement("key", dbKey);
        pRecord->addChild(pKey);

        // create a "value" tag
        // this will hold the metadata and payload of the DB element
        DbElement<T> thisElement = db[dbKey];
        Sptr pValue = makeTaggedElement("value");
        pRecord->addChild(pValue);

        // metadata tag and its children
        Sptr pMetadata = makeTaggedElement("metadata");
        pValue->addChild(pMetadata);
        Sptr pName = makeTaggedElement("name", thisElement.metadata().name());
        pMetadata->addChild(pName);
        Sptr pDescrip = makeTaggedElement("description", thisElement.metadata().descrip());
        pMetadata->addChild(pDescrip);
        Sptr pDateTime = makeTaggedElement("datetime", thisElement.metadata().dateTime());
        pMetadata->addChild(pDateTime);

        // relationships... its part of the record's metadata
        Keys children = thisElement.metadata().children();
        if (children.size() > 0)
        {
            Sptr pRelationships = makeTaggedElement("relationships");
            pMetadata->addChild(pRelationships);
            for (Key childkey : children)
            {
                Sptr pChildKey = makeTaggedElement("childkey", childkey);
                pRelationships->addChild(pChildKey);
            }
        }

        // the payload...
        Sptr pPayLoad = thisElement.payLoad().toXmlElement();
        pValue->addChild(pPayLoad);
    }

    //----< returns an Xml document serialization of the provided DB records >---------------------

    template <typename T>
    XmlProcessing::XmlDocument Persistence<T>::makeXml(const Keys& keys, ShardName shardName) const
    {
        using namespace XmlProcessing;

        // create an Xml Document object with the root tag "shard"
        Sptr pShard = makeTaggedElement("shard");
        pShard->addAttrib("name", shardName);
        Sptr pDocElem = makeDocElement(pShard);
        XmlDocument xmlDoc(pDocElem);

        // iterate over the list of keys whose records have to be persisted 
        for (Key dbKey : keys)
        {
            _addToXml<T>(pShard, dbKey, db_);
        }

        return xmlDoc;
    }

    //----< validates & deserializes a xml document and saves records to DB >---------------------

    template <typename T>
    typename Persistence<T>::Keys Persistence<T>::parseXmlAndSaveToDb(const FilePath& filePath, bool preserveOriginal) const
    {
        using namespace XmlProcessing;

        Keys keys;

        XmlDocument xmlDoc(filePath, XmlDocument::file);
        if (!validateXml(&xmlDoc))
            return keys;

        std::vector<Sptr> records = xmlDoc.descendents("record").select();
        for (auto pRecord : records)
        {
            std::vector<Sptr> pKeyValue = pRecord->children();
            keys.push_back(saveRecordToDb(pKeyValue, preserveOriginal));
        }

        return keys;
    }

    //----< parses DB records serialized as Xml and saves to DB >---------------------

    template <typename T>
    typename Persistence<T>::Key Persistence<T>::saveRecordToDb(std::vector<Sptr> pKeyValue, bool preserveOriginal) const
    {
        Key key;
        DbElement<T> dbElem;
        bool recordExists = false;

        for (auto pKeyValueChild : pKeyValue)
        {
            if (pKeyValueChild->tag() == "key")
            {
                if (pKeyValueChild->children().size() == 0)
                    break;

                key = pKeyValueChild->children()[0]->value();
                if (db_.contains(key)
                    && preserveOriginal)
                {
                    recordExists = true;
                    break;
                }
            }
            else
            {
                std::vector<Sptr> pValue = pKeyValueChild->children();
                dbElem = createDbElement(pValue);
            }
        }

        if (!recordExists)
            db_[key] = dbElem;

        return key;
    }

    //----< validates if xml document has required keys >---------------------

    template <typename T>
    bool Persistence<T>::validateXml(XmlProcessing::XmlDocument* pXmlDoc) const
    {
        std::vector<Sptr> found = pXmlDoc->element("shard").select();
        if (found.size() == 0)
            return false;

        found = pXmlDoc->element("record").select();
        if (found.size() == 0)
            return false;

        return true;
    }

    //----< serializes selected db records to xml file >---------------------

    template <typename T>
    bool Persistence<T>::writeXmlToFile(const FilePath& filePath, XmlProcessing::XmlDocument xmlDoc) const
    {
        using OutFileStream = std::ofstream;
        using WriteMode = std::ios;

        OutFileStream outf(filePath, WriteMode::trunc);
        if (!outf)
        {
            return false;
        }

        std::string xmlStr = xmlDoc.toString();
        outf << xmlStr << std::endl;
        outf.close();

        return true;
    }
}

#endif // !PERSISTENCE_H
