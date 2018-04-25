/////////////////////////////////////////////////////////////////////////////////
// SubcribableCustomCommand.cs                                                 //
// ver 1.0                                                                     //
// Language:    C#, Visual Studio 2017                                         //
// Application: SoftwareRepository, CSE687 - Object Oriented Design            //
// Author:      Ritesh Nair (rgnair@syr.edu)                                   //
/////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace RepoClientGUI.Commands
{
    public class SubcribableCustomCommand<T> : ICommand
    {
        public event EventHandler CanExecuteChanged;

        private List<Action<T>> actions_;

        public SubcribableCustomCommand()
        {
            actions_ = new List<Action<T>>();
        }

        public bool CanExecute(object parameter)
        {
            return true;
        }

        public void Execute(object parameter)
        {
            T actualParam = (T)parameter;
            foreach (Action<T> action in actions_)
            {
                action(actualParam);
            }
        }

        public void Subscribe(Action<T> action)
        {
            actions_.Add(action);
        }
    }
}
