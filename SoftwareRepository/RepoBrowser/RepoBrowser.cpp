///////////////////////////////////////////////////////////////////////
// RepoBrowser.cpp - Implements the RepoBrowser APIs                 //
// ver 1.1                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: SoftwareRepository, CSE687 - Object Oriented Design  //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
///////////////////////////////////////////////////////////////////////

/*
* Maintenance History:
* --------------------
* ver 1.1 : 20 Apr 2018
* - added implementation for browse filters
* - implemented changes to repo browser APIs
* ver 1.0 : 10 Mar 2018
* - first release
*/

#include "ResultProcessors.h"
#include "BrowserFilters.h"
#include "RepoBrowser.h"
#include "../RepoUtilities/RepoUtilities.h"

#include <iostream>

using namespace SoftwareRepository;
using namespace NoSqlDb;

/////////////////////////////////////////////////////////////////////
// ConsoleResultProcessor methods

//----< prints each resource to the standard console output >---------------------

/*
 *  - Prints verbose details for the resource on which browse request was initiated.
 *    Defined as level 0
 *  - Only name and version information are printed for dependencies 
 */

bool ConsoleResultProcessor::operator()(FileResource res, ResourceVersion version, Level level)
{
    if (level == 0)
    {
        std::cout << "\n  " << std::string(80, '-');
        std::cout << "\n    Name:         " << res.getResourceName() << " (version: " << version << ")";
        std::cout << "\n    Description:  " << res.getDescription();
        std::cout << "\n    Package:      " << res.getPackageName();
        std::cout << "\n    Categories:   " << stringifyCategories(res.getCategories());
        std::cout << "\n  " << std::string(80, '-') << "\n";
        std::cout << "\n  Dependencies: \n";
        return true;
    }

    std::cout << std::string(level * 2, ' ') << "|->  " <<
        res.getResourceName() << " (version: " << version << ")\n";
    return true;
}

//----< applies a query condition on the filename in the database >---------------------

Query<FileResourcePayload> FilenameFilter::apply(const Query<FileResourcePayload>& query)
{
    return query.where.metadata.eqName(filename_);
}

//----< applies a query condition on the file version in the database >---------------------

Query<FileResourcePayload> FileVersionFilter::apply(const Query<FileResourcePayload>& query)
{
    std::function<bool(FileResourcePayload)> thisVersion = [&](FileResourcePayload payload) {
        return payload.getVersion() == version_;
    };

    return query.where.payload.has(thisVersion);
}

//----< applies a query condition on the package name in the database >---------------------

Query<FileResourcePayload> PackageFilter::apply(const Query<FileResourcePayload>& query)
{
    std::function<bool(FileResourcePayload)> thisPackage = [&](FileResourcePayload payload) {
        return payload.getPackageName() == package_;
    };

    return query.where.payload.has(thisPackage);
}

/////////////////////////////////////////////////////////////////////
// RepoBrowser methods

//----< checks if a given version of a resource exists in the database >---------------------------

bool RepoBrowser::exists(ResourceIdentity resourceId, ResourceVersion version)
{
    ResourcePropsDbKey dbKey = getDbKeyForVersion(resourceId, version);
    return db_.contains(dbKey);
}

//----< fetches properties for a given version of a resource from properties database >---------------------------

ResourceProperties& RepoBrowser::get(ResourceIdentity resourceId, ResourceVersion version) {
    ResourcePropsDbKey dbKey = getDbKeyForVersion(resourceId, version);
    if (db_.contains(dbKey))
    {
        currProp_ = ResourceProperties(db_, dbKey);
        return currProp_;
    }

    // TODO: handle this scenario
    throw std::exception("Resource does not exist");
}

void RepoBrowser::executeQuery(FileResource res, ResourceVersion version, ResultProcessors processors)
{
    clearVisitedDeps();
    processResource(res.getIdentity(), version, 0, processors);
}

//----< searches the properties db with given filters and executes the callback on each result >---------------------------

void RepoBrowser::executeQuery(Filters filters, ResultProcessors processors)
{
    Query<FileResourcePayload> query;
    query.from(db_);

    for (Filter& filter : filters)
    {
        filter.apply(query);
    }

    DbCore<FileResourcePayload> resultDb = query.end();

    for (std::string key : resultDb.keys())
    {
        ResourceProperties thisKeyProps(db_, key);
        FileResource res(thisKeyProps.getNamespace(), thisKeyProps.getName());
        res.setDescription(thisKeyProps.getDescription());
        res.setPackageName(thisKeyProps.getRawPayload().getPackageName());
        for (Category category : thisKeyProps.getCategories())
            res.setCategory(category);

        ResourceVersion version = thisKeyProps.getRawPayload().getVersion();

        clearVisitedDeps();
        executeQuery(res, version, processors);
    }
}

//----< browses repository for a particular resource version >---------------------

void RepoBrowser::processResource(ResourceIdentity resourceId,
    ResourceVersion version, Level level,
    ResultProcessors processors)
{
    // if resource and version does not exist then return
    if (!exists(resourceId, version))
    {
        std::cout << "\n '" << resourceId << "' v" << version << " not found in db";
        return;
    }

    // if the resource was previously visited then no need to process it.
    // the 'visited_' map is used to keep track of all previosuly visited processes.
    // key for the map is <resource_id>-<version>

    std::string visitedKey = resourceId + '-' + std::to_string(version);
    bool firstVisit = visited_.find(visitedKey) == visited_.end();

    if (firstVisit)
    {
        // mark visited
        visited_[visitedKey] = true;

        // fetch the properties from DB and pass the resource to all result processors 
        IResourceProperties<FileResourcePayload>& props = get(resourceId, version);
        FileResource res(props.getNamespace(), props.getName());
        res.setDescription(props.getDescription());
        res.setPackageName(props.getRawPayload().getPackageName());
        for (Category category : props.getCategories())
            res.setCategory(category);

        for (ResultProcessor& processResult : processors)
        {
            processResult(res, version, level);
        }

        // process its dependencies
        Dependencies deps = props.getDependencies();
        for (ResourceIdentityWithVersion elem : deps)
        {
            processResource(elem.first, elem.second, level + 1, processors);
        }
    }
}

///////////////////////////////////////////////////////////////////////
// test functions

// Test Helper functions

//----< populates a test properties db with test data >----------------------

//RepoBrowser::FileResources populateHeirarchyInPropertiesDb(IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb)
//{
//    RepoBrowser::FileResources fileResources;
//    fileResources.push_back(FileResource("test_ns", "repo-browser-1.h").setDescription("Amazing Package 1 - Header"));
//    fileResources.push_back(FileResource("test_ns", "repo-browser-1.cpp").setDescription("Amazing Package 1 - Implementation"));
//    fileResources.push_back(FileResource("test_ns", "repo-browser-2.h").setDescription("Amazing Package 2 - Header"));
//    fileResources.push_back(FileResource("test_ns", "repo-browser-3.h").setDescription("Amazing Package 3 - Header"));
//    fileResources.push_back(FileResource("test_ns2", "repo-browser-4.h").setDescription("Amazing Package 4 - Header"));
//    fileResources.push_back(FileResource("test_ns2", "repo-browser-5.h").setDescription("Amazing Package 5 - Header"));
//    fileResources.push_back(FileResource("test_ns2", "repo-browser-5.cpp").setDescription("Amazing Package 5 - Implementation"));
//    fileResources.push_back(FileResource("test_ns", "repo-browser-6.h").setDescription("Amazing Package 6 - Header"));
//    fileResources.push_back(FileResource("test_ns", "repo-browser-6.cpp").setDescription("Amazing Package 6 - Implementation"));
//
//    pPropsDb->createEntry(fileResources[0], "test_ns_owner");
//    pPropsDb->createEntry(fileResources[1], "test_ns_owner");
//    pPropsDb->createEntry(fileResources[2], "test_ns_owner");
//    pPropsDb->createEntry(fileResources[3], "test_ns_owner");
//    pPropsDb->createEntry(fileResources[4], "test_ns2_owner");
//    pPropsDb->createEntry(fileResources[5], "test_ns2_owner");
//    pPropsDb->createEntry(fileResources[6], "test_ns2_owner");
//    pPropsDb->createEntry(fileResources[7], "test_ns2_owner");
//    pPropsDb->createEntry(fileResources[8], "test_ns2_owner");
//
//    pPropsDb->get(fileResources[0].getIdentity())
//        .addDependency(fileResources[2].getIdentity(), 1);
//
//    pPropsDb->get(fileResources[1].getIdentity())
//        .addDependency({ {fileResources[0].getIdentity(), 1}, { fileResources[6].getIdentity(), 1 } });
//
//    pPropsDb->get(fileResources[2].getIdentity())
//        .addDependency({ { fileResources[3].getIdentity(), 1 }, { fileResources[4].getIdentity(), 1 } });
//
//    pPropsDb->get(fileResources[6].getIdentity())
//        .addDependency({ {fileResources[5].getIdentity(), 1}, { fileResources[4].getIdentity(), 1 } });
//
//    pPropsDb->get(fileResources[4].getIdentity())
//        .addDependency(fileResources[7].getIdentity(), 1);
//
//    pPropsDb->get(fileResources[8].getIdentity())
//        .addDependency(fileResources[7].getIdentity(), 1);
//
//    std::cout << "\n  Initialized properties DB with below keys\n";
//    pPropsDb->showKeys();
//    std::cout << "\n";
//
//    return fileResources;
//}

//----< test stub >----------------------------------------------------

#ifdef TEST_REPOBROWSER

int main()
{
    /*SingleDigitVersionMgr versionMgr;
    IVersionMgr *pVersionMgr = &versionMgr;

    ResourcePropertiesDb propsDb(pVersionMgr);
    IResourcePropertiesDb<FileResource, FileResourcePayload> *pPropsDb = &propsDb;

    RepoBrowser::FileResources fileResources = populateHeirarchyInPropertiesDb(pPropsDb);
    RepoBrowser repoBrowser(pPropsDb);

    std::cout << "\n  Browsing repo for file :" << fileResources[1].getResourceName() << " (version: 1)";
    repoBrowser.browse(fileResources[1], 1);
    std::cout << "\n";*/

    return 0;
}

#endif // TEST_REPOBROWSER