#pragma once

#include "CoreMinimal.h"

class URZ_ButtonWidget;
class URZ_ComboBoxWidget;
class URZ_ProgressBarWidget;

class FRZ_BaseWidgetsModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};



