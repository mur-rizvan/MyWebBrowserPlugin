#pragma once

#include "Modules/ModuleManager.h"

class FMyWebBrowserPluginModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};

