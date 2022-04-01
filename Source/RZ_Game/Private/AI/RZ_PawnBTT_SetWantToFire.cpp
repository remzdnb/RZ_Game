/// RemzDNB

#include "AI/RZ_PawnBTT_SetWantToFire.h"
//
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type URZ_PawnBTT_SetWantToFire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!OwnerComp.GetAIOwner()) { return EBTNodeResult::Failed; }

	IRZ_PawnInterface* OwnerPawnInterface = Cast<IRZ_PawnInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (!OwnerPawnInterface) { return EBTNodeResult::Failed; }

	OwnerPawnInterface->SetWantToFire(bWantToFire);

	return EBTNodeResult::Succeeded;
}
