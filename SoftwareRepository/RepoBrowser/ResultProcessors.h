#pragma once
//////////////////////////////////////////////////////////////////////////////////////
// ResultProcessors.h - Implements various result processors for the RepoBrowser    //
// ver 1.0                                                                          //
// Language:    C++, Visual Studio 2017                                             //
// Application: SoftwareRepository, CSE687 - Object Oriented Design                 //
// Author:      Ritesh Nair (rgnair@syr.edu)                                        //
//////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the various instances of IBrowserResultProcessor interface to
* help process results fetched by queryies the resource properties Db.
* It contains below classes:
* - ConsoleResultProcessor      : prints each navigation step on the standard console output
*
* Required Files:
* ---------------
* IRepoBrowser.h
* FileResource.h, FileResource.cpp
*
* Maintenance History:
* --------------------
* ver 1.0 : 20 Apr 2018
* - first release
*/

#ifndef RESULTPROCESSORS_H
#define RESULTPROCESSORS_H

#include "IRepoBrowser.h"
#include "../FileResource/FileResource.h"

namespace SoftwareRepository
{

    /////////////////////////////////////////////////////////////////////
    // ConsoleResultProcessor
    // - prints each navigation step on the standard console output

    class ConsoleResultProcessor : public IBrowserResultProcessor<FileResource>
    {
    public:
        virtual bool operator()(FileResource, ResourceVersion, Level) override;
    };

}

#endif // !RESULTPROCESSORS_H
