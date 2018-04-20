#pragma once
//////////////////////////////////////////////////////////////////////////
// FileSystemStore.h - Implements a File System based Repository Store  //
// ver 1.0                                                              //
// Language:    C++, Visual Studio 2017                                 //
// Application: SoftwareRepository, CSE687 - Object Oriented Design     //
// Author:      Ritesh Nair (rgnair@syr.edu)                            //
//////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements a repository store. It contains below classes:
* - FileSystemStore which provides APIs to save and load files on File System
*
* Required Files:
* ---------------
* IRepoStore.h
* FileResource.h, FileResource.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 10 Mar 2018
* - first release
*/

#ifndef FILESYSTEM_STORE_H
#define FILESYSTEM_STORE_H

#include "IRepoStore.h"
#include "../FileResource/FileResource.h"

namespace SoftwareRepository
{
    /////////////////////////////////////////////////////////////////////
    // FileSystemStore
    // - implements File System based repository semantics
    // - it required pointer to a version manager
    // - optionally a root folder can be specified in the constructor

    class FileSystemStore: public IRepoStore<FileResource>
    {
    public:
        FileSystemStore(FileStoreRoot rootFolder = DEFAULT_FILE_STORE_ROOT_FOLDER) :
            rootFolder_(rootFolder) {
            createRootDirectory();
        }

        bool load(FileResource, ResourceVersion, CheckoutLocation);
        bool save(FileResource, ResourceVersion, SourceLocation);

        FileStoreRoot getRootFolder() { return rootFolder_; }

        static bool createDirectory(std::string fullQualifiedDirPath);
        static bool createDirectoryRecursively(std::string);
        static std::string getFilenameForSave(ResourceIdentity, ResourceVersion);

    private:
        FileStoreRoot rootFolder_ = DEFAULT_FILE_STORE_ROOT_FOLDER;

        bool createRootDirectory();
    };
}

#endif // !FILESYSTEM_STORE_H
