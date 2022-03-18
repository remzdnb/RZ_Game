// RemzDNB

#pragma once

#include "CoreMinimal.h"
#include "RZ_GameTypes.h"
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
	FName DefaultControlSettingsPresetName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_WorldSettings")
	TSubclassOf<APawn> DefaultPawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_WorldSettings")
	bool bSpawnAsSpectator;
};
