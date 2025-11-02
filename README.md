# My Web Browser Plugin for UE 5.7

Высокопроизводительный плагин веб-браузера для Unreal Engine 5.7 на основе WebView2 (Windows).

## Особенности

- Использует Microsoft WebView2 (Edge Chromium) для максимальной производительности
- Поддержка множественных одновременных экземпляров (оптимизировано для 8+ HTML файлов)
- Интеграция с UMG для использования в Blueprint и C++
- Поддержка Windows 10/11

## Требования

- Unreal Engine 5.7
- Windows 10/11
- Visual Studio 2019 или новее (с компонентом Windows SDK 10.0.19041.0 или новее)
- Microsoft Edge WebView2 Runtime (устанавливается автоматически или вручную)
  - Скачать: https://developer.microsoft.com/en-us/microsoft-edge/webview2/

## Установка

1. Скопируйте папку `MyWebBrowserPlugin` в папку `Plugins` вашего проекта UE
2. Перезагрузите проект UE или выполните "Regenerate Visual Studio project files"
3. Убедитесь, что WebView2 Runtime установлен на системе

## Использование

### В Blueprint

1. Создайте Widget Blueprint
2. Добавьте виджет "Web View Widget"
3. Установите Initial URL в свойствах
4. Используйте функции Load URL, Execute JavaScript и другие

### В C++

```cpp
#include "WebViewWidget.h"

UWebViewWidget* WebView = CreateWidget<UWebViewWidget>(GetWorld());
WebView->LoadURL(TEXT("https://example.com"));
```

## Статус разработки

- [x] Базовая структура плагина
- [x] Интеграция WebView2
- [ ] Оптимизация для множественных экземпляров
- [ ] Тестирование производительности

## Лицензия

Проект доступен на GitHub: https://github.com/mur-rizvan/MyWebBrowserPlugin

