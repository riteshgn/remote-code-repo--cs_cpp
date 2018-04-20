#pragma once
//////////////////////////////////////////////////////////////////////////
// SingleDigitVersionMgr.h - Implements a single digit based versioning //
//                           system                                     //
// ver 1.0                                                              //
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
* ver 1.0 : 10 Mar 2018
* - first release
*/

#ifndef INTEGER_VERSIONMGR_H
#define INTEGER_VERSIONMGR_H

#include "IVersionMgr.h"
#include "../../NoSqlDb/DbCore/DbCore.h"

namespace SoftwareRepository
{
    ///////////////////////////////////////////////////////////////////////
    // SingleDigitVersion class
    // - payload for the NoSqlDb instance used by SingleDigitVersionMgr
    // - maintains the current version and owner information for a resource

    class SingleDigitVersion
    {
    public:
        int getCurrentVersion() { return currentVersion_; }
        void setCurrentVersion(ResourceVersion version) { currentVersion_ = version; }
        AuthorId getAuthorId() { return authorId_; }
        void setAuthorId(AuthorId authorId) { authorId_ = authorId; }
    private:
        ResourceVersion currentVersion_;
        AuthorId authorId_;
    };

    /////////////////////////////////////////////////////////////////////
    // SingleDigitVersionMgr class
    // - implements the IVersionMgr
    // - provides single digit based versioning mechanism i.e. v1, v2, ...
    // - provides individual file ownership policy

    class SingleDigitVersionMgr : public IVersionMgr
    {
    public:
        virtual ResourceVersion getCurrentVersion(ResourceIdentity) override;
        virtual ResourceVersion getNextVersion(ResourceIdentity) override;
        virtual ResourceVersion incrementVersionAndSave(ResourceIdentity, AuthorId) override;
        virtual bool isValidVersion(ResourceIdentity, ResourceVersion) override;
        virtual bool hasPermission(ResourceIdentity, AuthorId, Action = DEFAULT_RESOURCE_MODIFY_ACTION) override;
        ResourcePropsDbSize size() { return db_.size(); }
        void showKeys() { NoSqlDb::showKeys(db_); }

    private:
        NoSqlDb::DbCore<SingleDigitVersion> db_;
    };
}

#endif // !INTEGER_VERSIONMGR_H
