/////////////////////////////////////////////////////////////////////////////////
// RepoPackage.cs                                                              //
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
    public class RepoPackage
    {
        private string packageName_;
        private List<RepoFile> repoFiles_;

        public string PackageName
        {
            get { return packageName_; }
            set { packageName_ = value; }
        }

        public List<RepoFile> RepoFiles
        {
            get { return repoFiles_; }
            set { repoFiles_ = value; }
        }
    }
}
