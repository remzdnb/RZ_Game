/// RemzDNB

// InventorySystem plugin
#include "RZ_InventoryComponent.h"
// Engine
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

URZ_InventoryComponent::URZ_InventoryComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;

	//

	QuickSlotsIndex = 1;
}

void URZ_InventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void URZ_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get editor data from interfaced GameInstance.
	
	InventorySystemModuleSettings = Cast<IRZ_InventorySystemModuleInterface>(GetWorld()->GetGameInstance())
		->GetInventorySystemModuleSettings();
	
	// Init slots.

	InventorySlots.SetNum(MAXINVENTORYSLOTS);
	for (uint8 Index = 0; Index < MAXINVENTORYSLOTS; Index++)
	{
		InventorySlots[Index].SlotID = Index;
		InventorySlots[Index].ItemName = "Empty";
	}

	//
	
	EquipItemSlot(0);
}

void URZ_InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* TickFunction)
{
	Super::TickComponent(DeltaTime, TickType, TickFunction);
	
	UpdateDemoActorLocation();
}

void URZ_InventoryComponent::AddItem(const FName& DataTableRowName)
{
	if (!IsAnyAvailableSlot()) { return; }
	if (!InventorySystemModuleSettings) { return; }
	
	const FRZ_InventoryItemSettings* ItemData = InventorySystemModuleSettings->GetInventoryItemSettingsFromDataTable(DataTableRowName);
	if (!ItemData) { return; }

	const uint8 SlotIndex = GetFirstAvailableSlotIndex();
	InventorySlots[SlotIndex].ItemName = DataTableRowName; // no actor spawn ?
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, *FString::Printf(TEXT("URZ_InventoryComponent::AddItem : %s"), *DataTableRowName.ToString()));
	
	const FTransform SpawnTransform = FTransform::Identity;
	AActor* SpawnedItem = GetWorld()->SpawnActorDeferred<AActor>(
		ItemData->ItemClass,
		SpawnTransform,
		GetOwner(),
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	if (SpawnedItem)
	{
		//SpawnedItem->Init(DataManager, DataRowName);
		UGameplayStatics::FinishSpawningActor(SpawnedItem, SpawnTransform);
		InventorySlots[SlotIndex].ItemActor = SpawnedItem;
		SpawnedItem->SetActorHiddenInGame(true);
	}

	OnItemAdded.Broadcast(SpawnedItem);
	OnInventoryUpdated.Broadcast();
}

void URZ_InventoryComponent::ReplaceItem(uint8 SlotIndex, const FName& ItemName)
{
	//if (InventorySlots.IsValidIndex(SlotIndex) == false)
		//return;
}

void URZ_InventoryComponent::SwapItems(uint8 SourceIndex, uint8 TargetIndex)
{
	if (!InventorySlots.IsValidIndex(SourceIndex)) { return; }
	if (!InventorySlots.IsValidIndex(TargetIndex)) { return; }

	const FRZ_InventorySlotSettings TempSlotSettings = InventorySlots[SourceIndex];
	InventorySlots[SourceIndex].ItemName = InventorySlots[TargetIndex].ItemName;
	InventorySlots[SourceIndex].ItemActor = InventorySlots[TargetIndex].ItemActor;
	InventorySlots[TargetIndex].ItemName = TempSlotSettings.ItemName;
	InventorySlots[TargetIndex].ItemActor = TempSlotSettings.ItemActor;

	OnInventoryUpdated.Broadcast();
}

void URZ_InventoryComponent::EquipItemSlot(uint8 SlotIndex)
{
	if (!InventorySlots.IsValidIndex(SlotIndex)) { return; }
	if (InventorySlots[SlotIndex].ItemName == "Empty") { return; }
	
	if (InventorySlots[EquippedSlotIndex].ItemActor)
	{
		InventorySlots[EquippedSlotIndex].ItemActor->SetActorHiddenInGame(true);
	}

	if (InventorySlots[SlotIndex].ItemActor)
	{
		InventorySlots[SlotIndex].ItemActor->SetActorHiddenInGame(false);
		EquippedItemInterface = Cast<IRZ_ItemActorInterface>(InventorySlots[SlotIndex].ItemActor);
	}

	EquippedSlotIndex = SlotIndex;

	/*const FRZ_InventoryItemSettings ItemSettings = EquippedItemInterface->GetItemSettings();
	if (ItemSettings.Type == ERZ_ItemType::Building)
	{
		//EquippedItem->SetItemState(ERZ_ItemState::Ready);

			ARZ_Item* EquippedItem = InventorySlots[EquippedSlotIndex].AttachedItem;
			if (EquippedItem)
			{
				EquippedItem->SetItemState(ERZ_ItemState::Holstered);
				EquippedItem->SetActorHiddenInGame(true);
			}
	}
		*/
	OnItemEquipped.Broadcast(InventorySlots[EquippedSlotIndex].ItemActor);
}

void URZ_InventoryComponent::DropEquippedItemAtTargetLocation()
{
	if (!InventorySlots.IsValidIndex(EquippedSlotIndex)) { return; }
	if (!InventorySlots[EquippedSlotIndex].ItemActor) { return; }

	IRZ_ItemActorInterface* ItemInterface = Cast<IRZ_ItemActorInterface>(InventorySlots[EquippedSlotIndex].ItemActor);
	if (!ItemInterface) { return; }

	// if building only

	ItemInterface->ToggleDemoMode(false);
	InventorySlots[EquippedSlotIndex].ItemName = "Empty";
	InventorySlots[EquippedSlotIndex].ItemActor = nullptr;
	
	//

	OnInventoryUpdated.Broadcast();
}

void URZ_InventoryComponent::SetWantToUseEquippedItem(bool bNewWantToUse)
{
	if (!EquippedItemInterface) { return; }

	EquippedItemInterface->SetWantsToUse(bNewWantToUse);
}

bool URZ_InventoryComponent::IsAnyAvailableSlot() const
{
	bool bIsAnyAvailableSlot = false;
	for (const auto& InventorySlot : InventorySlots)
	{
		if (InventorySlot.ItemName == "Empty")
		{
			bIsAnyAvailableSlot = true;
		}
	}

	return bIsAnyAvailableSlot;
}

uint8 URZ_InventoryComponent::GetFirstAvailableSlotIndex() const
{
	uint8 Index = 0;
	for (const auto& InventorySlot : InventorySlots)
	{
		if (InventorySlot.ItemName == "Empty")
		{
			return Index;
		}

		Index++;
	}

	return Index;
}

void URZ_InventoryComponent::UpdateDemoActorLocation()
{
	if (!EquippedItemInterface) { return; }
	if (!InventorySlots[EquippedSlotIndex].ItemActor) { return; }

	// dont do that every frame
	const FRZ_InventoryItemSettings ItemSettings = EquippedItemInterface->GetItemSettings();
	if (ItemSettings.Type == ERZ_ItemType::Building)
	{
		if (InventorySystemModuleSettings->bSnapDemoActorToGrid)
		{
			const FVector SnappedLocation = UKismetMathLibrary::Vector_SnappedToGrid(PlayerTargetLocation, 100.0f);
			InventorySlots[EquippedSlotIndex].ItemActor->SetActorLocation(SnappedLocation);
		}
		else
		{
			InventorySlots[EquippedSlotIndex].ItemActor->SetActorLocation(PlayerTargetLocation);
		}
	}
}
