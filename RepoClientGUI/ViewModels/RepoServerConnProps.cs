/////////////////////////////////////////////////////////////////////////////////
// RepoServerConnProps.cs                                                      //
// ver 1.0                                                                     //
// Language:    C#, Visual Studio 2017                                         //
// Application: SoftwareRepository, CSE687 - Object Oriented Design            //
// Author:      Ritesh Nair (rgnair@syr.edu)                                   //
/////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RepoClientGUI.ViewModels
{
    public class RepoServerConnProps : BaseViewModel
    {
        private string serverAddress_;
        private int serverPort_;
        private string userId_;
        private bool connected_;
        private string statusMsg_;

        public RepoServerConnProps()
        {
            serverAddress_ = "localhost";
            serverPort_ = 7790;
            connected_ = false;
            statusMsg_ = "Disconnected";
        }

        public string ServerAddress
        {
            get { return serverAddress_;  }
            set { serverAddress_ = value; }
        }

        public int ServerPort
        {
            get { return serverPort_; }
            set { serverPort_ = value; }
        }

        public string UserId
        {
            get { return userId_; }
            set { userId_ = value; }
        }

        public bool Connected
        {
            get { return connected_; }
            set { connected_ = value; this.OnPropertyChanged("Connected"); }
        }

        public string StatusMsg
        {
            get { return statusMsg_; }
            set { statusMsg_ = value; this.OnPropertyChanged("StatusMsg"); }
        }
    }
}
