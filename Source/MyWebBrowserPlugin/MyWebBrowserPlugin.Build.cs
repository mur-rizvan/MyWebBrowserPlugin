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
        });

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicDefinitions.Add("MYWEBBROWSER_WINDOWS=1");
        }
        else
        {
            PublicDefinitions.Add("MYWEBBROWSER_WINDOWS=0");
        }
    }
}

