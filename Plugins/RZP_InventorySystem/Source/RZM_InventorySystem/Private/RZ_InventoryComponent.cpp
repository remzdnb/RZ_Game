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

	SelectedQuickBarID = 0;
	SelectedSlotID = 0;
}

void URZ_InventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void URZ_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get editor data from interfaced GameInstance.
	
	SharedModuleSettings = Cast<IRZ_SharedModuleInterface>(GetWorld()->GetGameInstance())
		->GetSharedModuleSettings();
	InventorySystemModuleSettings = Cast<IRZ_InventorySystemModuleInterface>(GetWorld()->GetGameInstance())
		->GetInventorySystemModuleSettings();
	
	// Init slots.
 
	ItemSlots.SetNum(MAXINVENTORYSLOTS);
	for (uint8 Index = 0; Index < MAXINVENTORYSLOTS; Index++)
	{
		ItemSlots[Index].SlotID = Index;
		ItemSlots[Index].ItemName = "Empty";
	}

	//
	
	SelectSlot(0);
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
	
	const FRZ_ItemSettings* ItemSettings =
		SharedModuleSettings->GetItemSettingsFromTableRow(DataTableRowName);
	
	if (!ItemSettings) { return; }

	const int32 SlotIndex = GetFirstAvailableSlotIndex();
	ItemSlots[SlotIndex].ItemName = DataTableRowName;
	
	AActor* SpawnedItem = GetWorld()->SpawnActorDeferred<AActor>(
		ItemSettings->ItemClass,
		FTransform::Identity,
		GetOwner(),
		Cast<APawn>(GetOwner()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	if (SpawnedItem)
	{
		UGameplayStatics::FinishSpawningActor(SpawnedItem, FTransform::Identity);
		SelectItem(SpawnedItem, false);
		ItemSlots[SlotIndex].ItemActor = SpawnedItem;
	}

	OnItemAdded.Broadcast(SpawnedItem);
	OnInventoryUpdated.Broadcast();

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, *FString::Printf(TEXT("URZ_InventoryComponent::AddItem : %s"), *DataTableRowName.ToString()));
}

void URZ_InventoryComponent::SwapItems(uint8 SourceIndex, uint8 TargetIndex)
{
	if (!ItemSlots.IsValidIndex(SourceIndex)) { return; }
	if (!ItemSlots.IsValidIndex(TargetIndex)) { return; }

	const FRZ_InventorySlotInfo TempSlotSettings = ItemSlots[SourceIndex];
	ItemSlots[SourceIndex].ItemName = ItemSlots[TargetIndex].ItemName;
	ItemSlots[SourceIndex].ItemActor = ItemSlots[TargetIndex].ItemActor;
	ItemSlots[TargetIndex].ItemName = TempSlotSettings.ItemName;
	ItemSlots[TargetIndex].ItemActor = TempSlotSettings.ItemActor;

	OnInventoryUpdated.Broadcast();
}

void URZ_InventoryComponent::SelectSlot(int32 SlotID)
{
	if (SlotID == SelectedSlotID) { return; }
	if (!ItemSlots.IsValidIndex(SlotID)) { return; }
	if (ItemSlots[SlotID].ItemName == "Empty") { return; }
	
	SelectItem(ItemSlots[SelectedSlotID].ItemActor, false);
	SelectItem(ItemSlots[SlotID].ItemActor, true);

	SelectedSlotID = SlotID;
	SelectedItemInterface = Cast<IRZ_ItemInterface>(ItemSlots[SelectedSlotID].ItemActor);
	
	OnItemEquipped.Broadcast(ItemSlots[SelectedSlotID].ItemActor);
	OnInventoryUpdated.Broadcast();
}

void URZ_InventoryComponent::SelectItem(AActor* TargetActor, bool bNewIsSelected)
{
	if (!TargetActor) { return; }

	IRZ_ItemInterface* ItemInterface = Cast<IRZ_ItemInterface>(TargetActor);
	if (!ItemInterface) { return; }

	ItemInterface->OnSelectionUpdated(bNewIsSelected);
	ItemInterface->EnableBuildMode(true); // ? oué
	
	TargetActor->SetActorHiddenInGame(!bNewIsSelected);
	TargetActor->SetActorEnableCollision(bNewIsSelected);
	TargetActor->SetActorTickEnabled(bNewIsSelected);
}

void URZ_InventoryComponent::DropEquippedItemAtTargetLocation()
{
	if (!ItemSlots.IsValidIndex(SelectedSlotID)) { return; }
	if (!ItemSlots[SelectedSlotID].ItemActor) { return; }

	IRZ_ItemInterface* ItemInterface = Cast<IRZ_ItemInterface>(ItemSlots[SelectedSlotID].ItemActor);
	if (!ItemInterface) { return; }
	if (!ItemInterface->IsValidBuildLocation()) { return; }

	ItemInterface->EnableBuildMode(false);
	ItemSlots[SelectedSlotID].ItemName = "Empty";
	ItemSlots[SelectedSlotID].ItemActor = nullptr;
	
	//

	OnInventoryUpdated.Broadcast();
}

void URZ_InventoryComponent::SetWantToUseEquippedItem(bool bNewWantToUse)
{
	if (!SelectedItemInterface) { return; }

	SelectedItemInterface->SetWantToUse(bNewWantToUse);
}

void URZ_InventoryComponent::SelectQuickBar(int32 QuickBarID)
{
	SelectedQuickBarID = QuickBarID;
}

void URZ_InventoryComponent::SelectNextQuickBar(bool bIsNext)
{
	if (bIsNext)
	{
		SelectedQuickBarID + 1 >= MAXQUICKBARS ? SelectedQuickBarID = 0 : SelectedQuickBarID++;
	}
	else
	{
		SelectedQuickBarID - 1 < 0 ? SelectedQuickBarID = MAXQUICKBARS - 1 : SelectedQuickBarID--;
	}

	//UE_LOG(LogTemp, Warning, TEXT("URZ_InventoryComponent::SelectNextQuickBar - SelectedQuickBarID == %i" ), SelectedQuickBarID);

	OnQuickBarSelected.Broadcast();
	OnInventoryUpdated.Broadcast();
}

bool URZ_InventoryComponent::IsSlotOnSelectedQuickBar(int32 SlotID) const
{
	if (SlotID >= SelectedQuickBarID * MAXQUICKSLOTS && SlotID < (SelectedQuickBarID + 1) * MAXQUICKSLOTS )
		return true;

	return false;
}

void URZ_InventoryComponent::GetSlotsFromQuickBar(int32 QuickBarID, TArray<FRZ_InventorySlotInfo>& ResultArray) const
{
	for (int32 Index = QuickBarID * MAXQUICKSLOTS; Index < QuickBarID * MAXQUICKSLOTS + MAXQUICKSLOTS; Index++)
	{
		ResultArray.Add(ItemSlots[Index]);
	}
}

const FRZ_InventorySlotInfo& URZ_InventoryComponent::GetSlotInfo(int32 SlotID) const
{
	if (!ItemSlots.IsValidIndex(SlotID)) { return ItemSlots[0]; }

	return ItemSlots[SlotID];
}

bool URZ_InventoryComponent::IsAnyAvailableSlot() const
{
	bool bIsAnyAvailableSlot = false;
	for (const auto& InventorySlot : ItemSlots)
	{
		if (InventorySlot.ItemName == "Empty")
		{
			bIsAnyAvailableSlot = true;
		}
	}

	return bIsAnyAvailableSlot;
}

int32 URZ_InventoryComponent::GetFirstAvailableSlotIndex() const
{
	uint8 Index = 0;
	for (const auto& InventorySlot : ItemSlots)
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
	if (!SelectedItemInterface) { return; }

	const AActor* SelectedItem = ItemSlots[SelectedSlotID].ItemActor;
	if (!SelectedItem) { return; }

	FVector FinalSpawnLocation;
	if (InventorySystemModuleSettings->bSnapDemoActorToGrid)
	{
		FinalSpawnLocation = UKismetMathLibrary::Vector_SnappedToGrid(PlayerTargetLocation, 100.0f);
	}
	else
	{
		FinalSpawnLocation = PlayerTargetLocation;
	}

	const FVector LerpedItemLocation = UKismetMathLibrary::VLerp(
		SelectedItem->GetActorLocation(),
		FinalSpawnLocation,
		10.0f * GetWorld()->GetDeltaSeconds()
	);
	BuildItemMeshLocation = LerpedItemLocation;

	SelectedItemInterface->UpdateBuildModeLocation(FinalSpawnLocation, LerpedItemLocation);
	
}

void URZ_InventoryComponent::RotateBuildActor(bool bRotateRight)
{
	if (!ItemSlots[SelectedSlotID].ItemActor) { return; }

	float YawToAdd = 90.0f;
	if (bRotateRight) { YawToAdd *= -1; }
	ItemSlots[SelectedSlotID].ItemActor->AddActorLocalRotation(FRotator(0.0f, YawToAdd, 0.0f));
}
