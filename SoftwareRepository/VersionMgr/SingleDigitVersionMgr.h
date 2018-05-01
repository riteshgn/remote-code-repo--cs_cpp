#pragma once
//////////////////////////////////////////////////////////////////////////
// SingleDigitVersionMgr.h - Implements a single digit based versioning //
//                           system                                     //
// ver 1.1                                                              //
// Language:    C++, Visual Studio 2017                                 //
// Application: SoftwareRepository, CSE687 - Object Oriented Design     //
// Author:      Ritesh Nair (rgnair@syr.edu)                            //
//////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements a version manager using the NoSql database.
*
* Required Files:
* ---------------
* IVersionMgr.h
* DbCore.h, DbCore.cpp
*
* Maintenance History:
* --------------------
* ver 1.1 : 30 Apr 2018
* - SingleDigitVersion implements the IPayload interface and can now be persisted
* - added backup and restore functionality
* ver 1.0 : 10 Mar 2018
* - first release
*/

#ifndef INTEGER_VERSIONMGR_H
#define INTEGER_VERSIONMGR_H

#include "IVersionMgr.h"
#include "../../NoSqlDb/DbCore/DbCore.h"
#include "../../NoSqlDb/Payloads/IPayload.h"
#include "../../NoSqlDb/Persistence/Persistence.h"

namespace SoftwareRepository
{
    ///////////////////////////////////////////////////////////////////////
    // SingleDigitVersion class
    // - payload for the NoSqlDb instance used by SingleDigitVersionMgr
    // - maintains the current version and owner information for a resource

    class SingleDigitVersion : public NoSqlDb::IPayload<SingleDigitVersion>
    {
    public:
        int getCurrentVersion() { return currentVersion_; }
        void setCurrentVersion(ResourceVersion version) { currentVersion_ = version; }
        AuthorId getAuthorId() { return authorId_; }
        void setAuthorId(AuthorId authorId) { authorId_ = authorId; }

        friend std::ostream& operator<<(std::ostream& os, const SingleDigitVersion& payload)
        {
            return os << payload.toString();
        };

        virtual std::string toString() override { return toString(); }
        virtual NoSqlDb::IPayload<SingleDigitVersion>::Sptr toXmlElement() override;
        static SingleDigitVersion fromXmlElement(NoSqlDb::IPayload<SingleDigitVersion>::Sptr);

    private:
        ResourceVersion currentVersion_;
        AuthorId authorId_;

        std::string toString() const;
    };

    /////////////////////////////////////////////////////////////////////
    // SingleDigitVersionMgr class
    // - implements the IVersionMgr
    // - provides single digit based versioning mechanism i.e. v1, v2, ...
    // - provides individual file ownership policy

    class SingleDigitVersionMgr : public IVersionMgr
    {
    public:
        SingleDigitVersionMgr() : persistence_(db_, "VersionMgr") {}

        virtual ResourceVersion getCurrentVersion(ResourceIdentity) override;
        virtual ResourceVersion getNextVersion(ResourceIdentity) override;
        virtual ResourceVersion incrementVersionAndSave(ResourceIdentity, AuthorId) override;
        virtual bool isValidVersion(ResourceIdentity, ResourceVersion) override;
        virtual bool hasPermission(ResourceIdentity, AuthorId, Action = DEFAULT_RESOURCE_MODIFY_ACTION) override;
        ResourcePropsDbSize size() { return db_.size(); }
        void showKeys() { NoSqlDb::showKeys(db_); }

        virtual void loadDb(const SourceLocation&) override;
        virtual void saveDb(const SourceLocation&) override;

    private:
        NoSqlDb::DbCore<SingleDigitVersion> db_;
        NoSqlDb::Persistence<SingleDigitVersion> persistence_;
    };
}

#endif // !INTEGER_VERSIONMGR_H
