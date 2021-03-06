﻿/////////////////////////////////////////////////////////////////////////////////
// CheckOutProps.cs                                                            //
// ver 1.0                                                                     //
// Language:    C#, Visual Studio 2017                                         //
// Application: SoftwareRepository, CSE687 - Object Oriented Design            //
// Author:      Ritesh Nair (rgnair@syr.edu)                                   //
/////////////////////////////////////////////////////////////////////////////////

using RepoClientGUI.Commands;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RepoClientGUI.ViewModels
{
    public class CheckOutProps : BaseViewModel
    {
        private string checkoutFolder_;
        private bool dependenciesAreRequired_;
        private SubcribableCustomCommand<CheckOutProps> downloadRequestCommand_;

        public CheckOutProps()
        {
            dependenciesAreRequired_ = true;
            downloadRequestCommand_ = new SubcribableCustomCommand<CheckOutProps>();
        }

        public string CheckoutFolder
        {
            get { return checkoutFolder_; }
            set { checkoutFolder_ = value; this.OnPropertyChanged("CheckoutFolder"); }
        }

        public bool DependentsAreRequired
        {
            get { return dependenciesAreRequired_; }
            set { dependenciesAreRequired_ = value; this.OnPropertyChanged("DependentsAreRequired"); }
        }

        public SubcribableCustomCommand<CheckOutProps> DownloadRequestCommand
        {
            get { return downloadRequestCommand_; }
        }
    }
}
