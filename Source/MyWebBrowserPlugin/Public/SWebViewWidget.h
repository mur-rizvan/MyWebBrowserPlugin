#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

// Windows/WebView2 declarations used only on Windows
#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2
    #include "Windows/WindowsHWrapper.h"

    // Safe forward declaration for HWND without including windows.h here
    struct HWND__;
    typedef HWND__* HWND;

    // Forward declarations for WebView2 interfaces (no heavy headers in .h)
    struct ICoreWebView2;
    struct ICoreWebView2Controller;
#endif

class SWebViewWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SWebViewWidget)
        : _InitialURL(TEXT("about:blank"))
        , _EnableTransparency(false)
    {}
        SLATE_ARGUMENT(FString, InitialURL)
        SLATE_ARGUMENT(bool, EnableTransparency)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

    virtual ~SWebViewWidget();

    // Load URL
    void LoadURL(const FString& URL);

    // Execute JavaScript
    void ExecuteJavaScript(const FString& Script);

    // Navigation
    void GoBack();
    void GoForward();
    void Reload();
    void StopLoad();

protected:
    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
        const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
        int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

    virtual FVector2D ComputeDesiredSize(float) const override;

private:
    void InitializeWebView();
    void CleanupWebView();
    void ResizeWebView();

#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2
    ICoreWebView2Controller* WebViewController = nullptr;
    ICoreWebView2* WebView = nullptr;
    HWND WebViewHWND = nullptr;
#endif

    FString CurrentURL;
    FString InitialURL;
    bool bTransparencyEnabled = false;
    mutable FVector2D WidgetSize = FVector2D(800.0f, 600.0f);
};