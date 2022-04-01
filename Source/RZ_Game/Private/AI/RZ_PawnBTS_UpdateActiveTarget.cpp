/// RemzDNB

#include "AI/RZ_PawnBTS_UpdateActiveTarget.h"
#include "AI/RZ_PawnAIController.h"

URZ_PawnBTS_UpdateActiveTarget::URZ_PawnBTS_UpdateActiveTarget()
{
}

void URZ_PawnBTS_UpdateActiveTarget::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void URZ_PawnBTS_UpdateActiveTarget::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void URZ_PawnBTS_UpdateActiveTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	ARZ_PawnAIController* PawnAIController = Cast<ARZ_PawnAIController>(OwnerComp.GetAIOwner());
	if (PawnAIController)
	{
		PawnAIController->UpdateActiveTarget();
	}
}
