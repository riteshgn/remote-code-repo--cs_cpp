/////////////////////////////////////////////////////////////////////////////////
// RepoServerRequests.cs                                                       //
// ver 1.0                                                                     //
// Language:    C#, Visual Studio 2017                                         //
// Application: SoftwareRepository, CSE687 - Object Oriented Design            //
// Author:      Ritesh Nair (rgnair@syr.edu)                                   //
/////////////////////////////////////////////////////////////////////////////////

using MsgPassingCommunication;
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

        public void PostCheckIn(String package, String ns, String description,
            String file, String userId, Action<CheckInResponse> action,
            bool verbose = false)
        {
            string uniqueId = GetUniqueId();
            dispatcher_[uniqueId] = (CsMessage response) => {
                action(new CheckInResponse { Success = response.value("success"), RequestId = uniqueId });
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
            msg.add("package", package);
            msg.add("namespace", ns);
            msg.add("description", description);
            //msg.add("file", file);
            translater_.postMessage(msg);
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
