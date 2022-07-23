/// RemzDNB

#include "AI/RZ_UseEquippedItem_BTT.h"
//
#include "AIController.h"
#include "RZ_InventoryComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type URZ_UseEquippedItem_BTT::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!OwnerComp.GetAIOwner()) { return EBTNodeResult::Failed; }

	URZ_InventoryComponent* InventoryComp = Cast<URZ_InventoryComponent>(
		OwnerComp.GetAIOwner()->GetPawn()->GetComponentByClass(URZ_InventoryComponent::StaticClass())
	);
	if (InventoryComp)
	{
		InventoryComp->SetWantsToUseEquippedStorageItem(bWantToUse);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
