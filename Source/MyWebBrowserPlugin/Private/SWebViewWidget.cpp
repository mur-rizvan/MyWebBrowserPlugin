#include "SWebViewWidget.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Logging/LogMacros.h"

#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2
#include "WebView2Manager.h"
#include "Windows/WindowsHWrapper.h"
#include "HAL/PlatformApplicationMisc.h"
#include "ApplicationCore/Public/Windows/WindowsWindow.h"
#include "SlateCore/Public/Widgets/Window/SWindow.h"
#include "Slate/Public/Framework/Application/SlateApplication.h"
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <WebView2.h>
#include <wrl.h>
#include <comdef.h>

using namespace Microsoft::WRL;
#endif

DEFINE_LOG_CATEGORY_STATIC(LogWebView, Log, All);

SWebViewWidget::SWebViewWidget()
#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2
    : WebViewController(nullptr)
    , WebView(nullptr)
    , WebViewHWND(nullptr)
#endif
    , bTransparencyEnabled(false)
    , WidgetSize(800.0f, 600.0f)
{
}

SWebViewWidget::~SWebViewWidget()
{
    CleanupWebView();
}

void SWebViewWidget::Construct(const FArguments& InArgs)
{
    CurrentURL = InArgs._InitialURL;
    bTransparencyEnabled = InArgs._EnableTransparency;
    InitialURL = InArgs._InitialURL;

    ChildSlot
    [
        SNew(SBox)
        .WidthOverride(WidgetSize.X)
        .HeightOverride(WidgetSize.Y)
        [
            SNew(STextBlock)
            .Text(FText::FromString(TEXT("WebView2 Initializing...")))
        ]
    ];

    InitializeWebView();
}

void SWebViewWidget::LoadURL(const FString& URL)
{
    CurrentURL = URL;

#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2
    if (WebView)
    {
        HRESULT hr = WebView->Navigate(*URL);
        if (FAILED(hr))
        {
            UE_LOG(LogWebView, Warning, TEXT("Failed to navigate to URL: %s"), *URL);
        }
    }
#endif
}

void SWebViewWidget::ExecuteJavaScript(const FString& Script)
{
#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2
    if (WebView)
    {
        HRESULT hr = WebView->ExecuteScript(*Script, nullptr);
        if (FAILED(hr))
        {
            UE_LOG(LogWebView, Warning, TEXT("Failed to execute JavaScript"));
        }
    }
#endif
}

void SWebViewWidget::GoBack()
{
#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2
    if (WebView)
    {
        WebView->GoBack();
    }
#endif
}

void SWebViewWidget::GoForward()
{
#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2
    if (WebView)
    {
        WebView->GoForward();
    }
#endif
}

void SWebViewWidget::Reload()
{
#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2
    if (WebView)
    {
        WebView->Reload();
    }
#endif
}

void SWebViewWidget::StopLoad()
{
#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2
    if (WebView)
    {
        WebView->Stop();
    }
#endif
}

void SWebViewWidget::InitializeWebView()
{
#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2
    UE_LOG(LogWebView, Log, TEXT("Initializing WebView2..."));

    // Get WebView2 manager and ensure it's initialized
    FWebView2Manager& Manager = FWebView2Manager::Get();
    if (!Manager.Initialize())
    {
        UE_LOG(LogWebView, Error, TEXT("Failed to initialize WebView2 manager"));
        return;
    }

    ICoreWebView2Environment* Environment = Manager.GetEnvironment();
    if (!Environment)
    {
        UE_LOG(LogWebView, Warning, TEXT("WebView2 environment not ready yet, will retry..."));
        // Retry later - environment initialization is async
        return;
    }

    // Get parent window handle
    TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
    if (!ParentWindow.IsValid())
    {
        // Try to get any window
        const TArray<TSharedRef<SWindow>>& Windows = FSlateApplication::Get().GetAllWindows();
        if (Windows.Num() > 0)
        {
            ParentWindow = Windows[0];
        }
    }

    if (!ParentWindow.IsValid())
    {
        UE_LOG(LogWebView, Error, TEXT("No parent window found for WebView2"));
        return;
    }

    HWND ParentHWND = (HWND)ParentWindow->GetNativeWindow()->GetOSWindowHandle();
    if (!ParentHWND)
    {
        UE_LOG(LogWebView, Error, TEXT("Failed to get parent window handle"));
        return;
    }

    // Create child window for WebView2
    // Note: WebView2 needs its own HWND, we'll create a child window
    HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
    
    WebViewHWND = CreateWindowEx(
        WS_EX_NOACTIVATE,
        TEXT("STATIC"),
        TEXT("WebView2Host"),
        WS_CHILD | WS_VISIBLE,
        0, 0,
        (int)WidgetSize.X, (int)WidgetSize.Y,
        ParentHWND,
        NULL,
        hInstance,
        NULL
    );

    if (!WebViewHWND)
    {
        UE_LOG(LogWebView, Error, TEXT("Failed to create WebView2 host window"));
        return;
    }

    // Create WebView2 controller and webview
    Environment->CreateCoreWebView2Controller(
        WebViewHWND,
        Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
            [this](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT
            {
                if (SUCCEEDED(result))
                {
                    WebViewController = controller;
                    if (WebViewController)
                    {
                        WebViewController->AddRef();

                        // Get WebView from controller
                        WebViewController->get_CoreWebView2(&WebView);
                        if (WebView)
                        {
                            WebView->AddRef();

                            // Set transparency if enabled
                            if (bTransparencyEnabled)
                            {
                                WebViewController->put_IsVisible(TRUE);
                                WebView->put_DefaultBackgroundColor(COREWEBVIEW2_COLOR{0, 0, 0, 0}); // Transparent
                            }

                            // Set initial bounds
                            ResizeWebView();

                            // Load initial URL if set
                            if (!CurrentURL.IsEmpty())
                            {
                                LoadURL(CurrentURL);
                            }
                            else if (!InitialURL.IsEmpty())
                            {
                                LoadURL(InitialURL);
                            }

                            UE_LOG(LogWebView, Log, TEXT("WebView2 initialized successfully"));
                        }
                    }
                }
                else
                {
                    UE_LOG(LogWebView, Error, TEXT("Failed to create WebView2 controller. Error: 0x%08X"), result);
                }
                return S_OK;
            }).Get());

#endif
}

void SWebViewWidget::CleanupWebView()
{
#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2
    if (WebViewController)
    {
        WebViewController->Close();
        WebViewController->Release();
        WebViewController = nullptr;
    }
    
    if (WebView)
    {
        WebView->Release();
        WebView = nullptr;
    }
    
    if (WebViewHWND)
    {
        DestroyWindow(WebViewHWND);
        WebViewHWND = nullptr;
    }
#endif
}

void SWebViewWidget::ResizeWebView()
{
#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2
    if (WebViewController && WebViewHWND)
    {
        RECT bounds;
        GetClientRect(WebViewHWND, &bounds);
        WebViewController->put_Bounds(bounds);
    }
    else if (WebViewController)
    {
        // Resize based on widget size
        RECT bounds = { 0, 0, (LONG)WidgetSize.X, (LONG)WidgetSize.Y };
        WebViewController->put_Bounds(bounds);
    }
#endif
}

int32 SWebViewWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
    const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
    int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    WidgetSize = AllottedGeometry.GetLocalSize();
    
    // Resize WebView to match widget size
    const_cast<SWebViewWidget*>(this)->ResizeWebView();
    
    return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, 
        OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

FVector2D SWebViewWidget::ComputeDesiredSize(float) const
{
    return WidgetSize;
}

