using AvaloniaFrontend.Services;
using CommunityToolkit.Mvvm.ComponentModel;

namespace AvaloniaFrontend.ViewModels;

public sealed partial class ConfigViewModel : ObservableObject
{
	public ConfigViewModel(SelectedDevices selectedDevices)
	{
		SelectedDevices = selectedDevices;
	}

	public SelectedDevices SelectedDevices { get; }
}