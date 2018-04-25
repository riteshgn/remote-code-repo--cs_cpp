////////////////////////////////////////////////////////////////////////////////
// CheckInView.xaml.cs - View which provides controls for Check-In            //
// ver 1.0                                                                    //
// Language:    C#, Visual Studio 2017                                        //
// Application: SoftwareRepository, CSE687 - Object Oriented Design           //
// Author:      Ritesh Nair (rgnair@syr.edu)                                  //
////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the View for check-in controls.
*
* Required Packages:
* ------------------
* RepoClientGUI.Services.RepoServerComm
* RepoClientGUI.ViewModels
*
* Maintenance History:
* --------------------
* ver 1.0 : 30 Mar 2018
* - first release
*/

using RepoClientGUI.Services.RepoServerComm;
using RepoClientGUI.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace RepoClientGUI.Views.Partials
{
    /// <summary>
    /// Interaction logic for CheckInView.xaml
    /// </summary>
    public partial class CheckInView : UserControl
    {
        private bool loaded_ = false;

        public CheckInView()
        {
            InitializeComponent();
        }

        private void BrowsePkgFolderBtn_Click(object sender, RoutedEventArgs e)
        {
            //RepoClientState state = (RepoClientState)this.DataContext;
            //string message = "Connecting to server " + 
            //    $"'{state.ServerConnProps.ServerAddress}:{state.ServerConnProps.ServerPort}' " +
            //    $"as user '{state.ServerConnProps.UserId}'";
            //MessageBox.Show(message);
        }

        private void DetectPkgNamespaceBtn_Click(object sender, RoutedEventArgs e)
        {
            //RepoClientState state = (RepoClientState)this.DataContext;
            //string message = "Connecting to server " + 
            //    $"'{state.ServerConnProps.ServerAddress}:{state.ServerConnProps.ServerPort}' " +
            //    $"as user '{state.ServerConnProps.UserId}'";
            //MessageBox.Show(message);
        }

        // ----< posts check-in request to the server on button click >--------------------
        private void CheckInBtn_Click(object sender, RoutedEventArgs e)
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            string message = $"Committing files from package folder '{state.CheckInProps.PackageFolder}'\n" + 
                $"Namespace: '{state.CheckInProps.Namespace}'\n" +
                $"Description: '{state.CheckInProps.PackageDescription}'";
            MessageBox.Show(message, "Check-In");

            state.ServerCommService.Requests.PostCheckIn("NoSqlDb",
                state.CheckInProps.Namespace,
                state.CheckInProps.PackageDescription,
                "Logger.h",
                state.ServerConnProps.UserId,
                (CheckInResponse response) => {}, true);
        }

        // ----< actions to be performed when the view is loaded >--------------------
        private void CheckInView_Loaded(object sender, RoutedEventArgs e)
        {
            if (!loaded_) // ensures that this block is executed only once
            {
                loaded_ = true;
                DemoCheckIn();
            }
        }

        // ----< demonstrates req #3b - check-in >--------------------
        private void DemoCheckIn()
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            state.CheckInProps.PackageFolder = "C:/User/NoSqlDb/DbCore";
            state.CheckInProps.Namespace = "NoSqlDb";
            state.CheckInProps.PackageDescription = "Provides core key-value db semantics";

            state.ServerCommService.Requests.PostCheckIn("NoSqlDb",
                state.CheckInProps.Namespace,
                state.CheckInProps.PackageDescription,
                "Logger.h",
                state.ServerConnProps.UserId,
                (CheckInResponse response) => {
                    Console.WriteLine($"\n\n{new String('-', 60)}");
                    Console.WriteLine("  Demonstrating Requirement 3b - Check-In");
                    Console.WriteLine($"{new String('-', 60)}\n");
                    Console.WriteLine($"    > Check-In requestId [{response.RequestId}] succeeded");
                    Console.WriteLine($"    > Server responded to request with status success: {response.Success}");
                    Console.WriteLine($"\n  Test Passed\n");
                }, true);
        }
    }
}
