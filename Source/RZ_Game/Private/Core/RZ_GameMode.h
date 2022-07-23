#pragma once

#include "CoreMinimal.h"
#include "RZ_Game.h"
#include "GameFramework/GameModeBase.h"
#include "RZ_GameMode.generated.h"

class ARZ_GameState;
class URZ_GameSettings;
class ARZ_WorldSettings;
class ARZ_PawnStart;

UCLASS()
class RZ_GAME_API ARZ_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ARZ_GameMode();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:

	ARZ_GameState* GameState;
	URZ_GameSettings* GameSettings;
	ARZ_WorldSettings* WorldSettings;
	

	///// Controllers spawn handling
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UFUNCTION()
	void QueryRespawn(AController* NewController);
	
	UFUNCTION()
	FTransform QuerySpawnLocation();

	//

protected:

	UFUNCTION()
	ARZ_PawnStart* GetAvailablePawnStart();

	//

	TArray<TWeakObjectPtr<class AController>> ReadyControllers;
	TArray<TWeakObjectPtr<class ARZ_PawnStart>> PlayerPawnStarts;
	TArray<TWeakObjectPtr<class ARZ_PawnStart>> WaveAIPawnStarts;
	TArray<TWeakObjectPtr<class ARZ_PawnStart>> FreeAIPawnStarts;
	TArray<APawn*> PlayerCharacters;
	TArray<TWeakObjectPtr<class ARZ_Character>> AICharacters;
	
	UPROPERTY()
	uint8 PawnStartIndex;

	///// AI
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UFUNCTION()
	void AddAIController();

	UFUNCTION()
	void RemoveAIController();
};
