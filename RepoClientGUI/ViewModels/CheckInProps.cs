﻿/////////////////////////////////////////////////////////////////////////////////
// CheckInProps.cs                                                             //
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
    public class CheckInProps
    {
        private string packageFolder_;
        private string namespace_;
        private string packageDescription_;

        public string PackageFolder
        {
            get { return packageFolder_; }
            set { packageFolder_ = value; }
        }

        public string Namespace
        {
            get { return namespace_; }
            set { namespace_ = value; }
        }

        public string PackageDescription
        {
            get { return packageDescription_; }
            set { packageDescription_ = value; }
        }
    }
}
