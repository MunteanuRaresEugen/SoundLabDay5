using AudioEngineWrapper;
using AvaloniaFrontend.Services;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;

namespace AvaloniaFrontend.ViewModels;

public sealed partial class AcquisitionViewModel : ObservableObject
{
	private const int _sampleRate = 40960;

	private readonly AudioEngineService _audioEngineService = new(_sampleRate, _sampleRate / 5);

	[ObservableProperty, NotifyCanExecuteChangedFor(nameof(StopCommand))]
	private bool _canAcquire = false;

	[ObservableProperty]
	private AudioStrategyType _selectedStrategy;

	public AcquisitionViewModel(NavigationService navigationService)
	{
		NavigationService = navigationService;
	}

	public List<AudioStrategyType> AudioStrategyTypes { get; } = Enum.GetValues<AudioStrategyType>().ToList();
	public PlotViewModel TimePlot { get; } = new() { Title = "Time plot" };
	public PlotViewModel FFTPlot { get; } = new() { Title = "FFT plot" };
	public NavigationService NavigationService { get; }

	private bool StopCanExecute() => CanAcquire == true;

	[RelayCommand]
	public async Task Start()
	{
		CanAcquire = true;
		_audioEngineService.Start(SelectedStrategy);

		NavigationService.IsNavigationAllowed = false;

		PeriodicTimer timer = new(TimeSpan.FromMilliseconds(200));

		while (CanAcquire)
		{
			await timer.WaitForNextTickAsync();

			var buffer = _audioEngineService.GetBuffer();

			TimePlot.UpdatePlot(new Models.PlotData()
			{
				SampleRate = _sampleRate,
				YAxis = buffer
			});

			var fft = FftProcessor.GetMagnitude(_audioEngineService);

			FFTPlot.UpdatePlot(new Models.PlotData()
			{
				SampleRate = 0.2,
				YAxis = fft
			});

			_audioEngineService.ClearData();
		}
	}

	[RelayCommand(CanExecute = nameof(StopCanExecute))]
	public void Stop()
	{
		CanAcquire = false;
		_audioEngineService.Stop();

		NavigationService.IsNavigationAllowed = true;
	}
}
