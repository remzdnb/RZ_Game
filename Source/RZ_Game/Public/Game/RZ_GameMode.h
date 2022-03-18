#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RZ_GameMode.generated.h"

class ARZ_SpawnManager;

UCLASS()
class RZ_GAME_API ARZ_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ARZ_GameMode();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	///// Pawn+Character spawn handling
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UFUNCTION()
	FTransform QuerySpawnLocation();

	UFUNCTION()
	APawn* SpawnPawn(TSubclassOf<APawn> PawnClass);

private:

	class ARZ_WorldSettings* WorldSettings;

	UPROPERTY()
	TArray<class ARZ_CharacterStart*> PawnStarts;

	UPROPERTY()
	uint8 PawnStartIndex;
	
};
