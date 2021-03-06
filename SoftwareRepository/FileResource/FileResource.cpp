///////////////////////////////////////////////////////////////////////
// FileResource.cpp - Implements the FileResource APIs               //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: SoftwareRepository, CSE687 - Object Oriented Design  //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
///////////////////////////////////////////////////////////////////////

/*
* Maintenance History:
* --------------------
* ver 1.0 : 24 Feb 2018
* - first release
*/

#include "FileResource.h"
#include "FileResourceTests.h"
#include "../RepoUtilities/RepoUtilities.h"

using namespace SoftwareRepository;

/////////////////////////////////////////////////////////////////////
// FileResource methods

//----< adds a category to the resource >---------------------
/*
*  - Adds a category for the resource
*  - This is an idempotent function
*    i.e if the provided category has already been added, calling this
*    function again with the same category will have no effect.
*/

FileResource& FileResource::setCategory(Category category)
{
    Categories::iterator found = std::find(categories_.begin(), categories_.end(), category);
    if (found == categories_.end())
        categories_.push_back(category);

    return *this;
}

//----< stringifies the file resource >---------------------

std::string FileResource::toString() const
{
    // referenced: http://www.stroustrup.com/C++11FAQ.html#raw-strings

    return R"(
        Resource =>
        --  Namespace        )" + ns_ + R"(
        --  Resoure Name     )" + resourceName_ + R"(
        --  Description      )" + description_ + R"(
        --  Package Name     )" + package_ + R"(
        --  Source Location  )" + sourceLocation_ + R"(
        --  Categories       )" + stringifyCategories(categories_) + R"(
        --  Dependencies     )" + stringifyDependencies(dependencies_) + "\n\n";
}

///////////////////////////////////////////////////////////////////////
// test functions

using namespace SoftwareRepositoryTests;
using namespace TestCore;

//----< test create file resource >------------------------------------------

bool TestCreateFileResource::operator()()
{
    Namespace ns = "testns";
    ResourceName name = "testfile.h";
    ResourceDescription description = "functionality explained";
    SourceLocation location = "c:/path/to/source/testfile.h";
    AuthorId authorId = "testns_owner";

    FileResource resource(ns, name);
    resource.setDescription(description);
    resource.setSourceLocation(location);

    if (resource.getNamespace() != ns)
    {
        setMessage("Namespace was not set");
        return false;
    }
    if (resource.getResourceName() != name)
    {
        setMessage("Resource name was not set");
        return false;
    }
    if (resource.getDescription() != description)
    {
        setMessage("Decsription was not set");
        return false;
    }
    if (resource.getSourceLocation() != location)
    {
        setMessage("Source Location was not set");
        return false;
    }

    std::cout << resource.toString();
    setMessage("Create FileResource");
    return true;
}

//----< test adding category and dependencies >------------------------------------------

bool TestAddCategoryAndDepToFileResource::operator()()
{
    Namespace ns = "testns";
    ResourceName name = "testfile2.h";
    ResourceDescription description = "functionality explained";
    SourceLocation location = "c:/path/to/source/testfile2.h";
    AuthorId authorId = "testns_owner";

    FileResource resource(ns, name);
    resource.setDescription(description);
    resource.setSourceLocation(location);

    std::cout << "\n  Created a file resource" << resource.toString();

    resource
        .setCategory("utility")
        .setCategory("static");

    if (resource.getCategories().size() != 2)
    {
        setMessage("Categories were not set");
        return false;
    }

    std::cout << "\n  After adding categories" << resource.toString();

    FileResource dependency = FileResource("testns", "testfile.h");
    resource.setDependency(dependency, 1);
    dependency = FileResource("testns2", "testfile3.h");
    resource.setDependency(dependency, 2);
    dependency = FileResource("testns2", "testfile4.h");
    resource.setDependency(dependency, 1);

    if (resource.getDependencies().size() != 3)
    {
        setMessage("Dependencies were not set");
        return false;
    }

    std::cout << "\n  After adding dependencies" << resource.toString();

    setMessage("Adding categories and dependencies");
    return true;
}

//----< test stub >----------------------------------------------------

#ifdef TEST_FILERESOURCE

int main()
{
    TestCreateFileResource testCreateFileresource("creating file resource");
    TestAddCategoryAndDepToFileResource testAddCatAndDep("adding categories and dependencies");

    TestSuite fileResource("Testing FileResource");
    fileResource.registerEx({ testCreateFileresource, testAddCatAndDep });
    fileResource.executeAll();

    return 0;
}

#endif // TEST_FILERESOURCE

