#pragma once
/////////////////////////////////////////////////////////////////////////////////
// RepoPayload.h - Implements payload type for the Project#2 repository        //
// ver 1.1                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Application: NoSqlDb, CSE687 - Object Oriented Design                       //
// Author:      Ritesh Nair (rgnair@syr.edu)                                   //
/////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides:
* - RepoPayload class which provides Container for the reporsitories 
*   storage requirement.

* Required Files:
* ---------------
* - Nil - 
*
* Maintenance History:
* --------------------
* ver 1.1 : 16 Apr 2018
* - implements IPayload interface so that it is comptible for use with DbCore
* ver 1.0 : 08 Feb 2018
* - first release
*/

#ifndef REPO_PAYLOAD_H
#define REPO_PAYLOAD_H

#include "IPayload.h"

#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace Repository {

    /////////////////////////////////////////////////////////////////////
    // RepoPayload class
    // - Provides a NoSqlDb Payload that can hold the File Path and a 
    //   list of categories for the file which will be stored in the 
    //   database.

    class RepoPayload : public NoSqlDb::IPayload<RepoPayload> {
    public:
        using FilePath = std::string;
        using Category = std::string;
        using Categories = std::vector<Category>;

        FilePath& filePath() { return filePath_; }
        FilePath filePath() const { return filePath_; }
        void filePath(FilePath filePath) { filePath_ = filePath; }

        Categories& categories() { return categories_; }
        Categories categories() const { return categories_; }
        void categories(Categories categories) { categories_ = categories; }
        void addCategory(Category category) { categories_.push_back(category); }

        virtual std::string toString() override { return toString(); }

        virtual NoSqlDb::IPayload<RepoPayload>::Sptr toXmlElement() override;
        static RepoPayload fromXmlElement(NoSqlDb::IPayload<RepoPayload>::Sptr pPayloadElem);

        friend std::ostream& operator<<(std::ostream& os, const RepoPayload& payload);

    private:
        FilePath filePath_;
        Categories categories_;

        std::string toString() const;
        std::string stringifyCategories() const;
    };

    /////////////////////////////////////////////////////////////////////
    // RepoPayload methods

    //----< returns a comma separated list of values from the provided vector >---------------------

    template <typename T>
    std::string toCommaSeparatedString(const std::vector<T>& values)
    {
        int countOfValues = values.size();

        // no elements in the vector so nothing to do here
        if (countOfValues == 0)
            return "";

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

} // -- !ns::Repository

#endif // !REPO_PAYLOAD_H
