///////////////////////////////////////////////////////////////////////////////////
// BoolToVisibilityConverter.cs - Converts boolean values to Visibilty options   //
//                                and vice versa                                 //
// ver 1.0                                                                       //
// Language:    C#, Visual Studio 2017                                           //
// Application: SoftwareRepository, CSE687 - Object Oriented Design              //
// Author:      Ritesh Nair (rgnair@syr.edu)                                     //
///////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements a converter which converts boolean model values into 
* Visibilty options.
* ex: {Binding Path=DataContext.IsVisible,  Converter={StaticResource boolToVisibiltyConverter}}
* Mappings
*   - true  <-> Visibility.Visible
*   - false <-> Visibility.Hidden
*
* Required Packages:
* ------------------
* - Nil -
*
* Maintenance History:
* --------------------
* ver 1.0 : 25 Aprr 2018
* - first release
*/

using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;

namespace RepoClientGUI.Converters
{
    public class BoolToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if ((bool)value)
                return Visibility.Visible;

            return Visibility.Hidden;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            Visibility visibility = (Visibility)value;
            if (Visibility.Visible == visibility)
                return true;

            return false;
        }
    }
}
