/////////////////////////////////////////////////////////////////////////////////
// RepoFileMetadata.cs                                                         //
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
    public class RepoFileMetadata
    {
        private string author_;
        private string description_;
        private List<RepoFile> dependencies_;

        public RepoFileMetadata()
        {
            dependencies_ = new List<RepoFile>();
        }

        public string Author
        {
            get { return author_; }
            set { author_ = value; }
        }

        public string Description
        {
            get { return description_; }
            set { description_ = value; }
        }

        public List<RepoFile> Dependencies
        {
            get { return dependencies_; }
            set { dependencies_ = value; }
        }
    }
}
