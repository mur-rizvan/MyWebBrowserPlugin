#include "MyWebBrowserPlugin.h"
#include "Logging/LogMacros.h"

#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2
#include "WebView2Manager.h"
#endif

DEFINE_LOG_CATEGORY_STATIC(LogMyWebBrowser, Log, All);

class FMyWebBrowserPluginModule;

IMPLEMENT_MODULE(FMyWebBrowserPluginModule, MyWebBrowserPlugin)

void FMyWebBrowserPluginModule::StartupModule()
{
    UE_LOG(LogMyWebBrowser, Log, TEXT("MyWebBrowserPlugin module started"));

#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2
    // Initialize WebView2 manager
    FWebView2Manager::Get().Initialize();
#endif
}

void FMyWebBrowserPluginModule::ShutdownModule()
{
    UE_LOG(LogMyWebBrowser, Log, TEXT("MyWebBrowserPlugin module shutdown"));

#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2
    // Shutdown WebView2 manager
    FWebView2Manager::Get().Shutdown();
#endif
}

