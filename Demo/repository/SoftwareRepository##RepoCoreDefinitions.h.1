#pragma once
////////////////////////////////////////////////////////////////////////
// RepoCoreDefinitions.h - Define aliases & constants used throughout //
//                         the SoftwareRepository namespace           //
// ver 1.1                                                            //
// Language:    C++, Visual Studio 2017                               //
// Application: SoftwareRepository, CSE687 - Object Oriented Design   //
// Author:      Ritesh Nair (rgnair@syr.edu)                          //
////////////////////////////////////////////////////////////////////////
/*
* Maintenance History:
* --------------------
* ver 1.1 : 30 Apr 2018
* - added list of state strings
* ver 1.0 : 23 Feb 2018
* - first release
*/

#ifndef REPO_CORE_DEF_H
#define REPO_CORE_DEF_H

#include <functional>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace SoftwareRepository
{
    enum RESOURCE_MODIFY_ACTION { ALL };
    enum RESOURCE_STATE { OPEN, CLOSED, CLOSE_PENDING };
    enum FAILURE_CODE {
        NO_AUTHOR_INFO,
        FORBIDDEN,
        OPEN_VERSION,
        CLOSED_VERSION,
        OPEN_DEPENDENCIES,
        OPEN_DEPENDENCIES_PENDING_CLOSE,
        RESOURCE_NOT_FOUND,
        STORE_SAVE_FAILED,
        STORE_LOAD_FAILED,
        STORE_LOAD_DEP_FAILED
    };

    // ALIASES
    using Namespace = std::string;
    using PackageName = std::string;
    using AuthorId = std::string;
    using ResourceIdentity = std::string;
    using ResourceIdentities = std::vector<std::string>;
    using ResourcePropsDbKey = std::string;
    using ResourcePropsDbKeys = std::vector<ResourcePropsDbKey>;
    using ResourcePropsDbSize = size_t;
    using ResourceName = std::string;
    using ResourceDescription = std::string;
    using ResourceVersion = size_t;
    using ResourceVersions = std::vector<ResourceVersion>;
    using ResourceIdentityWithVersion = std::pair<ResourceIdentity, ResourceVersion>;
    using ResourceIdentitiesWithVersion = std::vector<ResourceIdentityWithVersion>;
    using Dependencies = ResourceIdentitiesWithVersion;
    using Category = std::string;
    using Categories = std::vector<Category>;
    using State = RESOURCE_STATE;

    using Action = RESOURCE_MODIFY_ACTION;

    using SourceLocation = std::string;
    using CheckoutLocation = std::string;
    using FileStoreRoot = std::string;

    template <typename T>
    using PropsDbQueryResults = std::vector<std::reference_wrapper<T>>;
    template <typename T>
    using BrowseFilters = std::vector<std::reference_wrapper<T>>;
    template <typename T>
    using BrowseResultProcessors = std::vector<std::reference_wrapper<T>>;
    using Level = size_t;

    using FailureCode = size_t;
    using FailureReason = std::string;
    using FailureReasons = std::unordered_map<FailureCode, FailureReason>;

    using PackageLocation = std::string;

    // FLAGS
    const bool DEFAULT_AUTO_COMMIT = false;
    const bool DEFAULT_FETCH_DEPENDENCIES = false;
    const bool DEFAULT_CREATE_IF_NOT_EXISTS = true;
    const bool DEFAULT_INCLUDE_CONSOLE_PROCESSOR = false;

    // CONSTANTS
    const Action DEFAULT_RESOURCE_MODIFY_ACTION = Action::ALL;
    const char PREFIX_DBKEY_VERSION = '#';
    const std::string NAMESPACE_FILENAME_SEPARATOR = "##";
    const FileStoreRoot DEFAULT_FILE_STORE_ROOT_FOLDER = "../Demo/repository";
    const std::vector<std::string> DEFAULT_SUPPORTED_FILETYPES = { "*.h", "*.cpp" };
    const std::vector<std::string> STATE_STRINGS = { "Open", "Closed", "Closed (Pending)" };
}

#endif // !REPO_CORE_DEF_H