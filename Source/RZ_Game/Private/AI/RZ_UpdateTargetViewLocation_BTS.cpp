/// RemzDNB

#include "AI/RZ_UpdateTargetViewLocation_BTS.h"
#include "AI/RZ_PawnAIController.h"
//
#include "BehaviorTree/BlackboardComponent.h"

URZ_UpdateTargetViewLocation_BTS::URZ_UpdateTargetViewLocation_BTS()
{
}

void URZ_UpdateTargetViewLocation_BTS::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void URZ_UpdateTargetViewLocation_BTS::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void URZ_UpdateTargetViewLocation_BTS::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// rename update from perception
	
	if (!OwnerComp.GetAIOwner()) { return; }
	if (!OwnerComp.GetAIOwner()->GetPawn()) { return; }
	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	if (!BlackboardComp) { return; }
	
	const AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TARGETACTOR_BBKEY));
	if (TargetActor)
	{
		BlackboardComp->SetValueAsVector(
			TARGETVIEWLOCATION_BBKEY, TargetActor->GetActorLocation()
		);
	}
	else
	{
		BlackboardComp->SetValueAsVector(
			TARGETVIEWLOCATION_BBKEY, FVector::ZeroVector//OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation()
		);
	}
	
	//UE_LOG(LogTemp, Error, TEXT("URZ_UpdateTargetViewLocation_BTS::TickNode /// SetCombatTargetActor = %s"), *GetNameSafe(ActiveTarget));
}
