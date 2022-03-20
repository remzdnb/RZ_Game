/// RemzDNB
///
///	RZ_ItemManagerComponent.cpp
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// ItemManagerPlugin
#include "RZ_ItemManagerComponent.h"
#include "RZ_ItemManagerComponent.h"
/// ItemActorPlugin
#include "RZ_Item.h"
#include "RZM_ItemActor.h"
#include "RZM_ItemActor.h"
/// Engine
#include "GameFramework/GameStateBase.h"
#include "RZ_ItemManagerComponent.h"
#include "Kismet/GameplayStatics.h"

URZ_ItemManagerComponent::URZ_ItemManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URZ_ItemManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	/// Get editor data from interfaced GameInstance.

	ItemActorPluginSettings = Cast<IRZ_ItemActorEditorSettingsInterface>(GetOwner()->GetGameInstance())
		->GetItemActorEditorSettings();
	ItemManagerEditorSettings = Cast<IRZ_ItemManagerEditorSettingsInterface>(GetOwner()->GetGameInstance())
		->GetItemManagerEditorSettings();
	
	///
	
	uint8 Index = 0;
	AttachedSlots.SetNum(ItemManagerEditorSettings->AttachedItemSlotsConfigsDT->GetRowNames().Num());
	for (auto const RowName : ItemManagerEditorSettings->AttachedItemSlotsConfigsDT->GetRowNames())
	{
		/// Init slot
		
		AttachedSlots[Index] = *ItemManagerEditorSettings->GetAttachedItemSlotConfigFromRow(RowName);
		AttachedSlots[Index].SlotID = Index;

		/// Spawn default item

		const FRZ_ItemSlotSettings* SlotConfig = ItemManagerEditorSettings->GetAttachedItemSlotConfigFromRow(RowName);
		if (SlotConfig == nullptr)
			break;
		
		ARZ_Item* DefaultItem = SpawnItem(SlotConfig->DefaultItemName);
		if (DefaultItem)
		{
			AttachedSlots[Index].AttachedItem = DefaultItem;
			DefaultItem->SetActorHiddenInGame(true);
		}

		///
		
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
	const FRZ_ItemData* ItemData = ItemActorPluginSettings->GetItemDataFromRow(ItemRowName);
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

