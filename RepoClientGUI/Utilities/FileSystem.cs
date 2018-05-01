/////////////////////////////////////////////////////////////////////////
// FileSystem.cs - Utility functions for interacting with File System  //
// ver 1.1                                                             //
// Language:    C#, Visual Studio 2017                                 //
// Application: SoftwareRepository, CSE687 - Object Oriented Design    //
// Author:      Ritesh Nair (rgnair@syr.edu)                           //
/////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides below APIs for helping with file-based IO
* - CopyFile
* - CreateDirectory
* - GetDirectoryName
* - GetFilesInDirectory
* - GetFileText
*
* Required Packages:
* ------------------
* System.IO
*
* Maintenance History:
* --------------------
* ver 1.1 : 01 May 2018
* - added API to create Diretcory
* ver 1.0 : 26 Apr 2018
* - first release
*/

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RepoClientGUI.Utilities
{
    public class FileSystem
    {
        // ----< creates the specified directory >--------------------
        /*
         * Note: Will create all intermediate paths. And if directory already exists then does nothing
         */
        public static string CreateDirectory(string destFolderPath, string rootPath = "")
        {
            string finalPath = Path.Combine(rootPath, destFolderPath);
            Directory.CreateDirectory(finalPath);
            return finalPath;
        }

        // ----< copies file from source to destination >--------------------
        /*
         * Note: Will override file in destination if file with same name exists in destination
         */
        public static bool CopyFile(string sourceFilePath, string destFolderPath)
        {
            string filename = Path.GetFileName(sourceFilePath);
            File.Copy(sourceFilePath, Path.Combine(destFolderPath, filename), true);
            return true;
        }

        // ----< gets the directory name from the given path >--------------------
        /*
         * *** Only supports windows style path separators ***
         * 
         * Examples
         * >>   C:\users\usr\project\package  --> package
         * >>   C:\users\usr\project\package\ --> package
         * >>   package                       --> package
         * >>   /home/usr/project/package     --> /home/usr/project/package
         */
        public static string GetDirectoryName(string dirPath)
        {
            // - the index after the last index of '\' in the directory path will be the 
            //   starting point of extraction algorithm.
            // - the endpoint of extraction; defaults to lenth of the string
            // ex: in 'C:\users\usr\project\package' extraction should start from index 21

            int fromIndex = dirPath.LastIndexOf('\\') + 1;
            int toIndex = dirPath.Length;

            // - handles the case when no '\' is found in provided directory path
            if (fromIndex == 0)
                return dirPath;

            // - handles the case when provided directory path ends in a '\'
            // - In this case, extraction starts from the index after the second last 
            //   index of '\' 
            // - the endpoint will be one less than the end of string
            // ex: in 'C:\users\usr\project\package\' extraction should start from index 21

            if (fromIndex == toIndex)
            {
                toIndex--;
                fromIndex = dirPath.Substring(0, toIndex).LastIndexOf('\\') + 1;
            }

            // - extract based on computed start and end indices using the Substring API
            // - the Substring API takes number of characters as the second argument
            return dirPath.Substring(fromIndex, toIndex - fromIndex);
        }

        // ----< gets all files in a given directory >--------------------
        public static IEnumerable<Tuple<string, string>> GetFilesInDirectory(string dirPath, String pattern = "*")
        {
            if (Directory.Exists(dirPath))
            {
                return Directory.GetFiles(dirPath, pattern)
                    .Select((filePath) => new Tuple<string, string>(Path.GetFileName(filePath), filePath));
            }

            return Enumerable.Empty<Tuple<string, string>>();
        }

        // ----< gets the text from provided file >--------------------
        public static string GetFileText(string filePath)
        {
            // reference: https://docs.microsoft.com/en-us/dotnet/standard/io/how-to-read-text-from-a-file
            try
            {   // Open the text file using a stream reader.
                using (StreamReader sr = new StreamReader(filePath))
                {
                    // Read the stream to a string, and write the string to the console.
                    return sr.ReadToEnd();
                }
            }
            catch (Exception exp)
            {
                return ($"The file could not be read: {filePath}");
            }
        }
    }
}
