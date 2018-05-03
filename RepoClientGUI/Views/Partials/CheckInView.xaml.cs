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
* ver 1.1 : 28 Apr 2018
* - added a browse folder dialog box
* - package name and namespace detected based on selected folder name
* - introduced a category text box
* - passing arguments based on changes for check-in request
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

        // reference: https://msdn.microsoft.com/en-us/library/system.windows.forms.folderbrowserdialog.aspx
        private System.Windows.Forms.FolderBrowserDialog browsePkgFolderDialog;

        public CheckInView()
        {
            InitializeComponent();
            SetupBrowsePkgDialog();
        }

        // ----< pops-up a folder select dialog box to select the package folder >--------------------
        private void BrowsePkgFolderBtn_Click(object sender, RoutedEventArgs e)
        {
            // Show the FolderBrowserDialog.
            System.Windows.Forms.DialogResult result = browsePkgFolderDialog.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK)
            {
                RepoClientState state = (RepoClientState)this.DataContext;
                state.CheckInProps.PackageFolder = browsePkgFolderDialog.SelectedPath;
            }
        }

        // ----< posts check-in request to the server on button click >--------------------
        private void CheckInBtn_Click(object sender, RoutedEventArgs e)
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            state.ServerCommService.Requests.PostCheckIn(
                state.CheckInProps.PackageFolder,
                state.CheckInProps.PackageName,
                state.CheckInProps.Namespace,
                state.CheckInProps.PackageDescription,
                state.CheckInProps.Category,
                state.ServerConnProps.UserId,
                (CheckInResponse response) => {
                    string message = $"Succesfully uploaded file '{response.File}'";
                    MessageBox.Show(message, "Check-In");
                }, true);
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

        // ----< setup the browse folder dialog >--------------------
        private void SetupBrowsePkgDialog()
        {
            this.browsePkgFolderDialog = new System.Windows.Forms.FolderBrowserDialog {
                // Set the help text description for the FolderBrowserDialog.
                Description = "Select the package directory that you want to check-in to the repository",
                // Do not allow the user to create new files via the FolderBrowserDialog.
                ShowNewFolderButton = false,
                // Default to the My Documents folder.
                RootFolder = Environment.SpecialFolder.MyComputer
            };
        }

        // ----< demonstrates req #3b - check-in >--------------------
        private void DemoCheckIn()
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            state.CheckInProps.PackageFolder = "../Demo/Logger";
            state.CheckInProps.PackageName = "Logger";
            state.CheckInProps.Namespace = "Logger";
            state.CheckInProps.PackageDescription = "Logger for C++ programs";
            state.CheckInProps.Category = "logger";

            state.ServerCommService.Requests.PostCheckIn(
                state.CheckInProps.PackageFolder,
                state.CheckInProps.PackageName,
                state.CheckInProps.Namespace,
                state.CheckInProps.PackageDescription,
                state.CheckInProps.Category,
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
