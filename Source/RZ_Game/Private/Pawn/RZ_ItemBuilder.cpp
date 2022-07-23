// RemzDNB

#include "Pawn/RZ_ItemBuilder.h"

#include "RZ_InventoryComponent.h"

ARZ_ItemBuilder::ARZ_ItemBuilder()
{
}

void ARZ_ItemBuilder::BeginPlay()
{
	Super::BeginPlay();

	InventoryComp->AddItemQuantityToStorage(ItemToBuildName, 1);
	InventoryComp->EquipStorageItem(1);

	IRZ_BuildableActorInterface* BuildableActorInterface = Cast<IRZ_BuildableActorInterface>(InventoryComp->GetStorageSlots()[1].SpawnedActor);
	if (BuildableActorInterface)
	{
		InventoryComp->GetStorageSlots()[1].SpawnedActor->SetActorLocation(
			GetActorLocation() + FVector(0.0f, 0.0f, BuildableActorInterface->GetBuildCenterZOffsetLocation()));
	}
	
	InventoryComp->SetWantsToUseEquippedStorageItem(true);
}

void ARZ_ItemBuilder::BuildItem()
{

}

