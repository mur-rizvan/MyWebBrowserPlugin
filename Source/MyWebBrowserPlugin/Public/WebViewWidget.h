#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "WebViewWidget.generated.h"

UCLASS()
class MYWEBBROWSERPLUGIN_API UWebViewWidget : public UWidget
{
    GENERATED_BODY()

public:
    UWebViewWidget(const FObjectInitializer& ObjectInitializer);

    // URL to load
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Web Browser")
    FString InitialURL;

    // Enable transparency
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Web Browser")
    bool bEnableTransparency;

    // Current URL
    UFUNCTION(BlueprintCallable, Category = "Web Browser")
    FString GetCurrentURL() const;

    // Load URL
    UFUNCTION(BlueprintCallable, Category = "Web Browser")
    void LoadURL(const FString& NewURL);

    // Execute JavaScript
    UFUNCTION(BlueprintCallable, Category = "Web Browser")
    void ExecuteJavaScript(const FString& Script);

    // Go back
    UFUNCTION(BlueprintCallable, Category = "Web Browser")
    void GoBack();

    // Go forward
    UFUNCTION(BlueprintCallable, Category = "Web Browser")
    void GoForward();

    // Reload
    UFUNCTION(BlueprintCallable, Category = "Web Browser")
    void Reload();

    // Stop loading
    UFUNCTION(BlueprintCallable, Category = "Web Browser")
    void StopLoad();

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
    TSharedPtr<class SWebViewWidget> WebViewWidget;
    
    FString CurrentURL;
};

