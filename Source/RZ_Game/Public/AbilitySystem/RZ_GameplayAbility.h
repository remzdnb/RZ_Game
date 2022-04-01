// RemzDNB

#pragma once

#include "CoreMinimal.h"
#include "RZ_Game.h"
#include "Abilities/GameplayAbility.h"
#include "RZ_GameplayAbility.generated.h"

UCLASS()
class RZ_GAME_API URZ_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	URZ_GameplayAbility();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ERZ_AbilityInputID AbilityInputID { ERZ_AbilityInputID::None };
};
