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
* - added provision to mark displayed files
* - checkout selected files
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
        private CheckOutFilesPopup checkOutFilesPopup_;

        public BrowseView()
        {
            InitializeComponent();
        }

        // ----< searches packages for entered category >--------------------
        private void SearchCategoryBtn_Click(object sender, RoutedEventArgs e)
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            if (String.IsNullOrEmpty(state.BrowseProps.Category))
            {
                MessageBox.Show("Category is mandatory", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            RefreshPackageList();
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
        
        // ----< downloads required files on button click >--------------------
        private void DownloadBtn_ClickAction(CheckOutProps props)
        {
            checkOutFilesPopup_.Close();

            RepoClientState state = (RepoClientState)this.DataContext;
            foreach (RepoFile repoFile in state.BrowseProps.SelectionPreview)
            {
                Download(repoFile);

                if (state.CheckOutProps.DependentsAreRequired)
                {
                    ProcessDependentDownloads(repoFile);
                }
            }
        }

        // ----< posts checkout request to the server >--------------------
        private void Download(RepoFile repoFile)
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            state.ServerCommService.Requests.PostCheckOut(
                    repoFile.PackageName,
                    repoFile.Namespace,
                    repoFile.Filename,
                    repoFile.Version,
                    state.CheckOutProps.DependentsAreRequired,
                    state.ServerConnProps.UserId,
                    (CheckOutResponse response) =>
                    {
                        RepoFile.copyToDest(repoFile, state.CheckOutProps.CheckoutFolder);
                        string message = $"Succesfully downloaded file '{repoFile.Filename}' " +
                            $"to {state.CheckOutProps.CheckoutFolder}";
                        MessageBox.Show(message, "Check-Out");
                    },
                    true);
        }

        // ----< posts checkout request to the server on button click >--------------------
        private void DownloadDependents(RepoFileMetadata repoFileMeta)
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            foreach (RepoFile dep in repoFileMeta.Dependencies)
                Download(dep);
        }

        private void ProcessDependentDownloads(RepoFile repoFile)
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            string metadataKey = RepoFile.stringify(repoFile);
            if (state.BrowseProps.MetadataDict.ContainsKey(metadataKey))
            {
                DownloadDependents(state.BrowseProps.MetadataDict[metadataKey]);
            }
            else
            {
                state.ServerCommService.Requests.GetFileMetadata(repoFile.PackageName,
                    repoFile.Namespace, repoFile.Filename, repoFile.Version,
                    state.ServerConnProps.UserId,
                    (GetFileMetadataResponse metadataRes) =>
                    {
                        state.BrowseProps.MetadataDict[metadataKey] = metadataRes.Metadata;
                        this.Dispatcher.Invoke(() => DownloadDependents(state.BrowseProps.MetadataDict[metadataKey]));
                    }, true);
            }
        }


        // ----< updates the selection preview when checkbox is checked >--------------------
        private void RepoFile_Checked(object sender, RoutedEventArgs e)
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            state.BrowseProps.UpdateSelectionPreview();
        }

        // ----< pop-up the checkout window >--------------------
        private void CheckoutSelectionBtn_Click(object sender, RoutedEventArgs e)
        {
            checkOutFilesPopup_ = new CheckOutFilesPopup
            {
                DataContext = (RepoClientState)this.DataContext
            };
            checkOutFilesPopup_.Show();
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
                state.CheckOutProps.DownloadRequestCommand.Subscribe(DownloadBtn_ClickAction);

                state.BrowseProps.Category = "utility";
                DemoCheckOut();
                DemoGetPackageFiles();
                DemoGetFileText();
                DemoGetFileMetatada();
            }
            
            if (state.ServerConnProps.Connected)
                RefreshPackageList();
        }

        // ----< requests the server for list of packages >--------------------
        private void RefreshPackageList()
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            state.ServerCommService.Requests.GetRepoPackages(
                state.BrowseProps.Category, 
                state.ServerConnProps.UserId,
                (GetRepoPackagesResponse response) => state.BrowseProps.RepoPackages = response.RepoPackages,
                true);
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

        // ----< demonstrates req #3c - checkout >--------------------
        private void DemoCheckOut()
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            RepoFile repoFile = new RepoFile {
                PackageName = "Process",
                Namespace = "Process",
                Filename = "Process.h",
                Version = 1
            };
            state.ServerCommService.Requests.PostCheckOut(
                    repoFile.PackageName,
                    repoFile.Namespace,
                    repoFile.Filename,
                    repoFile.Version,
                    false,
                    state.ServerConnProps.UserId,
                    (CheckOutResponse response) =>
                    {
                        Console.WriteLine($"\n\n{new String('-', 60)}");
                        Console.WriteLine("  Demonstrating Requirement #3c - Check-Out");
                        Console.WriteLine($"{new String('-', 60)}\n");
                        Console.WriteLine($"    > Check-Out requestId [{response.RequestId}] succeeded");
                        Console.WriteLine($"    > Server responded to request with status success: {response.Success}");
                        Console.WriteLine($"\n  Test Passed\n");
                    },
                    true);
        }

        // ----< demonstrates req #3d - browse: fetching files >--------------------
        private void DemoGetPackageFiles()
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            state.ServerCommService.Requests.GetPackageFiles("FileSystemDemo",
                state.ServerConnProps.UserId, (GetPackageFilesResponse res) =>
                {
                    Console.WriteLine($"\n\n{new String('-', 60)}");
                    Console.WriteLine($"  Demonstrating Requirement #3d - Browse: Fetch Files");
                    Console.WriteLine($"{new String('-', 60)}\n");
                    Console.WriteLine($"    > Fetch for requestId [{res.RequestId}] succeeded");
                    Console.WriteLine($"    > Found {res.RepoFiles.Count()} files for package 'FileSystemDemo'");
                    res.RepoFiles.ForEach((RepoFile repoFile) => Console.WriteLine($"      > {repoFile.Filename} v{repoFile.Version}"));
                    Console.WriteLine($"\n  Test Passed\n");
                }, true);
        }

        // ----< demonstrates req #3e - view file text >--------------------
        private void DemoGetFileText()
        {
            RepoClientState state = (RepoClientState)this.DataContext;
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
        private void DemoGetFileMetatada()
        {
            RepoClientState state = (RepoClientState)this.DataContext;
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
