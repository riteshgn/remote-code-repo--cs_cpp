/////////////////////////////////////////////////////////////////////////
// App.xaml.cs - Main Application for the Repository Client GUI        //
// ver 1.0                                                             //
// Language:    C#, Visual Studio 2017                                 //
// Application: SoftwareRepository, CSE687 - Object Oriented Design    //
// Author:      Ritesh Nair (rgnair@syr.edu)                           //
/////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the WPF application for the GUI.
* It loads the MainWindow with desired data context.
*
* Required Packages:
* ------------------
* RepoClientGUI.Services.RepoServerComm
* RepoClientGUI.ViewModels
*
* Maintenance History:
* --------------------
* ver 1.0 : 28 Mar 2018
* - first release
*/

using RepoClientGUI.Services.RepoServerComm;
using RepoClientGUI.Utilities;
using RepoClientGUI.ViewModels;
using RepoClientGUI.Views;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace RepoClientGUI
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        // ----< actions to be performed when the wpf app starts up >--------------------
        private void Application_Startup(object sender, StartupEventArgs e)
        {
            Dictionary<string, string> parsedArgs = CommandLineParser.parse(e.Args);
            int port = 7890;
            if (parsedArgs.ContainsKey("port"))
                port = Int32.Parse(parsedArgs["port"]);

            RepoServerCommService service = RepoServerCommService.GetInstance("localhost", port);
            RepoClientState state = new RepoClientState(service);

            MainWindow window = new MainWindow(state);
            window.Show();
        }
    }
}
