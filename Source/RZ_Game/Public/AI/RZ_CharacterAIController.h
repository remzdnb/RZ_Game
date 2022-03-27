/// RemzDNB
///
/// RZ_CharacterAIController.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "RZ_CharacterAIController.generated.h"

class ARZ_GameMode;
class URZ_GameSettings;
class ARZ_Character;

UCLASS()
class RZ_GAME_API ARZ_CharacterAIController : public AAIController
{
	GENERATED_BODY()

public:

	ARZ_CharacterAIController();

	void Init(APawn* NewTargetPawn, const FTransform& SpawnLocation);
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	//

	UFUNCTION()
	void SetFinalTargetPawn(APawn* NewTargetPawn);

	//
	
	FORCEINLINE TArray<AActor*> GetPatrolPoints() const { return PatrolPoints; }
	FORCEINLINE TArray<AActor*> GetDetectedActors() const { return DetectedActors; }

private:

	UFUNCTION()
	void OnActorPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	ARZ_GameMode* GameMode;
	URZ_GameSettings* GameSettings;
	
	class UBehaviorTree* BehaviorTree;
	class UBehaviorTreeComponent* BehaviorTreeComp;
	class UBlackboardComponent* BlackboardComp;

	TWeakObjectPtr<ARZ_Character> PossessedCharacter;
	TArray<AActor*> PatrolPoints;

	//

	UPROPERTY()
	FTransform PawnSpawnLocation;

	UPROPERTY()
	TArray<AActor*> DetectedActors;
	
	UPROPERTY()
	APawn* TargetPawn;
};
