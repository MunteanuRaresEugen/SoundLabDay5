namespace Core;

public class PageService
{
    public Dictionary<Type, PageData> Pages { get; } = new();

    public void RegisterPage<P, VM>(string pageName, string? icon = null, bool showSidePanel = true)
    {
        Pages.Add(typeof(P), new PageData()
        {
            Name = pageName,
            ViewModelType = typeof(VM),
            Type = typeof(P),
            Icon = icon,
            ShowSidePanel = showSidePanel
        });
    }
}
