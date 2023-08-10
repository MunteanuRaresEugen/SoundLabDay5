using Avalonia;
using Avalonia.Logging;
using Avalonia.ReactiveUI;
using Serilog;

namespace AvaloniaFrontend;

internal class Program
{
    [STAThread]
    public static void Main(string[] args)
    {
        TaskScheduler.UnobservedTaskException += UnobservedTaskException;

        Log.Logger = new LoggerConfiguration()
            .WriteTo.File("logs.txt")
            .CreateLogger();

        try
        {
            BuildAvaloniaApp().StartWithClassicDesktopLifetime(args);
        }
        catch (Exception e)
        {
            Log.Logger.Error(e, $"Exception caught in main: {e.Message}");
        }
        finally
        {
            Log.CloseAndFlush();
        }
    }

    private static void UnobservedTaskException(object? sender, UnobservedTaskExceptionEventArgs e)
    {
        Log.Logger.Error(e.Exception.InnerException, $"Unobserved task exception: {e.Exception.InnerException}");
        Log.CloseAndFlush();

        // throws the exception to terminate the process
        throw e.Exception;
    }

    // Avalonia configuration, don't remove; also used by visual designer.
    public static AppBuilder BuildAvaloniaApp()
        => AppBuilder.Configure<App>()
            .UsePlatformDetect()
            .LogToTrace(LogEventLevel.Debug, LogArea.Property, LogArea.Layout)
            .UseReactiveUI();
}
