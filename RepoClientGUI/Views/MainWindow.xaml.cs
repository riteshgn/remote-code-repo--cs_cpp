/////////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - Main Window for the Repository Client GUI      //
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
* ver 1.0 : 28 Mar 2018
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

namespace RepoClientGUI.Views
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private RepoClientState state_;

        public MainWindow(RepoClientState state)
        {
            InitializeComponent();
            
            state_ = state;
        }

        // ----< actions to be performed when the main window is loaded >--------------------
        private void MainWindow_Loaded(object sender, RoutedEventArgs e)
        {
            string machineEndpoint =
                $"{state_.ServerCommService.EndPoint.machineAddress}:" +
                $"{state_.ServerCommService.EndPoint.port}";

            // sets the console & window title
            Console.Title = $"Repository Client Console @ {machineEndpoint}"; 
            this.Title = $"Repository Client GUI @ {machineEndpoint}";

            // initializes the data context of the main window
            // and child views
            this.DataContext = state_;
        }
    }
}
