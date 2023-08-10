using AudioEngineWrapper;
using Avalonia.Data;
using AvaloniaFrontend.Models;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using System;
using System.Collections.Generic;
using System.Linq;

namespace AvaloniaFrontend.ViewModels;

public sealed partial class GeneratorViewModel : ObservableObject
{
    [ObservableProperty]
    private SignalType _currentOption = SignalType.Sin;

    [ObservableProperty]
    [NotifyPropertyChangedFor(nameof(Time))]
    [NotifyCanExecuteChangedFor(nameof(UpdatePlotCommand))]
    private uint _sampleRate = 16384;

    [ObservableProperty]
    [NotifyPropertyChangedFor(nameof(SampleRate))]
    [NotifyCanExecuteChangedFor(nameof(UpdatePlotCommand))]
    private double _time = 2;

    [ObservableProperty]
    private double _freq = 5;

    public List<SignalType> Options { get; }

    public PlotViewModel TimePlot { get; } = new() { Title = "Time plot" };

    public PlotViewModel FFTPlot { get; } = new() { Title = "FFT plot" };

    public GeneratorViewModel()
    {
        var data = Enum.GetValues<SignalType>().ToList();
        Options = data;
    }

    [RelayCommand(CanExecute = nameof(AreTimeAndSampleRateValid))]
    private void UpdatePlot()
    {
        using var generator = new SignalGenerator(Time, SampleRate, Freq);
        var signal = generator.Generate(CurrentOption);

        double[] fft = FftProcessor.GetMagnitude(generator);

        TimePlot.UpdatePlot(new PlotData()
        {
            YAxis = signal,
            SampleRate = generator.SampleRate,
        });

        FFTPlot.UpdatePlot(new PlotData()
        {
            YAxis = fft,
            SampleRate = Time
        });
    }

    partial void OnSampleRateChanged(uint value)
    {
        if (AreTimeAndSampleRateValid() is false)
        {
            UpdatePlotCommand.NotifyCanExecuteChanged();
            throw new DataValidationException("Invalid Time and Sample Rate");
        }
    }

    partial void OnTimeChanged(double value)
    {
        if (AreTimeAndSampleRateValid() is false)
        {
            UpdatePlotCommand.NotifyCanExecuteChanged();
            throw new DataValidationException("Invalid Time and Sample Rate");
        }
    }

    private bool AreTimeAndSampleRateValid()
    {
        return IsPowerOfTwo((ulong)(SampleRate * Time));
    }

    private static bool IsPowerOfTwo(ulong x) => (x & (x - 1)) == 0;
}
