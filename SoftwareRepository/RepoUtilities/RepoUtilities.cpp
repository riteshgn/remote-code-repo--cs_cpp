///////////////////////////////////////////////////////////////////////////////
// RepoUtilities.h - Implements utility functions which are used by various  //
//                   Repository components                                   //
// ver 1.2                                                                   //
// Language:    C++, Visual Studio 2017                                      //
// Application: SoftwareRepository, CSE687 - Object Oriented Design          //
// Author:      Ritesh Nair (rgnair@syr.edu)                                 //
///////////////////////////////////////////////////////////////////////////////

/*
* Maintenance History:
* --------------------
* ver 1.2 : 23 Apr 2018
* - added utility to compute the db id based on resource identity and version
* ver 1.1 : 10 Mar 2018
* - added utility to get failure reason given code
* ver 1.0 : 28 Feb 2018
* - first release
*/

#include "RepoUtilities.h"

using namespace SoftwareRepository;

//----< stringifies the categories >---------------------

std::string SoftwareRepository::stringifyCategories(Categories categories)
{
    return toCommaSeparatedString<Category>(categories);
}

//----< stringifies the dependencies >---------------------

std::string SoftwareRepository::stringifyDependencies(Dependencies dependencies)
{
    if (dependencies.empty())
        return "[]";

    std::string output = "[ ";
    for (std::pair<ResourceIdentity, ResourceVersion> element : dependencies)
    {
        output = output + "(" + element.first + ", v" + std::to_string(element.second) + ") ";
    }
    output = output + "]";

    return output;
}

//----< stringifies the resource state >---------------------

std::string SoftwareRepository::stringifyResourceState(State state)
{
    switch (state)
    {
    case OPEN: return "Open";
    case CLOSED: return "Closed";
    case CLOSE_PENDING: return "Closed (Pending)";
    default: return "Unknown";
    }
}

//----< reutrns the failure reason for given code >----------

FailureReason SoftwareRepository::getFailureReason(FailureCode code)
{
    switch (code)
    {
    case NO_AUTHOR_INFO: return "Author Info should be provided";
    case FORBIDDEN: return "Author does not have permission to perform this operation";
    case OPEN_VERSION: return "Version is in open state";
    case CLOSED_VERSION: return "Version is closed and cannot be modified";
    case OPEN_DEPENDENCIES: return "Some of the dependencies are open";
    case OPEN_DEPENDENCIES_PENDING_CLOSE: return
        "Some of the dependencies are open so resource state is set to 'Pending Close'";
    case RESOURCE_NOT_FOUND: return "The specified resource is not found in the repository";
    case STORE_SAVE_FAILED: return "Error when trying to save file to repository store";
    case STORE_LOAD_FAILED: return "Error when trying to load file from repository store";
    case STORE_LOAD_DEP_FAILED: return "Error when trying to load file dependencies from repository store";
    default: return "Unknown Failure";
    }
}

//----< returns a unique identifier for a resource based on the provided version number >--------------------

ResourcePropsDbKey SoftwareRepository::getDbKeyForVersion(ResourceIdentity resourceId, ResourceVersion version)
{
    return resourceId + PREFIX_DBKEY_VERSION + std::to_string(version);
}

#ifdef TEST_REPOUTILITIES

int main()
{
    return 0;
}

#endif // TEST_REPOUTILITIES