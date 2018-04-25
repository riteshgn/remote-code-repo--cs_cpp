/////////////////////////////////////////////////////////////////////////////////
// RepoServerResponses.cs                                                      //
// ver 1.0                                                                     //
// Language:    C#, Visual Studio 2017                                         //
// Application: SoftwareRepository, CSE687 - Object Oriented Design            //
// Author:      Ritesh Nair (rgnair@syr.edu)                                   //
/////////////////////////////////////////////////////////////////////////////////

using RepoClientGUI.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RepoClientGUI.Services.RepoServerComm
{
    public class PingResponse : ServerResponse
    {
        private string serverActive_;
        public string ServerActive
        {
            get { return serverActive_; }
            set { serverActive_ = value; }
        }
    }

    public class CheckInResponse : ServerResponse { }

    public class CheckOutResponse : ServerResponse { }

    public class GetFileMetadataResponse : ServerResponse
    {
        private RepoFileMetadata metadata_;
        public RepoFileMetadata Metadata
        {
            get { return metadata_; }
            set { metadata_ = value; }
        }
    }

    public class GetFileTextResponse : ServerResponse
    { }

    public class GetPackageFilesResponse : ServerResponse
    {
        private List<RepoFile> repoFiles_;
        public List<RepoFile> RepoFiles
        {
            get { return repoFiles_; }
            set { repoFiles_ = value; }
        }
    }

    public class GetRepoPackagesResponse : ServerResponse
    {
        private List<RepoPackage> repoPackages_;
        public List<RepoPackage> RepoPackages
        {
            get { return repoPackages_; }
            set { repoPackages_ = value; }
        }
    }

    public class ServerResponse
    {
        private string success_;
        private string requestId_;

        public string Success
        {
            get { return success_; }
            set { success_ = value; }
        }

        public string RequestId
        {
            get { return requestId_; }
            set { requestId_ = value; }
        }
    }
}
