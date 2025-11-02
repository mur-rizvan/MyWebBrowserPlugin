#pragma once

#include "CoreMinimal.h"
#include "HAL/PlatformProcess.h"

#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2

// Forward declarations
struct ICoreWebView2Environment;
struct ICoreWebView2EnvironmentOptions;
struct ICoreWebView2Controller;
struct ICoreWebView2;

class FWebView2Manager
{
public:
    static FWebView2Manager& Get();

    // Initialize WebView2 environment
    bool Initialize();

    // Cleanup
    void Shutdown();

    // Get WebView2 environment
    ICoreWebView2Environment* GetEnvironment() const { return WebView2Environment; }

    // Check if initialized
    bool IsInitialized() const { return bInitialized; }

private:
    FWebView2Manager();
    ~FWebView2Manager();

    ICoreWebView2Environment* WebView2Environment;
    bool bInitialized;
    
    static FWebView2Manager* Instance;
};

#endif // MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2

