/// RemzDNB
///
///	RZ_ItemManagerPluginInterfaces.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RZ_ItemManagerPluginInterfaces.generated.h"

/// Used to load editor data from a single DataAsset reference.
/// Must be implemented in project BaseGameState.

UINTERFACE(MinimalAPI)
class URZ_ItemManagerPluginSettingsInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_ITEMMANAGERPLUGIN_API IRZ_ItemManagerPluginSettingsInterface
{
	GENERATED_BODY()

public:

	virtual class URZ_ItemManagerPluginSettings* GetItemManagerPluginSettings() = 0;
};
