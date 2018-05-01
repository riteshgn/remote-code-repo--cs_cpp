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
        private string category_;
        private bool dependenciesAreFetched_;
        private List<RepoPackage> repoPackages_;
        private List<RepoFile> repoFiles_;
        private RepoFile selectedRepoFile_;
        private List<RepoFile> selectionPreview_;
        private Dictionary<string, RepoFileMetadata> metadataDict_;
        private SubcribableCustomCommand<RepoFile> showMetadataCommand_;
        private SubcribableCustomCommand<RepoFile> showFileTextCommand_;

        public BrowseProps()
        {
            repoPackages_ = new List<RepoPackage>();
            repoFiles_ = new List<RepoFile>();
            selectionPreview_ = new List<RepoFile>();
            metadataDict_ = new Dictionary<string, RepoFileMetadata>();
            showMetadataCommand_ = new SubcribableCustomCommand<RepoFile>();
            showFileTextCommand_ = new SubcribableCustomCommand<RepoFile>();
        }

        public string Category
        {
            get { return category_; }
            set { category_ = value; this.OnPropertyChanged("Category"); }
        }

        public bool DependenciesAreFetched
        {
            get { return dependenciesAreFetched_; }
            set { dependenciesAreFetched_ = value; }
        }

        public List<RepoPackage> RepoPackages
        {
            get { return repoPackages_; }
            set { repoPackages_ = value; this.OnPropertyChanged("RepoPackages"); }
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

        public List<RepoFile> SelectionPreview
        {
            get { return selectionPreview_; }
            set { selectionPreview_ = value; this.OnPropertyChanged("SelectionPreview"); }
        }

        public void UpdateSelectionPreview()
        {
            List<RepoFile> selections = new List<RepoFile>();
            foreach (RepoPackage pkg in repoPackages_)
            {
                if (pkg.RepoFiles != null)
                {
                    List<RepoFile> pkgSelections = pkg.RepoFiles.Where(file => file.RepoFileMarked).ToList();
                    selections.AddRange(pkgSelections);
                }
            }
            SelectionPreview = selections;
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
