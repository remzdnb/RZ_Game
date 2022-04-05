// RemzDNB

#pragma once

#include "CoreMinimal.h"
#include "Core/RZ_GameMode.h"
#include "RZ_DeathmatchGameMode.generated.h"

UCLASS()
class RZ_GAME_API ARZ_DeathmatchGameMode : public ARZ_GameMode
{
	GENERATED_BODY()

	ARZ_DeathmatchGameMode();

	virtual void BeginPlay() override;
};
