/// RemzDNB
///
///	RZ_ItemManager.cpp
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// ItemManagerPlugin
#include "RZ_ItemManager.h"
#include "RZ_ItemManagerPluginSettings.h"
/// ItemActorPlugin
#include "RZ_Item.h"
#include "RZ_ItemActorPluginInterfaces.h"
#include "RZ_ItemActorPluginSettings.h"
/// Engine
#include "GameFramework/GameStateBase.h"
#include "RZ_ItemManagerPluginInterfaces.h"
#include "Kismet/GameplayStatics.h"

URZ_ItemManager::URZ_ItemManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URZ_ItemManager::BeginPlay()
{
	Super::BeginPlay();

	/// Get settings data from interfaced GameInstance.
	
	ItemActorPluginSettings = Cast<IRZ_ItemActorPluginSettingsInterface>(GetOwner()->GetGameInstance())->GetItemActorPluginSettings();
	ItemManagerPluginSettings = Cast<IRZ_ItemManagerPluginSettingsInterface>(GetOwner()->GetGameInstance())->GetItemManagerPluginSettings();
	
	///
	
	uint8 Index = 0;
	AttachedSlots.SetNum(ItemManagerPluginSettings->AttachedItemSlotsConfigsDT->GetRowNames().Num());
	for (auto const RowName : ItemManagerPluginSettings->AttachedItemSlotsConfigsDT->GetRowNames())
	{
		/// Init slot
		
		AttachedSlots[Index] = *ItemManagerPluginSettings->GetAttachedItemSlotConfigFromRow(RowName);
		AttachedSlots[Index].SlotID = Index;

		/// Spawn default item

		const FRZ_SlotConfig* SlotConfig = ItemManagerPluginSettings->GetAttachedItemSlotConfigFromRow(RowName);
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

void URZ_ItemManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

ARZ_Item* URZ_ItemManager::GetEquippedItem() const
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

ARZ_Item* URZ_ItemManager::SpawnItem(const FName& ItemRowName)
{
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

	return nullptr;
}

void URZ_ItemManager::ReplaceItem(uint8 SlotIndex, const FName& ItemName)
{
	if (AttachedSlots.IsValidIndex(SlotIndex) == false)
		return;

	
}

void URZ_ItemManager::EquipItem(uint8 SlotIndex)
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


