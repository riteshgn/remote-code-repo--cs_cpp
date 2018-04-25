/////////////////////////////////////////////////////////////////////////////////
// CheckOutProps.cs                                                            //
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
    public class CheckOutProps : BaseViewModel
    {
        private string checkoutFolder_;
        private bool dependenciesAreFetched_;
        private List<RepoPackage> repoPackages_;
        private List<RepoFile> repoFiles_;
        private List<RepoFile> selectionPreview_;

        public CheckOutProps()
        {
            repoPackages_ = new List<RepoPackage>();
            repoFiles_ = new List<RepoFile>();
            selectionPreview_ = new List<RepoFile>();
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
    }
}
