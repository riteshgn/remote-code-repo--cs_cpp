////////////////////////////////////////////////////////////////////////////////
// CheckOutView.xaml.cs - View which provides controls for Check-Out          //
// ver 1.0                                                                    //
// Language:    C#, Visual Studio 2017                                        //
// Application: SoftwareRepository, CSE687 - Object Oriented Design           //
// Author:      Ritesh Nair (rgnair@syr.edu)                                  //
////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the View for check-out controls.
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
    /// Interaction logic for CheckOutView.xaml
    /// </summary>
    public partial class CheckOutView : UserControl
    {
        private bool loaded_ = false;
        public CheckOutView()
        {
            InitializeComponent();
        }

        // ----< fetches file list from the server on double click of package >--------------------
        private void PackageListBox_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            RepoClientState state = (RepoClientState) this.DataContext;

            RepoPackage repoPackage = (RepoPackage)PackageListBox.SelectedItem;

            if (repoPackage.RepoFiles == null || repoPackage.RepoFiles.Count() == 0)
            {
                state.ServerCommService.Requests.GetPackageFiles(repoPackage.PackageName,
                    state.ServerConnProps.UserId, (GetPackageFilesResponse res) =>
                    {
                        repoPackage.RepoFiles = res.RepoFiles;
                        state.CheckOutProps.RepoFiles = repoPackage.RepoFiles;
                    }, true);
            }
            else
            {
                state.CheckOutProps.RepoFiles = repoPackage.RepoFiles;
            }
            
        }

        // ----< updates the selection preview when checkbox is checked >--------------------
        private void RepoFile_Checked(object sender, RoutedEventArgs e)
        {
            RepoClientState state = (RepoClientState) this.DataContext;
            state.CheckOutProps.UpdateSelectionPreview();
        }

        // ----< posts checkout request to the server on button click >--------------------
        private void ConfirmAndDownloadBtn_Click(object sender, RoutedEventArgs e)
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            string message = $"Downloading files from package 'RepoCore'\n" +
                $"Namespace: 'SoftwareRepository'\n" +
                $"File: 'RepoCore.h v1'";
            MessageBox.Show(message, "Check-Out");

            state.ServerCommService.Requests.PostCheckOut("RepoCore", "SoftwareRepository",
                "RepoCore.h", 1, "rgnair", (CheckOutResponse response) => {}, true);
        }

        // ----< actions to be performed when the view is loaded >--------------------
        private void CheckOutView_Loaded(object sender, RoutedEventArgs e)
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            if (!loaded_ && state.ServerConnProps.Connected) // ensures that this block is executed only once
            {
                loaded_ = true;

                RefreshPackageList();
                //DemoCheckOut();
            }
        }

        // ----< requests the server for list of packages >--------------------
        private void RefreshPackageList()
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            state.ServerCommService.Requests.GetRepoPackages(state.ServerConnProps.UserId,
                (GetRepoPackagesResponse response) => state.CheckOutProps.RepoPackages.AddRange(response.RepoPackages));
        }

        // ----< demonstrates req #3c - checkout >--------------------
        private void DemoCheckOut()
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            state.CheckOutProps.CheckoutFolder = "C:/User/Workspaces/";

            state.ServerCommService.Requests.PostCheckOut("RepoCore", "SoftwareRepository",
                "RepoCore.h", 1, "rgnair", (CheckOutResponse response) =>
                {
                    Console.WriteLine($"\n\n{new String('-', 60)}");
                    Console.WriteLine("  Demonstrating Requirement #3c - Check-Out");
                    Console.WriteLine($"{new String('-', 60)}\n");
                    Console.WriteLine($"    > Check-Out requestId [{response.RequestId}] succeeded");
                    Console.WriteLine($"    > Server responded to request with status success: {response.Success}");
                    Console.WriteLine($"\n  Test Passed\n");
                }, true);

        }
    }
}
