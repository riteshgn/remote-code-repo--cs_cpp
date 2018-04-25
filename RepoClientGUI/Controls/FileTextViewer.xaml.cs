////////////////////////////////////////////////////////////////////////////////
// FileTextViewer.xaml.cs - View File Contents                                //
// ver 1.0                                                                    //
// Language:    C#, Visual Studio 2017                                        //
// Application: SoftwareRepository, CSE687 - Object Oriented Design           //
// Author:      Ritesh Nair (rgnair@syr.edu)                                  //
////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements ta re-usable user control for displaying a files contents
* in a scrollable viewer.
*
* Required Packages:
* ------------------
* None
*
* Maintenance History:
* --------------------
* ver 1.0 : 11 Apr 2018
* - first release
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace RepoClientGUI.Controls
{
    /// <summary>
    /// Interaction logic for FileTextPopupWindow.xaml
    /// </summary>
    public partial class FileTextViewer : UserControl
    {
        public FileTextViewer()
        {
            InitializeComponent();
        }
    }
}
