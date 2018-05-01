////////////////////////////////////////////////////////////////////////////////
// CheckOutFilesPopip.xaml.cs - View which provides controls for Check-Out    //
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
* ver 1.0 : 01 May 2018
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

namespace RepoClientGUI.Views.Popups
{
    /// <summary>
    /// Interaction logic for CheckOutFilesPopup.xaml
    /// </summary>
    public partial class CheckOutFilesPopup : Window
    {
        // reference: https://msdn.microsoft.com/en-us/library/system.windows.forms.folderbrowserdialog.aspx
        private System.Windows.Forms.FolderBrowserDialog browseCheckoutFolderDialog;

        public CheckOutFilesPopup()
        {
            InitializeComponent();
            SetupBrowseCheckoutFolderDialog();
        }

        // ----< pops-up a folder select dialog box to select the checkout folder >--------------------
        private void BrowseCheckOutFolderBtn_Click(object sender, RoutedEventArgs e)
        {
            // Show the FolderBrowserDialog.
            System.Windows.Forms.DialogResult result = browseCheckoutFolderDialog.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK)
            {
                RepoClientState state = (RepoClientState)this.DataContext;
                state.CheckOutProps.CheckoutFolder = browseCheckoutFolderDialog.SelectedPath;
            }
        }

        // ----< setup the browse folder dialog >--------------------
        private void SetupBrowseCheckoutFolderDialog()
        {
            this.browseCheckoutFolderDialog = new System.Windows.Forms.FolderBrowserDialog
            {
                // Set the help text description for the FolderBrowserDialog.
                Description = "Select the package directory that you want to check-in to the repository",
                // Do not allow the user to create new files via the FolderBrowserDialog.
                ShowNewFolderButton = false,
                // Default to the My Documents folder.
                RootFolder = Environment.SpecialFolder.MyComputer
            };
        }
    }
}
