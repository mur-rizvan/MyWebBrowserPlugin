using UnrealBuildTool;
using System.IO;

public class MyWebBrowserPlugin : ModuleRules
{
    public MyWebBrowserPlugin(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "Slate",
            "SlateCore",
            "UMG"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "RenderCore",
            "RHI",
            "InputCore",
            "ApplicationCore"
        });

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicDefinitions.Add("MYWEBBROWSER_WINDOWS=1");
            PublicDefinitions.Add("MYWEBBROWSER_WEBVIEW2=1");
            
            // WebView2 SDK paths inside the plugin
            string WebView2Dir = Path.Combine(ModuleDirectory, "ThirdParty", "WebView2");
            string WebView2Include = Path.Combine(WebView2Dir, "include");
            string WebView2Lib = Path.Combine(WebView2Dir, "lib", "Win64");

            if (Directory.Exists(WebView2Include))
            {
                PublicIncludePaths.Add(WebView2Include);
            }

            if (Directory.Exists(WebView2Lib))
            {
                PublicAdditionalLibraries.Add(Path.Combine(WebView2Lib, "WebView2LoaderStatic.lib"));
            }
            
            // Добавляем необходимые библиотеки Windows
            PublicSystemLibraries.Add("shlwapi.lib");
            PublicSystemLibraries.Add("version.lib");
        }
        else
        {
            PublicDefinitions.Add("MYWEBBROWSER_WINDOWS=0");
            PublicDefinitions.Add("MYWEBBROWSER_WEBVIEW2=0");
        }
    }
}

