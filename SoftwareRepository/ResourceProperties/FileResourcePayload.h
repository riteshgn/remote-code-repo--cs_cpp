#pragma once
///////////////////////////////////////////////////////////////////////
// FileResourcePayload.h - Implements payload from a properties DB   //
//                         which supports file-based storage         //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: SoftwareRepository, CSE687 - Object Oriented Design  //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements NoSqlDb payload of a file-based resource. It contains:
* - FileResourcePayload class which encapsulates the basic identity of a file like
*   its name and author along with details like the category it belongs to
*   and its commit state.
*
* Required Files:
* ---------------
* RepoCoreDefinitions.h
* RepoUtilities.h, RepoUtilities.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 24 Feb 2018
* - first release
*/

#ifndef FILERESOURCE_PAYLOAD_H
#define FILERESOURCE_PAYLOAD

#include "../RepoCore/RepoCoreDefinitions.h"
#include "../RepoUtilities/RepoUtilities.h"

#include <algorithm>

namespace SoftwareRepository
{

    /////////////////////////////////////////////////////////////////////
    // FileResourcePayload class
    // - provides APIs to store and access
    //   - author       std::string
    //   - categories   std::vector<std::string>
    //   - namesapce    std::string
    //   - package      std::string
    //   - state        int
    //   - version      int
    // - provides API to remove a category

    class FileResourcePayload
    {
    public:
        // methods to access payload's details

        AuthorId & getAuthor() { return author_; }
        AuthorId getAuthor() const { return author_; }

        Categories& getCategories() { return categories_; }
        Categories getCategories() const { return categories_; }

        Namespace& getNamespace() { return namespace_; }
        Namespace getNamespace() const { return namespace_; }

        PackageName& getPackageName() { return package_; }
        PackageName getPackageName() const { return package_; }

        State& getState() { return state_; }
        State getState() const { return state_; }

        ResourceVersion& getVersion() { return version_; }
        ResourceVersion getVersion() const { return version_; }

        // methods to set the payload's details

        FileResourcePayload& addCategory(Category category)
        {
            Categories::iterator found = std::find(categories_.begin(), categories_.end(), category);
            if (found == categories_.end())
                categories_.push_back(category);

            return *this;
        }

        FileResourcePayload& removeCategory(Category category)
        {
            categories_.erase(std::remove(categories_.begin(), categories_.end(), category), categories_.end());
            return *this;
        }

        FileResourcePayload& setAuthor(AuthorId author)
        {
            author_ = author;
            return *this;
        }

        FileResourcePayload& setNamespace(Namespace ns)
        {
            namespace_ = ns;
            return *this;
        }

        FileResourcePayload& setPackageName(PackageName package)
        {
            package_ = package;
            return *this;
        }

        FileResourcePayload& setState(State state)
        {
            state_ = state;
            return *this;
        }

        FileResourcePayload& setVersion(ResourceVersion version)
        {
            version_ = version;
            return *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const FileResourcePayload& payload)
        {
            return os << payload.toString();
        };

    private:
        AuthorId author_;
        State state_; // see: RESOURCE_STATE in RepoCoreDefinitions.h
        Categories categories_;
        Namespace namespace_;
        PackageName package_;
        ResourceVersion version_;

        std::string toString() const;
        std::string stringifyCategories() const;
    };
}

#endif // !FILERESOURCE_PAYLOAD_H
