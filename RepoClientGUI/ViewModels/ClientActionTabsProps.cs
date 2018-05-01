/////////////////////////////////////////////////////////////////////////////////
// ClientActionTabsProps.cs                                                              //
// ver 1.0                                                                     //
// Language:    C#, Visual Studio 2017                                         //
// Application: SoftwareRepository, CSE687 - Object Oriented Design            //
// Author:      Ritesh Nair (rgnair@syr.edu)                                   //
/////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RepoClientGUI.ViewModels
{
    public class ClientActionTabsProps : BaseViewModel
    {
        private ActionTab connectTab_;
        private ActionTab checkinTab_;
        private ActionTab browseTab_;

        public ClientActionTabsProps()
        {
            connectTab_ = new ActionTab("Connect", true);
            checkinTab_ = new ActionTab("Check In", false);
            browseTab_ = new ActionTab("Browse", false);
        }

        public ActionTab ConnectTab
        {
            get { return connectTab_; }
            set { connectTab_ = value; }
        }

        public ActionTab CheckInTab
        {
            get { return checkinTab_; }
            set { checkinTab_ = value; }
        }

        public ActionTab BrowseTab
        {
            get { return browseTab_; }
            set { browseTab_ = value; }
        }
    }

    public class ActionTab : BaseViewModel
    {
        private string header_;
        private bool selected_;

        public ActionTab(string header, bool selected = false)
        {
            header_ = header;
            selected_ = selected;
        }

        public string Header
        {
            get { return header_; }
            set { header_ = value; this.OnPropertyChanged("Header"); }
        }

        public bool Selected
        {
            get { return selected_; }
            set { selected_ = value; this.OnPropertyChanged("Selected"); }
        }
    }
}
