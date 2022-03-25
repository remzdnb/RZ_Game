/// RemzDNB

// ItemManagerPlugin
#include "RZ_ItemManagerComponent.h"
// ItemActorPlugin
#include "RZM_ItemActor.h"
#include "RZ_Item.h"
// Engine
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

URZ_ItemManagerComponent::URZ_ItemManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URZ_ItemManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get editor data from interfaced GameInstance.

	ItemActorModuleSettings = Cast<IRZ_ItemActorModuleInterface>(GetOwner()->GetGameInstance())
		->GetItemActorModuleSettings();
	ItemManagerModuleSettings = Cast<IRZ_ItemManagerModuleInterface>(GetOwner()->GetGameInstance())
		->GetItemManagerModuleSettings();
	
	//
	
	uint8 Index = 0;
	AttachedSlots.SetNum(ItemManagerModuleSettings->ItemSlotsConfigDT->GetRowNames().Num());
	for (auto const RowName : ItemManagerModuleSettings->ItemSlotsConfigDT->GetRowNames())
	{
		// Init slot
		
		AttachedSlots[Index] = *ItemManagerModuleSettings->GetItemSlotConfigFromRow(RowName);
		AttachedSlots[Index].SlotID = Index;

		// Spawn default item

		const FRZ_ItemSlotSettings* SlotConfig = ItemManagerModuleSettings->GetItemSlotConfigFromRow(RowName);
		if (SlotConfig == nullptr)
			break;
		
		ARZ_Item* DefaultItem = SpawnItem(SlotConfig->DefaultItemName);
		if (DefaultItem)
		{
			AttachedSlots[Index].AttachedItem = DefaultItem;
			DefaultItem->SetActorHiddenInGame(true);
		}

		//
		
		Index++;
	}
}

void URZ_ItemManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

ARZ_Item* URZ_ItemManagerComponent::GetEquippedItem() const
{
	if (AttachedSlots.IsValidIndex(EquippedSlotIndex))
	{
		return AttachedSlots[EquippedSlotIndex].AttachedItem;
	}
	else
	{
		return nullptr;
	}
}

ARZ_Item* URZ_ItemManagerComponent::SpawnItem(const FName& ItemRowName)
{
	// delete old item
	
	const FTransform SpawnTransform = FTransform::Identity;
	const FRZ_ItemInfo* ItemData = ItemActorModuleSettings->GetItemInfoFromRow(ItemRowName);
	if (ItemData)
	{
		ARZ_Item* SpawnedItem = GetWorld()->SpawnActorDeferred<ARZ_Item>(
			ItemData->ItemClass,
			SpawnTransform,
			GetOwner(),
			nullptr
		);
		if (SpawnedItem)
		{
			//SpawnedItem->Init(DataManager, DataRowName);
			UGameplayStatics::FinishSpawningActor(SpawnedItem, SpawnTransform);
			OnItemSpawned.Broadcast(SpawnedItem);
			return SpawnedItem;
		}
	}

	// replace slot
	
	return nullptr;
}

void URZ_ItemManagerComponent::ReplaceItem(uint8 SlotIndex, const FName& ItemName)
{
	if (AttachedSlots.IsValidIndex(SlotIndex) == false)
		return;

	
}

void URZ_ItemManagerComponent::EquipItem(uint8 SlotIndex)
{
	if (AttachedSlots.IsValidIndex(SlotIndex) == false)
		return;
	
	ARZ_Item* ItemToEquip = AttachedSlots[SlotIndex].AttachedItem;
	if (ItemToEquip == nullptr)
		return;

	ARZ_Item* EquippedItem = AttachedSlots[EquippedSlotIndex].AttachedItem;
	if (EquippedItem)
	{
		EquippedItem->SetItemState(ERZ_ItemState::Holstered);
		EquippedItem->SetActorHiddenInGame(true);
	}

	EquippedItem->SetItemState(ERZ_ItemState::Ready);
	ItemToEquip->SetActorHiddenInGame(false);
	EquippedSlotIndex = SlotIndex;
	OnItemEquipped.Broadcast();
}


