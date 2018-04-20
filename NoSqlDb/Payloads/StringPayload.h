#pragma once
/////////////////////////////////////////////////////////////////////////////////
// StringPayload.h - Implements payload type for string-only payloads          //
// ver 1.0                                                                     //
// Language:    C++, Visual Studio 2017                                        //
// Application: NoSqlDb, CSE687 - Object Oriented Design                       //
// Author:      Ritesh Nair (rgnair@syr.edu)                                   //
/////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides:
* - StringPayload class which provides Container for a string.

* Required Files:
* ---------------
* - Nil -
*
* Maintenance History:
* --------------------
* ver 1.0 : 16 Apr 2018
* - first release
*/

#ifndef STRINGPAYLOAD_H
#define STRINGPAYLOAD_H

#include "IPayload.h"

namespace NoSqlDb
{
    /////////////////////////////////////////////////////////////////////
    // StringPayload class
    // - Provides a NoSqlDb Payload that can hold a std::string object

    class StringPayload : public IPayload<StringPayload>
    {
    public:
        StringPayload() {}
        StringPayload(const std::string& value) { value_ = value; }

        std::string& value() { return value_; }
        std::string value() const { return value_; }
        void value(const std::string& value) { value_ = value; }

        virtual std::string toString() override { return value_; }

        virtual IPayload<StringPayload>::Sptr toXmlElement() override;
        static StringPayload fromXmlElement(IPayload<StringPayload>::Sptr pPayloadElem);

        friend std::ostream& operator<<(std::ostream& os, const StringPayload& payload)
        {
            return os << payload.value();
        };

    private:
        std::string value_;
    };
}

#endif // STRINGPAYLOAD_H
