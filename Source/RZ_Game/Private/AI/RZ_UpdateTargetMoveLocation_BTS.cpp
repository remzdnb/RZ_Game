/// RemzDNB

#include "AI/RZ_UpdateTargetMoveLocation_BTS.h"
#include "AI/RZ_PawnAIController.h"
//
#include "BehaviorTree/BlackboardComponent.h"

URZ_UpdateTargetMoveLocation_BTS::URZ_UpdateTargetMoveLocation_BTS()
{
}

void URZ_UpdateTargetMoveLocation_BTS::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void URZ_UpdateTargetMoveLocation_BTS::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void URZ_UpdateTargetMoveLocation_BTS::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (!OwnerComp.GetAIOwner()) { return; }
	if (!OwnerComp.GetAIOwner()->GetPawn()) { return; }
}
