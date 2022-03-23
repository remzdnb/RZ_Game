#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RZ_GameMode.generated.h"

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

private:

	class ARZ_WorldSettings* WorldSettings;

	///// Controllers spawn handling
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UFUNCTION()
	void QueryRespawn(AController* NewController);
	
	UFUNCTION()
	FTransform QuerySpawnLocation();

	UFUNCTION()
	APawn* SpawnPawn(const FTransform& SpawnTransform);

private:

	UFUNCTION()
	ARZ_PawnStart* GetAvailablePawnStart();

	//
	
	TArray<TWeakObjectPtr<class ARZ_PawnStart>> PawnStarts;
	TArray<TWeakObjectPtr<class AController>> ReadyControllers;
	TArray<TWeakObjectPtr<class AVCAIController>> AIControllers;

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
