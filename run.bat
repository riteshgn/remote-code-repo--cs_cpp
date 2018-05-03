cls
pushd Debug
start RemoteCodeRepository.exe
popd
timeout /t 2
pushd RepoClientGUI\bin\Debug
start RepoClientGUI.exe
popd