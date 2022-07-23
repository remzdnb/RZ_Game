// RemzDNB

#pragma once

#include "CoreMinimal.h"
#include "RZ_Game.h"
#include "GameFramework/WorldSettings.h"
#include "RZ_WorldSettings.generated.h"

UCLASS()
class RZ_GAME_API ARZ_WorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_WorldSettings")
	ERZ_GameType DefaultGameType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_WorldSettings")
	bool bSpawnAsSpectator;
};
