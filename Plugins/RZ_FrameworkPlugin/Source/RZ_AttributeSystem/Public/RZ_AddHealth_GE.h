// RemzDNB

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RZ_AddHealth_GE.generated.h"


UCLASS()
class RZ_ATTRIBUTESYSTEM_API URZ_AddHealth_GE : public UGameplayEffect
{
	GENERATED_BODY()

public:

	URZ_AddHealth_GE();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float HealthToAdd;
};
