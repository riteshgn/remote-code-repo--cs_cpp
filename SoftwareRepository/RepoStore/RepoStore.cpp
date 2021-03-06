///////////////////////////////////////////////////////////////////////
// RepoStore.cpp - Implements the RepoStore APIs                     //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: SoftwareRepository, CSE687 - Object Oriented Design  //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
///////////////////////////////////////////////////////////////////////

/*
* Maintenance History:
* --------------------
* ver 1.0 : 10 Mar 2018
* - first release
*/

#include "FileSystemStore.h"
#include "../FileSystemDemo/FileSystem.h"
#include "../VersionMgr/SingleDigitVersionMgr.h"

#include <iostream>

using namespace SoftwareRepository;

/////////////////////////////////////////////////////////////////////
// FileSystemStore methods

//----< loads the requested version of the resource to the specified location >---------------------

bool FileSystemStore::load(FileResource res, ResourceVersion version, CheckoutLocation location)
{
    std::string source = rootFolder_ + '\\' + FileSystemStore::getFilenameForSave(res.getIdentity(), version);

    if (!FileSystem::Directory::exists(location))
    {
        std::cout << "\n  Checkout directory '" << location << "' does not exist!\n";
        return false;
    }

    // copy from <repo_store_root>/<resourceId>.<version>
    //      to   <checkout_location>/<package|namespace>/<resourceName>

    std::string packageDirectory = location + '\\' + res.getPackageName();
    if (!FileSystem::Directory::exists(packageDirectory)
        && !FileSystemStore::createDirectory(packageDirectory))
    {
        std::cout << "\n  Package directory '" << packageDirectory << "' could not be created!\n";
        return false;
    }

    std::string destination = packageDirectory + '\\' + res.getResourceName();
    std::cout << "  --> Checking out " << res.getResourceName() << " (v" <<
        version << ") to " << destination << "\n";
    return FileSystem::File::copy(source, destination);
}

//----< saves the provided resource to repository backend >---------------------

bool FileSystemStore::save(FileResource res, ResourceVersion version, SourceLocation source)
{
    if (!FileSystem::File::exists(source))
    {
        std::cout << "\n  Source file '" << source << "' does not exist!";
        return false;
    }

    // copy from source location to <repo_store_root>/<resourceId>.<version>
    std::string destination = rootFolder_ + '\\' + FileSystemStore::getFilenameForSave(res.getIdentity(), version);
    std::cout << "  --> Committing " << source << " to repository as [" <<
        res.getResourceName() << " v" << version << "]\n";
    return FileSystem::File::copy(source, destination);
}

//----< creates the repositories root directory >---------------------

bool FileSystemStore::createRootDirectory()
{
    if (!FileSystemStore::createDirectoryRecursively(rootFolder_))
    {
        //std::cout << "Creation failed " << rootFolder_ << '\n';
        return false;
    }
    //std::cout << "Created successfully " << rootFolder_ << '\n';
    return true;
}

//----< helper function to create directories recursively >---------------------

bool FileSystemStore::createDirectoryRecursively(std::string directory)
{
    std::string fullQualifiedDirPath = FileSystem::Path::getFullFileSpec(directory);
    //std::cout << "Input " << fullQualifiedDirPath << '\n';
    int begin = 0;
    int end = fullQualifiedDirPath.find('\\');

    bool success = true;
    while (std::string::npos != end)
    {
        std::string thisDir = fullQualifiedDirPath.substr(0, begin + end);
        //std::cout << "Found " << thisDir << '\n';
        success = success && FileSystemStore::createDirectory(thisDir);
        if (!success)
        {
            //std::cout << "Creation failed " << thisDir << '\n';
            return false;
        }

        begin = begin + end + 1;
        end = fullQualifiedDirPath.substr(begin).find('\\');
    }
    return FileSystemStore::createDirectory(fullQualifiedDirPath);
    //return true;
}

//----< helper function to create a directory recursively >---------------------

bool FileSystemStore::createDirectory(std::string directory)
{
    if (!FileSystem::Directory::exists(directory))
    {
        //std::cout << "Creating " << directory << '\n';
        return !FileSystem::Directory::create(directory);
    }
    //std::cout << "Already exists " << directory << '\n';
    return true;
}

//----< compiles the filename for the resource to be saved >---------------------

std::string FileSystemStore::getFilenameForSave(ResourceIdentity resourceId, ResourceVersion version)
{
    return resourceId + '.' + std::to_string(version);
}

//----< test stub >----------------------------------------------------

#ifdef TEST_FILESYSTEM_STORE

int main()
{
    FileSystemStore store;

    FileResource res("test_ns", "testFile.h");
    bool result = store.save(res, 1, "../Demo/code/Projects/TreeWalkDemo/TreeWalkDemo.cpp");
    std::cout << "\n  Result save: " << std::to_string(result) << "\n\n";

    res.setPackageName("test_ns_package");
    result = store.load(res, 1, "../Demo/checkouts");
    std::cout << "\n  Result load: " << std::to_string(result) << "\n\n";

    return 0;
}

#endif // TEST_FILESYSTEM_STORE