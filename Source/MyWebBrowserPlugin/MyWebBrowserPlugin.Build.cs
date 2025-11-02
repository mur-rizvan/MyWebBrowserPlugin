using UnrealBuildTool;

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
            
            // WebView2 - используем системный runtime или loader
            // Для статической линковки нужен WebView2LoaderStatic.lib
            // Для динамической линковки используем CreateCoreWebView2EnvironmentWithOptions
            
            // Если используете статический loader, раскомментируйте:
            // string WebView2LibPath = ModuleDirectory + "/ThirdParty/WebView2/lib/Win64/";
            // PublicAdditionalLibraries.Add(WebView2LibPath + "WebView2LoaderStatic.lib");
            
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

