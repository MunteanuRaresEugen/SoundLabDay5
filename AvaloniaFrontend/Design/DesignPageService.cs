using Core;

namespace AvaloniaFrontend.Design;

/// <summary>
/// Used only for view design purposes
/// </summary>
public sealed class DesignPageService : PageService
{
    public DesignPageService()
    {
        RegisterPage<int, int>("First Page", Material.Icons.MaterialIconKind.MaterialUi.ToString());
        RegisterPage<string, string>("Second Page");
        RegisterPage<float, string>("Third Page");
        RegisterPage<double, string>("Fourth Page");
        RegisterPage<bool, string>("Fifth Page");
    }
}
