#pragma once
/////////////////////////////////////////////////////////////////////////////////
// IPayload.h - Interface for NoSQlDb compatible payloads                      //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Application: NoSqlDb, CSE687 - Object Oriented Design                       //
// Author:      Ritesh Nair (rgnair@syr.edu)                                   //
// Source:      Jim Fawcett, Syracuse University, CST 4-187                    //
//              jfawcett@twcny.rr.com                                          //
/////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides:
* - IPayload which defines a contract that must be followed by any payload class
*   which would be compatible for use with the NoSqlDb.
* - The interface ensures that the payload provides means to persist/read from 
*   XML source
*
* Required Files:
* ---------------
* - Nil -
*
* Maintenance History:
* --------------------
* ver 1.0 : 16 Apr 2018
* - first release
*/

#ifndef IPAYLOAD_H
#define IPAYLOAD_H

#include "../XmlDocument/XmlElement/XmlElement.h"
#include <string>

namespace NoSqlDb
{
    /////////////////////////////////////////////////////////////////////
    // IPayload interface
    // - defines the contract which must be followed by any payload class

    template <typename T>
    class IPayload
    {
    public:
        using Sptr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;

        virtual std::string toString() = 0;
        virtual Sptr toXmlElement() = 0;
        static T fromXmlElement(Sptr);
    };
}

#endif // !IPAYLOAD_H
