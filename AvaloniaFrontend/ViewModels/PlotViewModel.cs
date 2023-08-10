using AvaloniaFrontend.Models;

namespace AvaloniaFrontend.ViewModels;

public sealed class PlotViewModel : IPlotVM
{
    public required string Title { get; init; }

    public event Action<object, PlotData>? OnPlotChanged;

    public void UpdatePlot(PlotData data)
    {
        OnPlotChanged?.Invoke(this, data);
    }
}
