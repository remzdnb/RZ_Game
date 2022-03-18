/// RemzDNB
///
///	RZ_UIManagerPluginInterfaces.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RZ_UIManagerPluginInterfaces.generated.h"

/// Used to load editor data from a single DataAsset reference.
/// Must be implemented in project BaseGameState.

UINTERFACE(MinimalAPI)
class URZ_UIManagerPluginSettingsInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_UIMANAGERPLUGIN_API IRZ_UIManagerPluginSettingsInterface
{
	GENERATED_BODY()

public:

	virtual class URZ_UIManagerPluginSettings* GetUIManagerPluginSettings() = 0;
};
