#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RZ_GameInterfaces.generated.h"

UINTERFACE(MinimalAPI)
class URZ_InteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class IRZ_InteractionInterface
{
	GENERATED_BODY()

public:

	virtual void StartHover() = 0;
	virtual void StopHover() = 0;
	virtual void Use(class ARZ_PlayerController* InstigatorController) = 0;
};