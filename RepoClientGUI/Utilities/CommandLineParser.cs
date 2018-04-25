/////////////////////////////////////////////////////////////////////////
// CommandLineParser.cs - Limited Command Line Parsing Utility         //
// ver 1.0                                                             //
// Language:    C#, Visual Studio 2017                                 //
// Application: SoftwareRepository, CSE687 - Object Oriented Design    //
// Author:      Ritesh Nair (rgnair@syr.edu)                           //
/////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the MainWindow for the Repository Client GUI. 
* It renders the following 
* - A Tab Control which provides navigation to below features provided
*   by the repository
*   - connect
*   - check-in
*   - check-out
*   - browse
*   Each tab is rendered by a User Control defined under Views/Partials
* - A Status bar which displays relevant information based on the user's 
*   actions
*
* Required Packages:
* ------------------
* RepoClientGUI.Services.RepoServerComm
* RepoClientGUI.ViewModels
*
* Maintenance History:
* --------------------
* ver 1.0 : 12 Apr 2018
* - first release
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RepoClientGUI.Utilities
{
    public class CommandLineParser
    {
        // ----< parses the command line args into a dictionary of key/value >--------------------
        public static Dictionary<string, string> parse(string[] args)
        {
            Dictionary<string, string> parsedArgs = new Dictionary<string, string>();
            foreach(string arg in args)
            {
                if (arg.Contains('='))
                {
                    string[] parts = arg.Split('=');
                    parsedArgs.Add(parts[0], parts[1]);
                }
                else
                {
                    parsedArgs.Add(arg, "");
                }
            }
            return parsedArgs;
        }
    }
}
