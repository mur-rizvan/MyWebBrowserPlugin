#include "WebView2Manager.h"

#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2

#include "Logging/LogMacros.h"
#include "Windows/WindowsHWrapper.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/PlatformProcess.h"
#include "Misc/Paths.h"
#include <shlwapi.h>

// WebView2 headers - try to include if available
#if __has_include(<WebView2.h>)
    #include <WebView2.h>
    #include <wrl.h>
    using namespace Microsoft::WRL;
    #define WEBVIEW2_AVAILABLE 1
#else
    // Forward declarations if WebView2.h is not available
    #pragma message("Warning: WebView2.h not found. Please install WebView2 SDK.")
    struct ICoreWebView2Environment;
    struct ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler;
    namespace Microsoft { namespace WRL { template<typename T> class Callback {}; } }
    #define WEBVIEW2_AVAILABLE 0
    // Stub for CreateCoreWebView2EnvironmentWithOptions
    inline HRESULT CreateCoreWebView2EnvironmentWithOptions(void*, void*, void*, void*) { return E_NOTIMPL; }
#endif

DEFINE_LOG_CATEGORY_STATIC(LogWebView2Manager, Log, All);

FWebView2Manager* FWebView2Manager::Instance = nullptr;

FWebView2Manager& FWebView2Manager::Get()
{
    if (!Instance)
    {
        Instance = new FWebView2Manager();
    }
    return *Instance;
}

FWebView2Manager::FWebView2Manager()
    : WebView2Environment(nullptr)
    , bInitialized(false)
{
}

FWebView2Manager::~FWebView2Manager()
{
    Shutdown();
}

bool FWebView2Manager::Initialize()
{
    if (bInitialized)
    {
        return true;
    }

    // Get path to WebView2Loader.dll
    FString WebView2LoaderPath;
    
    // Try to find WebView2Loader.dll in common locations
    TArray<FString> SearchPaths;
    SearchPaths.Add(FPaths::EngineDir() / TEXT("Binaries/ThirdParty/WebView2/Win64/"));
    SearchPaths.Add(FPaths::ProjectDir() / TEXT("Binaries/ThirdParty/WebView2/Win64/"));
    SearchPaths.Add(FPaths::EnginePluginsDir() / TEXT("MyWebBrowserPlugin/Binaries/ThirdParty/WebView2/Win64/"));
    
    bool bFound = false;
    for (const FString& SearchPath : SearchPaths)
    {
        FString TestPath = SearchPath / TEXT("WebView2Loader.dll");
        if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*TestPath))
        {
            WebView2LoaderPath = TestPath;
            bFound = true;
            break;
        }
    }

    if (!bFound)
    {
        // Try system installation (WebView2 Runtime)
        // WebView2Loader should be available via COM if runtime is installed
        UE_LOG(LogWebView2Manager, Warning, TEXT("WebView2Loader.dll not found. Trying system WebView2 Runtime..."));
    }

    // Create WebView2 environment
    // This uses system WebView2 Runtime if available
    // For custom loader, we'd need WebView2Loader.dll
    
#if WEBVIEW2_AVAILABLE
    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(
        nullptr,  // browserExecutableFolder (null = use system runtime)
        nullptr,  // userDataFolder (null = use default)
        nullptr,  // options
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT result, ICoreWebView2Environment* env) -> HRESULT
            {
                if (SUCCEEDED(result) && env)
                {
                    WebView2Environment = env;
                    WebView2Environment->AddRef();
                    bInitialized = true;
                    UE_LOG(LogWebView2Manager, Log, TEXT("WebView2 environment initialized successfully"));
                }
                else
                {
                    UE_LOG(LogWebView2Manager, Error, TEXT("Failed to create WebView2 environment. Error: 0x%08X"), result);
                    
                    // Check if WebView2 Runtime is installed
                    if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
                    {
                        UE_LOG(LogWebView2Manager, Error, TEXT("WebView2 Runtime not found. Please install Microsoft Edge WebView2 Runtime."));
                    }
                }
                return S_OK;
            }).Get());

    if (FAILED(hr))
    {
        UE_LOG(LogWebView2Manager, Error, TEXT("Failed to call CreateCoreWebView2EnvironmentWithOptions. Error: 0x%08X"), hr);
        return false;
    }

    // Note: This is async, so initialization may not be complete immediately
    // We'll check IsInitialized() before using the environment
    // For synchronous approach, we could use a different method with loader DLL
    
    return true;
}

void FWebView2Manager::Shutdown()
{
    if (WebView2Environment)
    {
        WebView2Environment->Release();
        WebView2Environment = nullptr;
    }
    
    bInitialized = false;
}

#endif // MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2

