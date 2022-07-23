/// RemzDNB
///
/// RZ_PawnAIController.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
#include "AIController.h"
#include "RZ_PawnAIController.generated.h"

class UBehaviorTreeComponent;

#define TARGETACTOR_BBKEY "TargetActor"
#define TARGETVIEWLOCATION_BBKEY "TargetViewLocation"
#define TARGETMOVELOCATION_BBKEY "TargetMoveLocation"
#define	HASATTACKRANGE_BBKEY "HasAttackRange"
#define HASATTACKANGLE_BBKEY "HasAttackAngle"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionListUpdatedDelegate);

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
	
	FActionListUpdatedDelegate OnActionListUpdated;

private:

	UPROPERTY()
	TArray<FRZ_AIAction> ActionList;

	FVector SmoothedViewTargetLocation;

	//
	
	ARZ_GameMode* GameMode;
	URZ_GameSettings* GameSettings;
	UBehaviorTree* BehaviorTree;
	UBehaviorTreeComponent* BehaviorTreeComp;
	UBlackboardComponent* BlackboardComp;

	//

	TWeakObjectPtr<ARZ_Character> PossessedCharacter;

public:

	UFUNCTION()
	void ToggleAI(bool bNewIsEnabled);

	UFUNCTION()
	void AddNewDelayedAction(FName ActionName, const FVector& TargetLocation, AActor* TargetActor);

	UFUNCTION()
	void ConsumeLatestAction();

	//

	UFUNCTION()
	void SetMoveToLocation(const FVector& NewMoveToLocation);
	
	//

	FORCEINLINE const TArray<FRZ_AIAction>& GetActionList() const { return ActionList; }
	FORCEINLINE TArray<AActor*> GetPatrolPoints() const { return PatrolPoints; }
	FORCEINLINE TArray<AActor*> GetDetectedActors() const { return DetectedActors; }

private:

	TWeakObjectPtr<class URZ_PerceptionComponent> PawnPerceptionComponent;
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

	UPROPERTY()
	FRotator InitialRotation;

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
