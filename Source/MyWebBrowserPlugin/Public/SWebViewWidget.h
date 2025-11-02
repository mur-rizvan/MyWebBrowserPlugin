#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

#if MYWEBBROWSER_WINDOWS && MYWEBBROWSER_WEBVIEW2
// Forward declarations
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
    ICoreWebView2Controller* WebViewController;
    ICoreWebView2* WebView;
    HWND WebViewHWND;
#endif

    FString CurrentURL;
    FString InitialURL;
    bool bTransparencyEnabled;
    mutable FVector2D WidgetSize;
};

