using AvaloniaFrontend.Services;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Core;

namespace AvaloniaFrontend.ViewModels;

public sealed partial class HomeViewModel : ObservableObject
{
    private readonly PageService _pageService;
    private readonly NavigationService _navigationService;

    public List<PageData> Pages { get; }

    public HomeViewModel(PageService pageService, NavigationService navigationService)
    {
        _pageService = pageService;
        _navigationService = navigationService;
        Pages = _pageService.Pages.Select(x => x.Value).Where(x => x.ViewModelType != this.GetType()).ToList();
    }

    [RelayCommand]
    private void ButtonClick(PageData pageData)
    {
        _navigationService.CurrentPageType = pageData.Type;
    }
}
