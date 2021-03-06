///////////////////////////////////////////////////////////////////////
// RepoBrowser.cpp - Implements the RepoBrowser APIs                 //
// ver 1.2                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: SoftwareRepository, CSE687 - Object Oriented Design  //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
///////////////////////////////////////////////////////////////////////

/*
* Maintenance History:
* --------------------
* ver 1.2 : 30 Apr 2018
* - added category filter
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

Query<FileResourcePayload> CategoryFilter::apply(const Query<FileResourcePayload>& query)
{
    std::function<bool(FileResourcePayload)> thisCategory = [&](FileResourcePayload payload) {
        Categories categories = payload.getCategories();
        Categories::iterator found = std::find(categories.begin(), categories.end(), category_);
        return (found != categories.end());
    };

    return query.where.payload.has(thisCategory);
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
        DbElement<FileResourcePayload> elem = resultDb[key];
        FileResource res(elem.payLoad().getNamespace(), elem.metadata().name());
        ResourceVersion version = elem.payLoad().getVersion();

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
        res.setAuthor(props.getAuthorId());
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

//----< test stub >----------------------------------------------------

#ifdef TEST_REPOBROWSER

int main()
{
    return 0;
}

#endif // TEST_REPOBROWSER