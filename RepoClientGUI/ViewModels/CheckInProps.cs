/////////////////////////////////////////////////////////////////////////////////
// CheckInProps.cs                                                             //
// ver 1.0                                                                     //
// Language:    C#, Visual Studio 2017                                         //
// Application: SoftwareRepository, CSE687 - Object Oriented Design            //
// Author:      Ritesh Nair (rgnair@syr.edu)                                   //
/////////////////////////////////////////////////////////////////////////////////

using RepoClientGUI.Utilities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RepoClientGUI.ViewModels
{
    public class CheckInProps : BaseViewModel
    {
        private string packageFolder_;
        private string packageName_;
        private string namespace_;
        private string packageDescription_;
        private string category_;

        public string PackageFolder
        {
            get { return packageFolder_; }
            set {
                packageFolder_ = value;
                this.OnPropertyChanged("PackageFolder");
                PackageName = FileSystem.GetDirectoryName(packageFolder_);
                Namespace = PackageName;
            }
        }

        public string PackageName
        {
            get { return packageName_; }
            set { packageName_ = value; this.OnPropertyChanged("PackageName"); }
        }

        public string Namespace
        {
            get { return namespace_; }
            set { namespace_ = value; this.OnPropertyChanged("Namespace"); }
        }

        public string PackageDescription
        {
            get { return packageDescription_; }
            set { packageDescription_ = value; }
        }

        public string Category
        {
            get { return category_; }
            set { category_ = value; }
        }
    }
}
