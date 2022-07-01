/// RemzDNB

//
#include "RZ_InventoryComponent.h"
#include "RZ_AttachedSlotComponent.h"
//
#include "Kismet/GameplayStatics.h"

URZ_InventoryComponent::URZ_InventoryComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;

	SelectedSlotID = 0;
	SelectedQuickBarID = 0;
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
	InventorySystemSettings = Cast<IRZ_InventorySystemModuleInterface>(GetWorld()->GetGameInstance())
		->GetInventorySystemModuleSettings();
	
	// Init StorageSlots.
 
	StorageSlots.SetNum(MAXINVENTORYSLOTS);
	for (uint8 Index = 0; Index < MAXINVENTORYSLOTS; Index++)
	{
		StorageSlots[Index].SlotID = Index;
	}
	AddItemFromDataTable("DefaultWeapon");
	SelectSlot(0);

	// Init AttachedSlots.

	TInlineComponentArray<URZ_AttachedSlotComponent*> AttachedSlotComponents;
	GetOwner()->GetComponents(AttachedSlotComponents);
	for (const auto& AttachedSlotComponent : AttachedSlotComponents)
	{
		FRZ_InventorySlotInfo InventorySlotInfo;
		
		const FRZ_ActorSettings* ActorSettings =
			SharedModuleSettings->GetActorSettingsFromTableRow(AttachedSlotComponent->GetDefaultActorTableRowName());
		if (ActorSettings)
		{
			AttachedSlotComponent->SetChildActorClass(ActorSettings->ActorClass);
			AttachedSlotComponent->CreateChildActor();
		}

		const int32 NewSlotID = AttachedSlotComponents.Num();
		InventorySlotInfo.AttachedActor = AttachedSlotComponent->GetChildActor();
		InventorySlotInfo.SlotID = NewSlotID;
		AttachedSlots.Add(InventorySlotInfo);

		//InventorySlotInfo.
		/*AActor* SpawnedActor = GetWorld()->SpawnActorDeferred<AActor>(
	ActorSettings->ActorClass,
	FTransform::Identity,
	GetOwner(),
	Cast<APawn>(GetOwner()),
	ESpawnActorCollisionHandlingMethod::AlwaysSpawn
);
if (SpawnedActor)
{
	UGameplayStatics::FinishSpawningActor(SpawnedActor, FTransform::Identity);


	IRZ_InventoryActorInterface* Interface = Cast<IRZ_InventoryActorInterface>(SpawnedItem);
	if (Interface)
	{
		//Interface->OwnerInventory = this;
		Interface->OnAttachedToInventory();
	}
	//SelectItem(SpawnedItem, false);
	StorageSlots[SlotIndex].ItemActor = SpawnedItem;
}*/
	}
}

void URZ_InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* TickFunction)
{
	Super::TickComponent(DeltaTime, TickType, TickFunction);

	if (SelectedItemInterface)
	{
		SelectedItemInterface->SetPlayerTargetLocation(PlayerTargetLocation);
	}
	
	Debug(DeltaTime);
}

void URZ_InventoryComponent::AddItemFromDataTable(const FName& DataTableRowName)
{
	if (!IsAnyAvailableSlot()) { return; }
	if (!InventorySystemSettings) { return; }
	
	const FRZ_ActorSettings* ItemSettings = 
		SharedModuleSettings->GetActorSettingsFromTableRow(DataTableRowName);
	
	if (!ItemSettings) { return; }

	const int32 SlotIndex = GetFirstAvailableSlotIndex();
	
	AActor* SpawnedItem = GetWorld()->SpawnActorDeferred<AActor>(
		ItemSettings->ActorClass,
		FTransform::Identity,
		GetOwner(),
		Cast<APawn>(GetOwner()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	if (SpawnedItem)
	{
		UGameplayStatics::FinishSpawningActor(SpawnedItem, FTransform::Identity);
		IRZ_InventoryActorInterface* Interface = Cast<IRZ_InventoryActorInterface>(SpawnedItem);
		if (Interface)
		{
			Interface->OnAttachedToInventory(this);
		}
		//SelectItem(SpawnedItem, false);
		StorageSlots[SlotIndex].AttachedActor = SpawnedItem;
	}

	OnItemAdded.Broadcast(SpawnedItem);
	OnInventoryUpdated.Broadcast();
}

void URZ_InventoryComponent::AddItemFromWorld(AActor* ItemToAdd)
{
	IRZ_ActorInterface* ActorInterface = Cast<IRZ_ActorInterface>(ItemToAdd);
	if (!ActorInterface) { return; }

	IRZ_InventoryActorInterface* InventoryActorInterface = Cast<IRZ_InventoryActorInterface>(ItemToAdd);
	if (!InventoryActorInterface) { return; }

	if (!IsAnyAvailableSlot()) { return; }

	const int32 SlotIndex = GetFirstAvailableSlotIndex();
	StorageSlots[SlotIndex].AttachedActor = ItemToAdd;

	InventoryActorInterface->OnAttachedToInventory(this);
	//SelectItem(ItemToAdd, false);

	OnItemAdded.Broadcast(ItemToAdd);
	OnInventoryUpdated.Broadcast();
}

void URZ_InventoryComponent::SelectSlot(int32 SlotID)
{
	if (SlotID == SelectedSlotID) { return; }
	if (!StorageSlots.IsValidIndex(SlotID)) { return; }
	if (!StorageSlots[SlotID].AttachedActor) { return; }
	
	IRZ_InventoryActorInterface* LastInterface = Cast<IRZ_InventoryActorInterface>(StorageSlots[SelectedSlotID].AttachedActor);
	if (LastInterface)
		LastInterface->OnInventorySelection(false);

	IRZ_InventoryActorInterface* NewInterface = Cast<IRZ_InventoryActorInterface>(StorageSlots[SlotID].AttachedActor);
	if (NewInterface)
		NewInterface->OnInventorySelection(true);

	SelectedSlotID = SlotID;
	SelectedItemInterface = Cast<IRZ_ActorInterface>(StorageSlots[SelectedSlotID].AttachedActor);

	OnItemSelected.Broadcast(StorageSlots[SelectedSlotID].AttachedActor);
	OnInventoryUpdated.Broadcast();
}

void URZ_InventoryComponent::SwapItems(uint8 SourceIndex, uint8 TargetIndex)
{
	if (!StorageSlots.IsValidIndex(SourceIndex)) { return; }
	if (!StorageSlots.IsValidIndex(TargetIndex)) { return; }

	const FRZ_InventorySlotInfo TempSlotSettings = StorageSlots[SourceIndex];
	StorageSlots[SourceIndex].AttachedActor = StorageSlots[TargetIndex].AttachedActor;
	StorageSlots[TargetIndex].AttachedActor = TempSlotSettings.AttachedActor;

	OnInventoryUpdated.Broadcast();
}

void URZ_InventoryComponent::DropSelectedSlot()
{
	if (!StorageSlots.IsValidIndex(SelectedSlotID)) { return; }
	if (!StorageSlots[SelectedSlotID].AttachedActor) { return; }
	
	const IRZ_ActorInterface* ItemInterface = Cast<IRZ_ActorInterface>(StorageSlots[SelectedSlotID].AttachedActor);
	if (!ItemInterface) { return; }
	
	StorageSlots[SelectedSlotID].AttachedActor = nullptr;
	
	SelectFirstSlotFromQuickBar();

	OnInventoryUpdated.Broadcast();

	if (InventorySystemSettings->bDebugInventoryComponent) {
		UE_LOG(LogTemp, Display, TEXT("URZ_InventoryComponent::DropSelectedSlot // SlotID == %i"), SelectedSlotID);
	}
}

void URZ_InventoryComponent::DestroySelectedSlot()
{
}

void URZ_InventoryComponent::SetWantToUseEquippedItem(bool bNewWantToUse)
{
	if (!SelectedItemInterface) { return; }

	SelectedItemInterface->SetWantToUse(bNewWantToUse);
}

void URZ_InventoryComponent::SelectQuickBar(int32 QuickBarID)
{
	SelectedQuickBarID = QuickBarID;

	OnInventoryUpdated.Broadcast();
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
	
	OnInventoryUpdated.Broadcast();
}

bool URZ_InventoryComponent::IsSlotOnSelectedQuickBar(int32 SlotID) const
{
	if (SlotID > SelectedQuickBarID * MAXQUICKSLOTS && SlotID <= (SelectedQuickBarID + 1) * MAXQUICKSLOTS)
		return true;

	return false;
}

void URZ_InventoryComponent::SelectFirstSlotFromQuickBar()
{
	TArray<FRZ_InventorySlotInfo> SlotInfoArray;
	GetSlotsFromQuickBar(SelectedQuickBarID, SlotInfoArray);

	for (const auto& SlotInfo : SlotInfoArray)
	{
		if (SlotInfo.AttachedActor)
		{
			SelectSlot(SlotInfo.SlotID);
		}
	}

	OnInventoryUpdated.Broadcast();
}

void URZ_InventoryComponent::GetSlotsFromQuickBar(int32 QuickBarID, TArray<FRZ_InventorySlotInfo>& ResultArray) const
{
	for (int32 Index = (QuickBarID * MAXQUICKSLOTS) + 1; Index <= QuickBarID * MAXQUICKSLOTS + MAXQUICKSLOTS; Index++)
	{
		ResultArray.Add(StorageSlots[Index]);
	}
}

const FRZ_InventorySlotInfo& URZ_InventoryComponent::GetSlotInfo(int32 SlotID) const
{
	if (!StorageSlots.IsValidIndex(SlotID)) { return StorageSlots[0]; }

	return StorageSlots[SlotID];
}

bool URZ_InventoryComponent::IsAnyAvailableSlot() const
{
	bool bIsAnyAvailableSlot = false;
	for (const auto& InventorySlot : StorageSlots)
	{
		if (!InventorySlot.AttachedActor)
		{
			bIsAnyAvailableSlot = true;
		}
	}

	return bIsAnyAvailableSlot;
}

int32 URZ_InventoryComponent::GetFirstAvailableSlotIndex() const
{
	uint8 Index = 0;
	for (const auto& InventorySlot : StorageSlots)
	{
		if (!InventorySlot.AttachedActor)
		{
			return Index;
		}

		Index++;
	}

	return Index;
}

void URZ_InventoryComponent::Debug(float DeltaTime)
{
	if (!InventorySystemSettings) { return; }
	if (!InventorySystemSettings->bDebugInventoryComponent) { return; }

	const FString PrefixString = this->GetName() + " /// ";
	const FString SelectedSlotString = "SelectedSlotID == " + FString::FromInt(SelectedSlotID) + " /// ";
	const FString SelectedItemNameString = "";
	//const FString SelectedItemNameString = "SelectedItemName == " + StorageSlots[SelectedSlotID].ItemName.ToString() + " /// ";
	//const FString SelectedItemActor = "SelectedItemActor == " + ItemSlots[SelectedSlotID].ItemActor->GetName();
	const FString StringToPrint = PrefixString + SelectedSlotString + SelectedItemNameString;
	
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, StringToPrint);
}

void URZ_InventoryComponent::DebugSlot(const FRZ_InventorySlotInfo& SlotInfo)
{
	if (!InventorySystemSettings->bDebugInventoryComponent) { return; }

	const FString StartString = "///		" + this->GetName() + " / " + GetNameSafe(SlotInfo.AttachedActor) + "			///";
	UE_LOG(LogTemp, Display, TEXT("%s"), *StartString);

	if (SlotInfo.AttachedActor)
	{
		const FString InfoString = "Owner == " + GetNameSafe(SlotInfo.AttachedActor->GetOwner());
		UE_LOG(LogTemp, Display, TEXT("%s"), *InfoString);
	}
	
	const FString EndString = "//////////////////////////////////////////////";
	UE_LOG(LogTemp, Display, TEXT("%s"), *EndString);
}
