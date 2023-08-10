namespace Core;

public class PageData
{
    public required Type Type { get; init; }
    public required string Name { get; init; }
    public required Type ViewModelType { get; init; }
    public string? Icon { get; init; }
    public bool ShowSidePanel { get; init; } = true;
}
