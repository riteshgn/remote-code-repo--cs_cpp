/////////////////////////////////////////////////////////////////////////////////
// RepoServerRequests.cs                                                       //
// ver 1.2                                                                     //
// Language:    C#, Visual Studio 2017                                         //
// Application: SoftwareRepository, CSE687 - Object Oriented Design            //
// Author:      Ritesh Nair (rgnair@syr.edu)                                   //
/////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements API which Post Requests to the remote server. These include
* - GetFileMetadata
* - GetFileText
* - GetPackageFiles
* - GetRepoPackages
* - PostCheckIn
* - PostCheckOut
* - Ping
* Apart from these it also has an API which Requests to shutdown the client's comm system
* - PostQuit
*
* Required Packages:
* ------------------
* MsgPassingCommunication
* Translater.dll
*
* Maintenance History:
* --------------------
* ver 1.2 : 28 Apr 2018
* - Updated check-in API with all required attributes
* ver 1.1 : 26 Apr 2018
* - added API to disconnect the comm service
* ver 1.0 : 11 Apr 2018
* - first release
*/

using MsgPassingCommunication;
using RepoClientGUI.Utilities;
using RepoClientGUI.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RepoClientGUI.Services.RepoServerComm
{
    public class RepoServerRequests
    {
        private Translater translater_;
        private Dictionary<string, Action<CsMessage>> dispatcher_;
        private CsEndPoint endPoint_;
        private CsEndPoint serverEndPoint_;

        public RepoServerRequests(Translater translater,
            Dictionary<string, Action<CsMessage>> dispatcher,
            CsEndPoint endPoint,
            CsEndPoint serverEndPoint)
        {
            translater_ = translater;
            dispatcher_ = dispatcher;
            endPoint_ = endPoint;
            serverEndPoint_ = serverEndPoint;
        }

        public void GetFileMetadata(String package, String ns, String filename,
            int version, String userId, Action<GetFileMetadataResponse> action,
            bool verbose = false)
        {
            string uniqueId = GetUniqueId();
            dispatcher_[uniqueId] = (CsMessage response) => {
                List<RepoFile> dependencies = new List<RepoFile>();
                int count = 1;
                while (true)
                {
                    if (!response.containsKey($"dependency-{count}"))
                        break;

                    string fileInfo = response.value($"dependency-{count}");
                    dependencies.Add(RepoFile.parse(fileInfo));

                    count++;
                }

                RepoFileMetadata metadata = new RepoFileMetadata
                {
                    Author = response.value("author"),
                    Description = response.value("description"),
                    Dependencies = dependencies
                };
                action(new GetFileMetadataResponse { Metadata = metadata, RequestId = uniqueId });
                dispatcher_.Remove(uniqueId);
            };

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint_));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "get-file-metadata");
            msg.add("requestId", uniqueId);
            msg.add("userId", userId);
            if (verbose)
                msg.add("verbose", "yes");
            msg.add("package", package);
            msg.add("namespace", ns);
            msg.add("filename", filename);
            msg.add("version", version.ToString());
            translater_.postMessage(msg);
        }

        public void GetFileText(String package, String ns, String filename,
            int version, String userId, Action<GetFileTextResponse> action,
            bool verbose = false)
        {
            string uniqueId = GetUniqueId();
            dispatcher_[uniqueId] = (CsMessage response) =>
            {
                //response.show();
                action(new GetFileTextResponse { RequestId = uniqueId });
                dispatcher_.Remove(uniqueId);
            };

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint_));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "get-file-text");
            msg.add("requestId", uniqueId);
            msg.add("userId", userId);
            if (verbose)
                msg.add("verbose", "yes");
            msg.add("package", package);
            msg.add("namespace", ns);
            msg.add("filename", filename);
            msg.add("version", version.ToString());
            translater_.postMessage(msg);
        }

        public void GetPackageFiles(String packageName,
            String userId, Action<GetPackageFilesResponse> action,
            bool verbose = false)
        {
            string uniqueId = GetUniqueId();
            dispatcher_[uniqueId] = (CsMessage response) => {
                List<RepoFile> repoFiles = new List<RepoFile>();
                int count = 1;
                while (true)
                {
                    if (!response.containsKey($"file-{count}"))
                        break;

                    string fileInfo = response.value($"file-{count}");
                    repoFiles.Add(RepoFile.parse(fileInfo));

                    count++;
                }
                action(new GetPackageFilesResponse { RepoFiles = repoFiles, RequestId = uniqueId });

                dispatcher_.Remove(uniqueId);
            };

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint_));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "get-package-files");
            msg.add("requestId", uniqueId);
            msg.add("userId", userId);
            if (verbose)
                msg.add("verbose", "yes");
            msg.add("package", packageName);
            translater_.postMessage(msg);
        }

        public void GetRepoPackages(String userId,
            Action<GetRepoPackagesResponse> action, bool verbose = false)
        {
            string uniqueId = GetUniqueId();
            dispatcher_[uniqueId] = (CsMessage response) => {
                List<RepoPackage> packages = new List<RepoPackage>();

                int count = 1;
                while (true)
                {
                    if (!response.containsKey($"package-{count}"))
                        break;

                    packages.Add(new RepoPackage { PackageName = response.value($"package-{count}") });
                    count++;
                }
                action(new GetRepoPackagesResponse { RepoPackages = packages, RequestId = uniqueId });

                dispatcher_.Remove(uniqueId);
            };

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint_));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "get-repo-packages");
            msg.add("requestId", uniqueId);
            msg.add("userId", userId);
            if (verbose)
                msg.add("verbose", "yes");
            translater_.postMessage(msg);
        }

        // ----< posts check-in request to the server for every file within the selected package folder >--------------------
        public void PostCheckIn(String packageFolder, String packageName, String ns, 
            String description, String category, String userId, 
            Action<CheckInResponse> action, bool verbose = false)
        {
            postCheckInReq(packageFolder, "*.h", packageName, ns, description, category, userId, action, verbose);
            postCheckInReq(packageFolder, "*.cpp", packageName, ns, description, category, userId, action, verbose);
        }

        // ----< posts check-in request to the server for every file within the selected package folder matching the pattern >--
        private void postCheckInReq(String packageFolder, String pattern, String packageName, 
            String ns, String description, String category, String userId, 
            Action<CheckInResponse> action, bool verbose)
        {
            foreach (Tuple<string, string> fileInfo in FileSystem.GetFilesInDirectory(packageFolder, pattern))
            {
                FileSystem.CopyFile(fileInfo.Item2, "../SendFiles");

                string uniqueId = GetUniqueId();
                dispatcher_[uniqueId] = (CsMessage response) =>
                {
                    action(new CheckInResponse {
                        Success = response.value("success"),
                        RequestId = uniqueId,
                        File = fileInfo.Item2
                    });
                    dispatcher_.Remove(uniqueId);
                };

                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(serverEndPoint_));
                msg.add("from", CsEndPoint.toString(endPoint_));
                msg.add("command", "check-in");
                msg.add("requestId", uniqueId);
                msg.add("userId", userId);
                if (verbose)
                    msg.add("verbose", "yes");
                msg.add("package", packageName);
                msg.add("namespace", ns);
                msg.add("description", description);
                if (!String.IsNullOrEmpty(category))
                    msg.add("category", category);
                msg.add("file", fileInfo.Item1);
                translater_.postMessage(msg);
            }
        }

        public void PostCheckOut(String package, String ns, String filename,
            int version, String userId, Action<CheckOutResponse> action,
            bool verbose = false)
        {
            string uniqueId = GetUniqueId();
            dispatcher_[uniqueId] = (CsMessage response) => {
                action(new CheckOutResponse { Success = response.value("success"), RequestId = uniqueId });
                dispatcher_.Remove(uniqueId);
            };

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint_));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "check-out");
            msg.add("requestId", uniqueId);
            msg.add("userId", userId);
            if (verbose)
                msg.add("verbose", "yes");
            msg.add("package", package);
            msg.add("namespace", ns);
            msg.add("filename", filename);
            msg.add("version", version.ToString());
            translater_.postMessage(msg);
        }

        public void PostQuit(Action<CsMessage> onQuit, bool verbose = false)
        {
            string uniqueId = GetUniqueId();
            dispatcher_[uniqueId] = (CsMessage response) => {
                onQuit(response);
                dispatcher_.Remove(uniqueId);
            };

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(endPoint_));
            msg.add("command", "__quit");
            msg.add("responseId", uniqueId);
            msg.add("userId", "system");
            if (verbose)
                msg.add("verbose", "yes");
            translater_.postMessage(msg);
        }

        public void Ping(Action<PingResponse> action, bool verbose = false)
        {
            string uniqueId = GetUniqueId();
            dispatcher_[uniqueId] = (CsMessage response) => {
                action(new PingResponse { ServerActive = response.value("alive"), RequestId = uniqueId });
                dispatcher_.Remove(uniqueId);
            };

            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(serverEndPoint_));
            msg.add("from", CsEndPoint.toString(endPoint_));
            msg.add("command", "ping");
            msg.add("requestId", uniqueId);
            if (verbose)
                msg.add("verbose", "yes");
            translater_.postMessage(msg);
        }

        private string GetUniqueId()
        {
            // reference: https://msdn.microsoft.com/en-us/library/system.guid.newguid.aspx
            return Guid.NewGuid().ToString();
        }

    }
}
