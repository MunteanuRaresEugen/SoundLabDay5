using Avalonia.Controls;
using AvaloniaFrontend.Services;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Core;

namespace AvaloniaFrontend.ViewModels;

public sealed partial class MainWindowViewModel : ObservableObject
{
    private readonly ServiceCollection _serviceCollection;
    [ObservableProperty]
    private UserControl? _content;

    [ObservableProperty]
    private SplitViewDisplayMode _mode = SplitViewDisplayMode.CompactInline;

    [ObservableProperty]
    private bool _isPaneOpen = true;

    public List<PageData> Pages { get; }
    public NavigationService Navigation { get; }

    public MainWindowViewModel(ServiceCollection serviceCollection, PageService pageService, NavigationService navigation)
    {
        _serviceCollection = serviceCollection;
        Navigation = navigation;
        Pages = pageService.Pages.Select(x => x.Value).ToList();

        if (navigation.CurrentPageType is not null)
        {
            ButtonClick(pageService.Pages[navigation.CurrentPageType]);
        }

        navigation.CurrentPageChanged += type =>
        {
            ButtonClick(pageService.Pages[type]);
        };
    }

    [RelayCommand]
    public void ButtonClick(PageData pageData)
    {
        if (Content is { } oldContent)
        {
            DataContextIsActiveChanged(false, oldContent.DataContext);
        }

        var control = _serviceCollection.GetService(pageData.Type!) as UserControl ?? throw new System.Exception("null control");
        control.DataContext = _serviceCollection.GetService(pageData.ViewModelType!);
        Content = control;

        DataContextIsActiveChanged(true, control.DataContext);

        var oldMode = Mode;
        Mode = pageData.ShowSidePanel ? SplitViewDisplayMode.CompactInline : SplitViewDisplayMode.Inline;

        if (pageData.ShowSidePanel is false)
        {
            IsPaneOpen = false;
        }
        else if (IsPaneOpen is false && oldMode is SplitViewDisplayMode.Inline)
        {
            IsPaneOpen = true;
        }
    }

    public static void IsActiveChanged(bool isActive, IActiveAware activeAware)
    {
        if (isActive)
        {
            activeAware.OnActivated();
        }
        else
        {
            activeAware.OnDeactivated();
        }
    }

    private static void DataContextIsActiveChanged(bool isActive, object? dataContext)
    {
        if (dataContext is IActiveAware activeAware)
        {
            IsActiveChanged(isActive, activeAware);
        }
    }
}
