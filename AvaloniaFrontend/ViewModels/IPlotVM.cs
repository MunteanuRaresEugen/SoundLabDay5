using AvaloniaFrontend.Models;

namespace AvaloniaFrontend.ViewModels;

public interface IPlotVM
{
    event Action<object, PlotData> OnPlotChanged;

    public string Title { get; }
}
