#include "MyWebBrowserPlugin.h"
#include "Logging/LogMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogMyWebBrowser, Log, All);

class FMyWebBrowserPluginModule;

IMPLEMENT_MODULE(FMyWebBrowserPluginModule, MyWebBrowserPlugin)

void FMyWebBrowserPluginModule::StartupModule()
{
    UE_LOG(LogMyWebBrowser, Log, TEXT("MyWebBrowserPlugin module started"));
}

void FMyWebBrowserPluginModule::ShutdownModule()
{
    UE_LOG(LogMyWebBrowser, Log, TEXT("MyWebBrowserPlugin module shutdown"));
}
