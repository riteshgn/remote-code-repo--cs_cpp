/////////////////////////////////////////////////////////////////////////////////
// BrowseProps.cs                                                              //
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
    public class BrowseProps : BaseViewModel
    {
        private string checkoutFolder_;
        private bool dependenciesAreFetched_;
        private List<RepoPackage> repoPackages_;
        private List<RepoFile> repoFiles_;
        private RepoFile selectedRepoFile_;
        private Dictionary<string, RepoFileMetadata> metadataDict_;
        private SubcribableCustomCommand<RepoFile> showMetadataCommand_;
        private SubcribableCustomCommand<RepoFile> showFileTextCommand_;

        public BrowseProps()
        {
            repoPackages_ = new List<RepoPackage>();
            repoFiles_ = new List<RepoFile>();
            metadataDict_ = new Dictionary<string, RepoFileMetadata>();
            showMetadataCommand_ = new SubcribableCustomCommand<RepoFile>();
            showFileTextCommand_ = new SubcribableCustomCommand<RepoFile>();
        }

        public string CheckoutFolder
        {
            get { return checkoutFolder_; }
            set { checkoutFolder_ = value; }
        }

        public bool DependenciesAreFetched
        {
            get { return dependenciesAreFetched_; }
            set { dependenciesAreFetched_ = value; }
        }

        public List<RepoPackage> RepoPackages
        {
            get { return repoPackages_; }
            set { repoPackages_ = value; }
        }
        public List<RepoFile> RepoFiles
        {
            get { return repoFiles_; }
            set { repoFiles_ = value; this.OnPropertyChanged("RepoFiles"); }
        }

        public RepoFile SelectedRepoFile
        {
            get { return selectedRepoFile_; }
            set { selectedRepoFile_ = value; }
        }

        public Dictionary<string, RepoFileMetadata> MetadataDict
        {
            get { return metadataDict_; }
            set { metadataDict_ = value; }
        }

        public SubcribableCustomCommand<RepoFile> ShowMetadataCommand
        {
            get { return showMetadataCommand_; }
        }

        public SubcribableCustomCommand<RepoFile> ShowFileTextCommand
        {
            get { return showFileTextCommand_; }
        }
    }
}
