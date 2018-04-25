////////////////////////////////////////////////////////////////////////////////
// FileTextPopupWindow.xaml.cs - Window which displays a File's contents      //
// ver 1.0                                                                    //
// Language:    C#, Visual Studio 2017                                        //
// Application: SoftwareRepository, CSE687 - Object Oriented Design           //
// Author:      Ritesh Nair (rgnair@syr.edu)                                  //
////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the window displaying a selected File's contents.
*
* Required Packages:
* ------------------
* RepoClientGUI.Services.RepoServerComm
* RepoClientGUI.ViewModels
* RepoClientGUI.Controls
*
* Maintenance History:
* --------------------
* ver 1.0 : 11 Apr 2018
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
using System.Windows.Shapes;

namespace RepoClientGUI.Views.Partials
{
    /// <summary>
    /// Interaction logic for FileTextPopupWindow.xaml
    /// </summary>
    public partial class FileTextPopupWindow : Window
    {
        public FileTextPopupWindow()
        {
            InitializeComponent();
        }

        // ----< actions to be performed when the window is loaded >--------------------
        private void FileTextPopup_Loaded(object sender, RoutedEventArgs e)
        {
            // clears any existing paragraphs from the embedded
            // file text viewer user control
            this.fileTextViewer.fileText.Blocks.Clear();

            RepoClientState state = (RepoClientState)this.DataContext;
            RepoFile repoFile = state.BrowseProps.SelectedRepoFile;
            
            // query the repository for the file's text
            state.ServerCommService.Requests.GetFileText(repoFile.PackageName,
                    repoFile.Namespace, repoFile.Filename, repoFile.Version,
                    state.ServerConnProps.UserId,
                    (GetFileTextResponse fileTextRes) => {
                        // invoking through dispatcher because UI elements cannot 
                        // be set from external thread
                        this.Dispatcher.Invoke(() => {
                            // feeds the received file's contents to the file text viewer
                            Paragraph para = new Paragraph();
                            para.Inlines.Add(new Run(RepoFile.readFileText(repoFile.Filename)));
                            this.fileTextViewer.fileText.Blocks.Add(para);
                        });
                    }, true);
        }
    }
}
