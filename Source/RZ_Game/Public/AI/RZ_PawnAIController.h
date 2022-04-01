/// RemzDNB
///
/// RZ_PawnAIController.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
#include "AIController.h"
#include "RZ_PawnAIController.generated.h"

class ARZ_GameMode;
class URZ_GameSettings;
class ARZ_Character;

UCLASS()
class RZ_GAME_API ARZ_PawnAIController : public AAIController
{
	GENERATED_BODY()

public:

	ARZ_PawnAIController();

	void Init(APawn* NewTargetPawn, const FTransform& SpawnLocation);
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;

	//

	UFUNCTION()
	void ToggleAI(bool bNewIsEnabled);

	UFUNCTION()
	void UpdateActiveTarget();
	
	UFUNCTION()
	void SetFinalTargetActor(APawn* NewTargetPawn);

	//
	
	FORCEINLINE TArray<AActor*> GetPatrolPoints() const { return PatrolPoints; }
	FORCEINLINE TArray<AActor*> GetDetectedActors() const { return DetectedActors; }

private:

	ARZ_GameMode* GameMode;
	URZ_GameSettings* GameSettings;
	
	class UBehaviorTree* BehaviorTree;
	class UBehaviorTreeComponent* BehaviorTreeCT;
	class UBlackboardComponent* BlackboardCT;

	TWeakObjectPtr<class URZ_PerceptionComponent> PawnPerceptionComponent;

	TWeakObjectPtr<ARZ_Character> PossessedCharacter;
	TArray<AActor*> PatrolPoints;

	//

	UPROPERTY()
	FTransform PawnSpawnLocation;

	UPROPERTY()
	TArray<AActor*> DetectedActors;
	
	UPROPERTY()
	APawn* TargetPawn;

	UPROPERTY()
	FRotator TargetControlRotation; // TODO : implement this

	static AActor* ARZ_PawnAIController::GetClosestActorFromLocation(TArray<AActor*> ActorArray, FVector TargetLocation)
	{
		float MinDistance = 999999999999999999.0f;
		AActor* ResultActor = nullptr;

		for (const auto& Actor : ActorArray)
		{
			if (Actor)
			{
				const float NewDistance = FVector::Dist(Actor->GetActorLocation(), TargetLocation);
				if (NewDistance < MinDistance)
				{
					MinDistance = NewDistance;
					ResultActor = Actor;
				}
			}
		}
	
		return ResultActor;
	}
};
