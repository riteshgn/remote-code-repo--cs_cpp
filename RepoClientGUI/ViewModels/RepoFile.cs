/////////////////////////////////////////////////////////////////////////////////
// RepoFile.cs                                                                 //
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
    public class RepoFile : BaseViewModel
    {
        private bool repoFileMarked_;
        private string packageName_;
        private string namespace_;
        private string filename_;
        private int version_;

        public bool RepoFileMarked
        {
            get { return repoFileMarked_; }
            set { repoFileMarked_ = value; this.OnPropertyChanged("RepoFileMarked"); }
        }

        public string PackageName
        {
            get { return packageName_; }
            set { packageName_ = value; }
        }

        public string Namespace
        {
            get { return namespace_; }
            set { namespace_ = value; }
        }

        public string Filename
        {
            get { return filename_; }
            set { filename_ = value; }
        }

        public int Version
        {
            get { return version_; }
            set { version_ = value; }
        }

        // ----< parses a server key for file information >--------------------
        /*
         *  The server key is of the form <pacakge>#<namespace>#<filename>.<version>
         */
        public static RepoFile parse(string serverKey)
        {
            string[] parts = serverKey.Split('#');
            string packageName = parts[0];
            string ns = parts[1];

            int positionVerDot = parts[2].LastIndexOf('.');
            string filename = parts[2].Substring(0, positionVerDot);
            string versionStr = parts[2].Substring(positionVerDot + 1);

            return new RepoFile
            {
                PackageName = packageName,
                Namespace = ns,
                Filename = filename,
                Version = Int32.Parse(versionStr)
            };
        }

        // ----< stringifies file information into desired format  >--------------------
        public static string stringify(RepoFile repoFile)
        {
            return $"{repoFile.PackageName}#{repoFile.Namespace}#{repoFile.Filename}.{repoFile.Version}";
        }

        // ----< loads the contents of the file from disk  >--------------------
        public static string readFileText(string filename)
        {
            return FileSystem.GetFileText($"../SaveFiles/{filename}");
        }
    }
}
