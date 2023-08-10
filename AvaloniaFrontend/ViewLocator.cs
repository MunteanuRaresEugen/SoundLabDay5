using Avalonia.Controls;
using Avalonia.Controls.Templates;
using CommunityToolkit.Mvvm.ComponentModel;
using System;

namespace AvaloniaFrontend;

public class ViewLocator : IDataTemplate
{
    public IControl Build(object data)
    {
        var name = data.GetType().FullName!.Replace("ViewModel", "View");
        var type = Type.GetType(name);

        return type != null ? (IControl)Activator.CreateInstance(type)! : new TextBlock { Text = "Not Found: " + name };
    }

    public bool Match(object data)
    {
        return data is ObservableObject;
    }
}
