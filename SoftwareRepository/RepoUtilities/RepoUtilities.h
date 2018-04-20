#pragma once
///////////////////////////////////////////////////////////////////////////////
// RepoUtilities.h - Implements utility functions which are used by various  //
//                   Repository components                                   //
// ver 1.1                                                                   //
// Language:    C++, Visual Studio 2017                                      //
// Application: SoftwareRepository, CSE687 - Object Oriented Design          //
// Author:      Ritesh Nair (rgnair@syr.edu)                                 //
///////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements below utility functions
* - std::string toCommaSeparatedString(const std::vector<T>& values)
* - std::string stringifyCategories(Categories);
* - std::string stringifyDependencies(Dependencies)
* - std::string stringifyResourceState(State);
* - FailureReason getFailureReason(FailureCode);
*
* Required Files:
* ---------------
* RepoCoreDefinitions.h
*
* Maintenance History:
* --------------------
* ver 1.1 : 10 Mar 2018
* - added utility to get failure reason given code
* ver 1.0 : 28 Feb 2018
* - first release
*/

#ifndef REPO_UTILITIES_H
#define REPO_UTILITIES_H

#include "../RepoCore/RepoCoreDefinitions.h"

namespace SoftwareRepository
{
    // UTILITY FUNCTIONS

    //----< returns a comma separated list of values from the provided vector >---------------------

    template <typename T>
    std::string toCommaSeparatedString(const std::vector<T>& values)
    {
        int countOfValues = values.size();

        // no elements in the vector so nothing to do here
        if (countOfValues == 0)
            return "[]";

        // referenced from: https://stackoverflow.com/a/6693088
        // - If there are more than one elements then we need to return a comma separated string of them.
        // - This is acheived by
        //   1. copying all but the last element of the vector to a string output stream (std::ostringstream) 
        //      and appending a ', ' between each element.
        //   2. appending the last element to the output stream without ', ' to the end of the output stream.
        // - Incase there is only one element in the vector, we simply append that element to the output stream.

        std::ostringstream output;

        if (countOfValues > 1)
        {
            std::copy(values.begin(), values.end() - 1, std::ostream_iterator<T>(output, ", "));
        }

        // append the last element to the output stream
        output << values.back();

        return output.str();

    }

    //----< stringifies the categories >---------------------

    std::string stringifyCategories(Categories);

    //----< stringifies the dependencies >---------------------

    std::string stringifyDependencies(Dependencies);
    
    //----< stringifies the resource state >---------------------

    std::string stringifyResourceState(State);

    //----< reutrns the failure reason for given code >----------

    FailureReason getFailureReason(FailureCode);
}

#endif // !REPO_UTILITIES_H