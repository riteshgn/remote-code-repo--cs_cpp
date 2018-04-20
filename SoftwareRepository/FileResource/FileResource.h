#pragma once
///////////////////////////////////////////////////////////////////////
// FileResource.h - Implements file-based repository resource        //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: SoftwareRepository, CSE687 - Object Oriented Design  //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides the container for a file-based resource. It contains:
* - FileResource class which encapsulates the basic identity of a file like
*   its name and source location along with details like the category it belongs to
*   and any other file dependencies it has.
*
* Required Files:
* ---------------
* RepoCoreDefinitions.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 24 Feb 2018
* - first release
*/

#include "../RepoCore/RepoCoreDefinitions.h"

#include <string>

namespace SoftwareRepository
{

    /////////////////////////////////////////////////////////////////////
    // FileResource class
    // - provides a constructor which accepts all required fields for
    //   a file-based source including
    //   - namespace
    //   - resource name
    //   - desccription
    //   - source location
    //   - owner's ID
    // - provides APIs to store categories and dependencies for the resource
    //   - categories are a std::vector<std::string>
    //   - dependecies are a nested unordered_map having {namespace: {resource: version}}
    // - provides API to compute the resource identity's prefix

    class FileResource
    {
    public:
        FileResource(Namespace ns, ResourceName resourceName) :
            ns_(ns), resourceName_(resourceName) {}

        // methods to access private members
        
        Categories& getCategories() { return categories_; }
        Categories getCategories() const { return categories_; }

        Dependencies& getDependencies() { return dependencies_; }
        Dependencies getDependencies() const { return dependencies_; }

        ResourceDescription& getDescription() { return description_; }
        ResourceDescription getDescription() const { return description_; }

        Namespace& getNamespace() { return ns_; }
        Namespace getNamespace() const { return ns_; }

        PackageName& getPackageName() { return package_; }
        PackageName getPackageName() const { return package_; }

        ResourceName& getResourceName() { return resourceName_; }
        ResourceName getResourceName() const { return resourceName_; }

        SourceLocation& getSourceLocation() { return sourceLocation_; }
        SourceLocation getSourceLocation() const { return sourceLocation_; }

        // methods to set private member values

        FileResource& setCategory(Category);
        FileResource& setDependency(FileResource& res, ResourceVersion resourceVersion) {
            dependencies_.push_back(ResourceIdentityWithVersion(res.getIdentity(), resourceVersion));
            return *this;
        };
        FileResource& setDescription(ResourceDescription description) { 
            description_ = description; 
            return *this; 
        };
        FileResource& setPackageName(PackageName package) {
            package_ = package;
            return *this;
        };
        FileResource& setSourceLocation(SourceLocation sourceLocation) { 
            sourceLocation_ = sourceLocation; 
            return *this; 
        };

        // method which will return an identity for the resource lookup in the repo

        //----< returns a unique identity of this file resource >---------------------
        /*
        *  - The file resource defines its identity as <namespace>::<resource_name>
        *  - Repository components need to uniquely identify this resource.
        *    However, there would be multiple version of this resource in the repo.
        *    So the uniqueness must be specified as <resourceIdentity>_<version>
        *  - Individual components will resolve the required version. 
        */

        ResourceIdentity getIdentity() { return ns_ + NAMESPACE_FILENAME_SEPARATOR + resourceName_;  }

        // stringify to view in human readable form

        std::string toString() const;

    private:
        Namespace ns_;
        ResourceName resourceName_;
        ResourceDescription description_;
        PackageName package_;
        SourceLocation sourceLocation_;
        Dependencies dependencies_;
        Categories categories_;
    };
}