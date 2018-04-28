///////////////////////////////////////////////////////////////////////////////////
// NegateBoolConverter.cs - Negates the boolean binding values                   //
// ver 1.0                                                                       //
// Language:    C#, Visual Studio 2017                                           //
// Application: SoftwareRepository, CSE687 - Object Oriented Design              //
// Author:      Ritesh Nair (rgnair@syr.edu)                                     //
///////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package implements a converter which converts boolean model values into 
* its negation. This is useful when having to set boolean properties on components
* for 'not' values as this is not supported in Binding expressions.
* ex: {Binding Path=!DataContext.DoThisAction} is not supported instead use
*     {Binding Path=DataContext.DoThisAction,  Converter={StaticResource negateBoolConverter}}
* Mappings
*   - true  <-> false
*   - false <-> true
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
using System.Windows.Data;

namespace RepoClientGUI.Converters
{
    public class NegateBoolConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if ((bool)value)
                return false;

            return true;
        }

        public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
        {
            if ((bool)value)
                return false;

            return true;
        }
    }
}
