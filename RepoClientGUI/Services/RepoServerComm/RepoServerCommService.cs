/////////////////////////////////////////////////////////////////////////////////
// RepoServerCommService.cs - Implements the communication service for the GUI //
// ver 1.0                                                                     //
// Language:    C#, Visual Studio 2017                                         //
// Application: SoftwareRepository, CSE687 - Object Oriented Design            //
// Author:      Ritesh Nair (rgnair@syr.edu)                                   //
/////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements the communication layer for the GUI.
* It starts a socket server and a socket listener which are used to send requests 
* to the repository server and handle corresponding responses.
*
* Required Packages:
* ------------------
* MsgPassingCommunication
* Translater.dll
*
* Maintenance History:
* --------------------
* ver 1.0 : 11 Apr 2018
* - first release
*/

// Translater has to be statically linked with CommLibWrapper
// - loader can't find Translater.dll dependent CommLibWrapper.dll

using MsgPassingCommunication;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace RepoClientGUI.Services.RepoServerComm
{
    public class RepoServerCommService
    {

        private static RepoServerCommService instance_;

        private Translater translater_;
        private CsEndPoint endPoint_;
        private CsEndPoint serverEndPoint_;
        private Thread messagesListenerThread_ = null;
        private Dictionary<string, Action<CsMessage>> dispatcher_;
        private RepoServerRequests requests_;

        private RepoServerCommService(String bindHost, int bindPort)
        {
            endPoint_ = new CsEndPoint();
            endPoint_.machineAddress = bindHost;
            endPoint_.port = bindPort;

            dispatcher_ = new Dictionary<string, Action<CsMessage>>();
        }

        public static RepoServerCommService GetInstance(
            String bindHost = "localhost", int bindPort = 7890)
        {
            if (instance_ == null)
            {
                instance_ = new RepoServerCommService(bindHost, bindPort);
            }
            return instance_;
        }

        public void Start(String serverHost = "localhost", int serverPort = 7790)
        {
            serverEndPoint_ = new CsEndPoint();
            serverEndPoint_.machineAddress = serverHost;
            serverEndPoint_.port = serverPort;

            StartServer();
            Console.WriteLine($"\n\n  --> Client server @ {endPoint_.machineAddress}:{endPoint_.port} has been started");

            requests_ = new RepoServerRequests(translater_, dispatcher_, endPoint_, serverEndPoint_);

            Console.WriteLine($"  --> Starting message listener...");
            StartMessagesListenerThread();
        }

        public RepoServerRequests Requests { get { return requests_; } }

        public CsEndPoint EndPoint { get { return endPoint_; } }

        private void StartMessagesListenerThread()
        {
            ThreadStart listen = () =>
            {
                while (true)
                {
                    CsMessage message = translater_.getMessage();
                    if (message.containsKey("responseId"))
                    {
                        string uniqueId = message.value("responseId");
                        if (dispatcher_.ContainsKey(uniqueId))
                            dispatcher_[uniqueId].Invoke(message);
                    }

                }
            };
            messagesListenerThread_ = new Thread(listen);
            messagesListenerThread_.IsBackground = true;
            messagesListenerThread_.Start();
        }

        private void StartServer()
        {
            translater_ = new Translater();
            translater_.listen(endPoint_);
        }

    }
}
