#pragma once
///////////////////////////////////////////////////////////////////////
// ConsoleColor.h - Provides helper functions to change the color    //
//                  of windows console output                        //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: NoSqlDb, CSE687 - Object Oriented Design             //
// Author:      Ritesh Nair (rgnair@syr.edu)                         //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides functions:
* - setConsoleColor(Color)           change console output color
*
* Required Files:
* ---------------
* - Nil - 
*
* Maintenance History:
* --------------------
* ver 1.0 : 10 Feb 2018
* - first release
*/

#include <Windows.h>

namespace Console {

    /////////////////////////////////////////////////////////////////////
    // Color enumeration
    // - defines the colors which can be painted on the console.

    enum Color {
        COLOR_WHITE = 7,
        COLOR_GREY = 8,
        COLOR_GREEN = 10,
        COLOR_RED = 12,
        COLOR_PURPLE = 13
    };

    //----< sets the windows console color to the specified color >---------------------

    void setConsoleColor(Color color)
    {
        // source: https://www.daniweb.com/programming/software-development/code/216345/add-a-little-color-to-your-console-text
        HANDLE  hConsole;
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }

}