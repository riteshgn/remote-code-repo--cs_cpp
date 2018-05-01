///////////////////////////////////////////////////////////////////////////////////
// ConnectToRepoView.xaml.cs - View which provides controls for Repo Connection  //
// ver 1.0                                                                       //
// Language:    C#, Visual Studio 2017                                           //
// Application: SoftwareRepository, CSE687 - Object Oriented Design              //
// Author:      Ritesh Nair (rgnair@syr.edu)                                     //
///////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the View for connection to repository.
*
* Required Packages:
* ------------------
* RepoClientGUI.Services.RepoServerComm
* RepoClientGUI.ViewModels
*
* Maintenance History:
* --------------------
* ver 1.2 : 01 May 2018
* - Browse is the first tab that is displayed after successful connection
* ver 1.1 : 26 Apr 2018
* - implemented an exit button
* - on successful connection tabs are switched to the check-in tab
* - added validation for user id
* ver 1.0 : 29 Mar 2018
* - first release
*/

using RepoClientGUI.Services.RepoServerComm;
using RepoClientGUI.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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
    /// Interaction logic for ConnectToRepoView.xaml
    /// </summary>
    public partial class ConnectToRepoView : UserControl
    {
        private bool loaded_ = false;

        public ConnectToRepoView()
        {
            InitializeComponent();
        }

        // ----< starts the comm system and pings server on connect button click >--------------------
        private void ConnectToRepoBtn_Click(object sender, RoutedEventArgs e)
        {
            RepoClientState state = (RepoClientState)this.DataContext;

            if (String.IsNullOrEmpty(state.ServerConnProps.UserId))
            {
                MessageBox.Show("User Id is mandatory for connection", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            // start the comm system
            if (!state.ServerConnProps.Connected)
                state.ServerCommService.Start(state.ServerConnProps.ServerAddress, state.ServerConnProps.ServerPort);

            state.ServerCommService.Requests.Ping((PingResponse response) => {
                // mark connected
                state.ServerConnProps.Connected = true;

                // set the correct connection message in status bar
                string message = "Connected to server " +
                    $"{state.ServerConnProps.ServerAddress}:{state.ServerConnProps.ServerPort} " +
                    $"as user {state.ServerConnProps.UserId}";
                state.ServerConnProps.StatusMsg = message;

                // switch tabs
                state.ActionTabsProps.ConnectTab.Selected = false;
                state.ActionTabsProps.BrowseTab.Selected = true;

                // disable connection text boxes
                this.Dispatcher.Invoke(() => {
                    ServerAddressTextBox.IsEnabled = false;
                    ServerPortTextBox.IsEnabled = false;
                    UserIdTextBox.IsEnabled = false;
                });
                
            }, true);
        }

        // ----< stops the comm system and pings server on connect button click >--------------------

        private void DisconnectRepoBtn_Click(object sender, RoutedEventArgs e)
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            state.ServerCommService.Stop(() => {
                this.Dispatcher.Invoke(() => {
                    state.ServerConnProps.Connected = false;
                    state.ServerConnProps.StatusMsg = "Disconnected";
                    Application.Current.Shutdown();
                });
            });
        }

        // ----< actions to be performed when the view is loaded >--------------------
        private void ConnectToRepoView_Loaded(object sender, RoutedEventArgs e)
        {
            if (!loaded_) // ensures that this block is executed only once
            {
                loaded_ = true;
                //DemoConnection();
            }
        }

        // ----< demonstrates req #3a - connection to server >--------------------
        private void DemoConnection()
        {
            RepoClientState state = (RepoClientState)this.DataContext;
            state.ServerConnProps.UserId = "rgnair";
            state.ServerCommService.Start(state.ServerConnProps.ServerAddress, state.ServerConnProps.ServerPort);

            state.ServerCommService.Requests.Ping((PingResponse response) => {
                Console.WriteLine($"\n\n{new String('-', 60)}");
                Console.WriteLine("  Demonstrating Requirement #3a - Connecting to Servers");
                Console.WriteLine($"{new String('-', 60)}\n");
                Console.WriteLine($"    > Connection to server @ {state.ServerConnProps.ServerAddress}:{state.ServerConnProps.ServerPort} succeeded");
                Console.WriteLine($"    > Server responded to ping requestId [{response.RequestId}] with status alive: {response.ServerActive}");
                Console.WriteLine($"\n  Test Passed\n");

                state.ServerConnProps.Connected = true;

                string message = "Connected to server " +
                    $"{state.ServerConnProps.ServerAddress}:{state.ServerConnProps.ServerPort} " +
                    $"as user {state.ServerConnProps.UserId}";
                state.ServerConnProps.StatusMsg = message;
            }, true);
        }
    }
}
