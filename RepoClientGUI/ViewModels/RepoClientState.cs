/////////////////////////////////////////////////////////////////////////////////
// RepoClientState.cs                                                          //
// ver 1.0                                                                     //
// Language:    C#, Visual Studio 2017                                         //
// Application: SoftwareRepository, CSE687 - Object Oriented Design            //
// Author:      Ritesh Nair (rgnair@syr.edu)                                   //
/////////////////////////////////////////////////////////////////////////////////

using RepoClientGUI.Services.RepoServerComm;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RepoClientGUI.ViewModels
{
    public class RepoClientState
    {
        private RepoServerConnProps connProps_;
        private ClientActionTabsProps clientActionTabsProps_;
        private CheckInProps checkInProps_;
        private CheckOutProps checkOutProps_;
        private BrowseProps browseProps_;

        private RepoServerCommService serverCommService_;

        public RepoClientState(RepoServerCommService serverCommService)
        {
            connProps_ = new RepoServerConnProps { };
            clientActionTabsProps_ = new ClientActionTabsProps { };
            checkInProps_ = new CheckInProps { };
            checkOutProps_ = new CheckOutProps { };
            browseProps_ = new BrowseProps { };

            serverCommService_ = serverCommService;
        }

        public RepoServerConnProps ServerConnProps
        {
            get { return connProps_; }
            set { connProps_ = value; }
        }

        public ClientActionTabsProps ActionTabsProps
        {
            get { return clientActionTabsProps_; }
            set { clientActionTabsProps_ = value; }
        }

        public CheckInProps CheckInProps
        {
            get { return checkInProps_; }
            set { checkInProps_ = value; }
        }

        public CheckOutProps CheckOutProps
        {
            get { return checkOutProps_; }
            set { checkOutProps_ = value; }
        }

        public BrowseProps BrowseProps
        {
            get { return browseProps_; }
            set { browseProps_ = value; }
        }

        public RepoServerCommService ServerCommService
        {
            get { return serverCommService_; }
        }
    }
}
