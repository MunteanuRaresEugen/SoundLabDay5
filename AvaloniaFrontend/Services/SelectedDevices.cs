using AudioEngineWrapper;
using CommunityToolkit.Mvvm.ComponentModel;

namespace AvaloniaFrontend.Services;

public partial class SelectedDevices : ObservableObject
{
	private AudioDevices _audioDevices;

	public SelectedDevices()
	{
		_audioDevices = new AudioDevices();
		CaptureDevices = _audioDevices.GetCaptureDevices().ToList();
		PlaybackDevices = _audioDevices.GetPlaybackDevices().ToList();

		_selectedCapture = CaptureDevices.First(x=>x.IsDefault);
		_selectedPlayback = CaptureDevices.First(x=>x.IsDefault);
	}

	public List<DeviceData> CaptureDevices { get; }
	public List<DeviceData> PlaybackDevices { get; }

	[ObservableProperty] private DeviceData _selectedCapture;
	[ObservableProperty] private DeviceData _selectedPlayback;
}