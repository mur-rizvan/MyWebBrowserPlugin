#include "WebViewWidget.h"
#include "Slate/SWebViewWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

UWebViewWidget::UWebViewWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , bEnableTransparency(false)
{
}

FString UWebViewWidget::GetCurrentURL() const
{
    return CurrentURL;
}

void UWebViewWidget::LoadURL(const FString& NewURL)
{
    CurrentURL = NewURL;
    if (WebViewWidget.IsValid())
    {
        WebViewWidget->LoadURL(NewURL);
    }
}

void UWebViewWidget::ExecuteJavaScript(const FString& Script)
{
    if (WebViewWidget.IsValid())
    {
        WebViewWidget->ExecuteJavaScript(Script);
    }
}

void UWebViewWidget::GoBack()
{
    if (WebViewWidget.IsValid())
    {
        WebViewWidget->GoBack();
    }
}

void UWebViewWidget::GoForward()
{
    if (WebViewWidget.IsValid())
    {
        WebViewWidget->GoForward();
    }
}

void UWebViewWidget::Reload()
{
    if (WebViewWidget.IsValid())
    {
        WebViewWidget->Reload();
    }
}

void UWebViewWidget::StopLoad()
{
    if (WebViewWidget.IsValid())
    {
        WebViewWidget->StopLoad();
    }
}

TSharedRef<SWidget> UWebViewWidget::RebuildWidget()
{
    WebViewWidget = SNew(SWebViewWidget)
        .InitialURL(InitialURL)
        .EnableTransparency(bEnableTransparency);

    return WebViewWidget.ToSharedRef();
}

void UWebViewWidget::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    WebViewWidget.Reset();
}

