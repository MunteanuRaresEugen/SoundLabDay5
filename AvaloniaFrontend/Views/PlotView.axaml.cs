using Avalonia.Controls;
using AvaloniaFrontend.Models;
using AvaloniaFrontend.ViewModels;

namespace AvaloniaFrontend.Views;

public partial class PlotView : UserControl
{
    public PlotView()
    {
        InitializeComponent();
    }

    protected override void OnDataContextChanged(EventArgs e)
    {
        base.OnDataContextChanged(e);

        if (DataContext is IPlotVM plotVM)
        {
            plotVM.OnPlotChanged += PlotVM_OnPlotChanged;

            Plot.Plot.Title(plotVM.Title);

            Plot.Plot.YAxis.TickLabelFormat("F4", false);
        }
    }

    private void PlotVM_OnPlotChanged(object sender, PlotData data)
    {
        if (sender != DataContext)
        {
            return;
        }

        Plot.Plot.Clear();
        Plot.Plot.AddSignal(data.YAxis, data.SampleRate);
        Plot.Refresh();
    }
}
