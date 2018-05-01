////////////////////////////////////////////////////////////////////////////////
// BrowseView.xaml.cs - View which provides controls for Repository Browsing  //
// ver 1.1                                                                    //
// Language:    C#, Visual Studio 2017                                        //
// Application: SoftwareRepository, CSE687 - Object Oriented Design           //
// Author:      Ritesh Nair (rgnair@syr.edu)                                  //
////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the View for browsing controls. 
* It renders the following 
* - A List Box which render all packages.
* - Another List Box which renders all files in a package on user's request.
*
* Required Packages:
* ------------------
* RepoClientGUI.Services.RepoServerComm
* RepoClientGUI.ViewModels
*
* Maintenance History:
* --------------------
* ver 1.1 : 30 Apr 2018
* - fixed onload refresh packages issue
* - packages are searched by provided category
* ver 1.0 : 10 Apr 2018
* - first release
*/

using RepoClientGUI.Services.RepoServerComm;
using RepoClientGUI.ViewModels;
using RepoClientGUI.Views.Popups;
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
    /// Interaction logic for BrowseView.xaml
    /// </summary>
    public partial class BrowseView : UserControl
    {
        private bool loaded_ = false;

        public BrowseView()
        {
            InitializeComponent();
        }

        // ----< fetches file list from the server on double click of package >--------------------
        private void PackageListBox_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            RepoPackage repoPackage = (RepoPackage)PackageListBox.SelectedItem;

            if (repoPackage.RepoFiles == null || repoPackage.RepoFiles.Count() == 0)
            {
                state.ServerCommService.Requests.GetPackageFiles(repoPackage.PackageName,
                    state.ServerConnProps.UserId, (GetPackageFilesResponse res) =>
                    {
                        repoPackage.RepoFiles = res.RepoFiles;
                        state.BrowseProps.RepoFiles = repoPackage.RepoFiles;
                    }, true);
            }
            else
            {
                state.BrowseProps.RepoFiles = repoPackage.RepoFiles;
            }
        }

        // ----< fetches file's text from server and displays it >--------------------
        private void FileTextLink_ClickAction(RepoFile repoFile)
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            state.BrowseProps.SelectedRepoFile = repoFile;

            FileTextPopupWindow fileTextPopup = new FileTextPopupWindow();
            fileTextPopup.DataContext = state;
            fileTextPopup.Title = $"{repoFile.Filename} v{repoFile.Version}";
            fileTextPopup.Show();
        }

        // ----< fetches file's metadata from server and displays it >--------------------
        private void MetadataLink_ClickAction(RepoFile repoFile)
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            string metadataKey = RepoFile.stringify(repoFile);
            if (state.BrowseProps.MetadataDict.ContainsKey(metadataKey))
            {
                DisplayMetadata(repoFile, state.BrowseProps.MetadataDict[metadataKey]);
            }
            else
            {
                state.ServerCommService.Requests.GetFileMetadata(repoFile.PackageName,
                    repoFile.Namespace, repoFile.Filename, repoFile.Version,
                    state.ServerConnProps.UserId,
                    (GetFileMetadataResponse metadataRes) => {
                        state.BrowseProps.MetadataDict[metadataKey] = metadataRes.Metadata;
                        this.Dispatcher.Invoke(() => DisplayMetadata(repoFile, state.BrowseProps.MetadataDict[metadataKey]));
                    }, true);
            }
        }

        private void SearchCategoryBtn_Click(object sender, RoutedEventArgs e)
        {
            RefreshPackageList();
        }

        // ----< actions to be performed when the view is loaded >--------------------
        private void BrowseView_Loaded(object sender, RoutedEventArgs e)
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            if (!loaded_) // ensures that this block is executed only once
            {
                loaded_ = true;

                // subscribe to the show metadata and show file text commands and register 
                // corresponding actions to be performed
                state.BrowseProps.ShowMetadataCommand.Subscribe(MetadataLink_ClickAction);
                state.BrowseProps.ShowFileTextCommand.Subscribe(FileTextLink_ClickAction);

                state.BrowseProps.Category = "utility";
                //DemoBrowse();
            }
            
            if (state.ServerConnProps.Connected)
                RefreshPackageList();
        }

        // ----< requests the server for list of packages >--------------------
        private void RefreshPackageList()
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            state.ServerCommService.Requests.GetRepoPackages(state.BrowseProps.Category, state.ServerConnProps.UserId,
                (GetRepoPackagesResponse response) => state.BrowseProps.RepoPackages = response.RepoPackages);
        }

        // ----< displays a message box with the metadata information of selected file >--------------------
        private void DisplayMetadata(RepoFile repoFile, RepoFileMetadata metadata)
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            state.BrowseProps.SelectedRepoFile = repoFile;
            IEnumerable<String> depList = metadata.Dependencies.Select((RepoFile depFile) => $"> {depFile.Filename} v{depFile.Version}");

            FileMetadataPopupWindow fileMetadataPopup = new FileMetadataPopupWindow();
            fileMetadataPopup.Title = $"Metadata for {repoFile.Filename} v{repoFile.Version}";
            fileMetadataPopup.Author.Text = metadata.Author;
            fileMetadataPopup.Description.Text = metadata.Description;
            fileMetadataPopup.Dependencies.Text = String.Join("\n", depList);
            fileMetadataPopup.Show();
        }

        // ----< demonstrates req #3d - browse: fetching packages >--------------------
        private void DemoBrowse()
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            state.ServerCommService.Requests.GetRepoPackages("utility", state.ServerConnProps.UserId,
                (GetRepoPackagesResponse response) =>
                {
                    Console.WriteLine($"\n\n{new String('-', 60)}");
                    Console.WriteLine($"  Demonstrating Requirement #3d - Browse: Fetch Packages");
                    Console.WriteLine($"{new String('-', 60)}\n");
                    Console.WriteLine($"    > Fetch for requestId [{response.RequestId}] succeeded");
                    Console.WriteLine($"    > Found {response.RepoPackages.Count()} packages");
                    response.RepoPackages.ForEach((RepoPackage pkg) => Console.WriteLine($"      > {pkg.PackageName}"));
                    Console.WriteLine($"\n  Test Passed\n");
                    state.BrowseProps.RepoPackages.AddRange(response.RepoPackages);

                    foreach (RepoPackage pkg in state.BrowseProps.RepoPackages)
                    {
                        DemoGetPackageFiles(pkg, state);
                    }

                    DemoGetFileText(state);
                    DemoGetFileMetatada(state);
                }, true);
        }

        // ----< demonstrates req #3d - browse: fetching files >--------------------
        private void DemoGetPackageFiles(RepoPackage pkg, RepoClientState state)
        {
            state.ServerCommService.Requests.GetPackageFiles(pkg.PackageName,
                state.ServerConnProps.UserId, (GetPackageFilesResponse res) =>
                {
                    Console.WriteLine($"\n\n{new String('-', 60)}");
                    Console.WriteLine($"  Demonstrating Requirement #3d - Browse: Fetch Files");
                    Console.WriteLine($"{new String('-', 60)}\n");
                    Console.WriteLine($"    > Fetch for requestId [{res.RequestId}] succeeded");
                    Console.WriteLine($"    > Found {res.RepoFiles.Count()} files for package '{pkg.PackageName}'");
                    res.RepoFiles.ForEach((RepoFile repoFile) => Console.WriteLine($"      > {repoFile.Filename} v{repoFile.Version}"));
                    Console.WriteLine($"\n  Test Passed\n");
                    pkg.RepoFiles = res.RepoFiles;
                }, true);
        }

        // ----< demonstrates req #3e - view file text >--------------------
        private void DemoGetFileText(RepoClientState state)
        {
            state.ServerCommService.Requests.GetFileText("RepoCore", "SoftwareRepository",
                "RepoCore.h", 1, state.ServerConnProps.UserId,
                (GetFileTextResponse fileTextRes) => {
                    Console.WriteLine($"\n\n{new String('-', 60)}");
                    Console.WriteLine($"  Demonstrating Requirement #3e - Viewing full file text");
                    Console.WriteLine($"{new String('-', 60)}\n");
                    Console.WriteLine($"    > Fetch for requestId [{fileTextRes.RequestId}] succeeded");
                    Console.WriteLine($"    > Below are few lines of text from file RepoCore.h v1\n");
                    Console.WriteLine($"    {new String('-', 15)}x{new String('-', 15)}");
                    Console.WriteLine($"\n{RepoFile.readFileText("RepoCore.h").Substring(0, 1025)}\n");
                    Console.WriteLine($"    {new String('-', 15)}x{new String('-', 15)}");
                    Console.WriteLine($"\n  Test Passed\n");
                }, true);
        }

        // ----< demonstrates req #3f - view file metadata >--------------------
        private void DemoGetFileMetatada(RepoClientState state)
        {
            state.ServerCommService.Requests.GetFileMetadata("RepoCore", "SoftwareRepository",
                "RepoCore.h", 1, state.ServerConnProps.UserId,
                (GetFileMetadataResponse metadataRes) => {
                    Console.WriteLine($"\n\n{new String('-', 60)}");
                    Console.WriteLine("  Demonstrating Requirement #3f - Viewing file metadata");
                    Console.WriteLine($"{new String('-', 60)}\n");
                    Console.WriteLine($"    > Fetch for requestId [{metadataRes.RequestId}] succeeded");
                    Console.WriteLine($"    > Found below metadata for file RepoCore.h v1");
                    Console.WriteLine($"      > Author: {metadataRes.Metadata.Author}");
                    Console.WriteLine($"      > Description: {metadataRes.Metadata.Description}");
                    Console.WriteLine($"      > Dependencies:");
                    metadataRes.Metadata.Dependencies.ForEach((RepoFile repoFile) => Console.WriteLine($"        > {repoFile.Filename} v{repoFile.Version}"));
                    Console.WriteLine($"\n  Test Passed\n");
                }, true);
        }
    }
}
