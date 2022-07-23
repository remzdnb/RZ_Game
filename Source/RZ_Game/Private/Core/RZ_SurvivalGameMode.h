/// RemzDNB
///
///	RZ_SurvivalGameMode.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Core/RZ_GameMode.h"
#include "RZ_SurvivalGameMode.generated.h"

UCLASS()
class RZ_GAME_API ARZ_SurvivalGameMode : public ARZ_GameMode
{
	GENERATED_BODY()

public:

	ARZ_SurvivalGameMode();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	
private:

	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void StopGame();

	UFUNCTION()
	void SpawnAIWave();

	//

	//TWeakObjectPtr<ARZ_Pawn> MainBuilding;

	FTimerHandle WaveTimer;
	
	UPROPERTY()
	uint8 WaveID;
	
	UPROPERTY()
	TArray<ARZ_Character*> WaveAICharacters;

	UPROPERTY()
	TArray<ARZ_Character*> FreeAICharacters;
};
