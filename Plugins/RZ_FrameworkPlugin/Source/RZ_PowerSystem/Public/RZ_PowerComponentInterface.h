/// RemzDNB

#pragma once

#include "RZ_PowerSystem.h"
#include "UObject/Interface.h"
#include "RZ_PowerComponentInterface.generated.h"

UINTERFACE(MinimalAPI)
class URZ_PowerComponentInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_POWERSYSTEM_API IRZ_PowerComponentInterface
{
	GENERATED_BODY()

	// Must be implemented by any PowerComponent owner.

public:
	
	virtual URZ_PowerComponent* GetPowerComponent() = 0;
	virtual void OnPowerStatusUpdated(bool bNewIsPowered) = 0;
};
