/// RemzDNB

#include "RZ_InventoryComponent.h"
#include "RZ_AttachedSlotComponent.h"
//
#include "RZ_CommonTags.h"
//
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

URZ_InventoryComponent::URZ_InventoryComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;

	EquippedStorageSlotID = 0;
	SelectedQuickBarID = 0;
}

void URZ_InventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (!GetWorld()->IsGameWorld()) { return; }

	MaxQuickBars = InventoryComponentSettings.MaxStorageSlots / InventoryComponentSettings.MaxQuickStorageSlots;
}

void URZ_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get editor data from interfaced GameInstance.
	
	InventorySystemSettings = Cast<IRZ_InventorySystemInterface>(GetWorld()->GetGameInstance())
		->GetInventorySystemSettings();
	
	// Init StorageSlots.
 
	StorageSlots.SetNum(InventoryComponentSettings.MaxStorageSlots + 1); // Add room for default item at SlotID = 0.
	for (uint8 Index = 0; Index < InventoryComponentSettings.MaxStorageSlots + 1; Index++)
	{
		StorageSlots[Index].InventorySlotSignature.OwnerInventoryComp = this;
		StorageSlots[Index].InventorySlotSignature.SlotID = Index;
		StorageSlots[Index].InventorySlotSignature.SlotType = ERZ_InventorySlotType::Storage;
		StorageSlots[Index].ActorName = EMPTYITEMSLOTNAME;
	}
	
	//AddStorageItem(true, nullptr, "DefaultWeapon");
	AddItemQuantityToStorage("Default", 1);
	EquipStorageItem(0);

	// Init AttachedSlots.

	int32 Index = 0;
	for (auto& AttachedSlot : AttachedSlots)
	{
		AttachedSlot.InventorySlotSignature.OwnerInventoryComp = this;
		AttachedSlot.InventorySlotSignature.SlotID = Index;
		AttachedSlot.InventorySlotSignature.SlotType = ERZ_InventorySlotType::Attached;
		Index++;
	}

	for (const auto& ItemName : InventoryComponentSettings.DefaultStorageItems)
	{
		AddItemQuantityToStorage(ItemName.Key, ItemName.Value);
	}
}

void URZ_InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	
	Debug(DeltaTime);
}

AActor* URZ_InventoryComponent::SpawnItemActor(const FName& ActorName, int32 StackSize)
{
	const FRZ_ItemSettings* ActorSettings = InventorySystemSettings->GetItemSettingsFromTableRow(ActorName);
	if (!ActorSettings) { return nullptr; }

	AActor* SpawnedActor = GetWorld()->SpawnActorDeferred<AActor>(
		ActorSettings->ActorClass,
		FTransform::Identity,
		GetOwner(),
		Cast<APawn>(GetOwner()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	if (SpawnedActor)
	{
		UAbilitySystemComponent* ASComp = Cast<UAbilitySystemComponent>(
			SpawnedActor->GetComponentByClass(UAbilitySystemComponent::StaticClass())
		);
		if (ASComp)
		{
			ASComp->AddLooseGameplayTag(FRZ_CommonTags::Get().GameActor_Enabled);
		}
		
		UGameplayStatics::FinishSpawningActor(SpawnedActor, FTransform::Identity);
	}

	return SpawnedActor;
}

void URZ_InventoryComponent::NewSlotOperation(URZ_InventoryComponent* SourceInventoryComp, uint8 SourceIndex,
                                              URZ_InventoryComponent* TargetInventoryComp, uint8 TargetIndex)
{
	if (!SourceInventoryComp) { return; }
	if (!SourceInventoryComp->GetStorageSlots().IsValidIndex(SourceIndex)) { return; }
	if (!TargetInventoryComp) { return; }
	if (!TargetInventoryComp->GetStorageSlots().IsValidIndex(TargetIndex)) { return; }
	
	/*IRZ_WorldInteractionInterface* SourceActorInterface = Cast<IRZ_WorldInteractionInterface>(
		SourceInventoryComp->GetStorageSlots()[SourceIndex].SpawnedActor
	);
	if (!SourceActorInterface) { return; }

	IRZ_WorldInteractionInterface* TargetActorInterface = Cast<IRZ_WorldInteractionInterface>(
		TargetInventoryComp->GetStorageSlots()[TargetIndex].SpawnedActor
	);*/

	// Merging

	/*if (SourceActorInterface && TargetActorInterface)
	{
		if (SourceActorInterface->GetActorName() == TargetActorInterface->GetActorName()
			&& SourceActorInterface->GetActorSettings().MaxStack > 1
			&& TargetActorInterface->GetStackSize() != TargetActorInterface->GetActorSettings().MaxStack)
		{
			const int32 MaxStack = SourceActorInterface->GetActorSettings().MaxStack;
			
			if (SourceActorInterface->GetStackSize() + TargetActorInterface->GetStackSize() <= MaxStack)
			{
				TargetActorInterface->SetStackSize(SourceActorInterface->GetStackSize() + TargetActorInterface->GetStackSize());
				SourceActorInterface->DestroyOwnerActor();
			}
			else
			{
				TargetActorInterface->SetStackSize(MaxStack);
				SourceActorInterface->SetStackSize(
					SourceActorInterface->GetStackSize() - (MaxStack - TargetActorInterface->GetStackSize())
				);
			}

			return;
		}
	}*/

	// Swap
	
	const FRZ_InventorySlotData TempSlotSettings = SourceInventoryComp->GetStorageSlots()[SourceIndex];
	SourceInventoryComp->StorageSlots[SourceIndex].SpawnedActor = TargetInventoryComp->StorageSlots[TargetIndex].SpawnedActor;
	TargetInventoryComp->StorageSlots[TargetIndex].SpawnedActor = TempSlotSettings.SpawnedActor;

	//

	//SourceInventoryComp->BroadcastInventoryUpdate();
	//TargetInventoryComp->BroadcastInventoryUpdate();
	
	// Debug.

	//if (InventorySystemSettings && InventorySystemSettings->bDebugInventoryComponent)
	//{
		const FString PrefixString = "URZ_InventoryComponent::NewSlotOperation : ";
		const FString SourceString = "Source == " + GetFullNameSafe(SourceInventoryComp) + " /// SourceID == " + FString::FromInt(SourceIndex);
		const FString TargetString = "Target == " + GetFullNameSafe(TargetInventoryComp) + " /// TargetID == " + FString::FromInt(TargetIndex);
		const FString EndString = "---------------------------------------";
		UE_LOG(LogTemp, Display, TEXT("%s"), *PrefixString);
		UE_LOG(LogTemp, Display, TEXT("%s"), *SourceString);
		UE_LOG(LogTemp, Display, TEXT("%s"), *TargetString);
		UE_LOG(LogTemp, Display, TEXT("%s"), *EndString);
	//}
}


#pragma region +++++ Storage slots ...

/*void URZ_InventoryComponent::AddActorToStorageSlot(int32 SlotID, const FName& ActorName, int32 StackSize, AActor* SpawnedActor)
{
	int32 TmpSlotID = SlotID;
	if (SlotID == -1)
	{
		TmpSlotID = GetFirstAvailableStorageSlotIndex();
		if (TmpSlotID == -1)
		{
			// Storage is full.
			return;
		}
	}
	
	if (!StorageSlots.IsValidIndex(TmpSlotID)) { return;}
	
	const FRZ_ItemSettings* ActorSettings = InventorySystemSettings->GetItemSettingsFromTableRow(ActorName);
	if (!ActorSettings) { return; }

	if (StorageSlots[TmpSlotID].ActorName != EMPTYITEMSLOTNAME) { return; }

	AActor* NewActor;
	SpawnedActor ? NewActor = SpawnedActor : NewActor = SpawnItemActor(ActorName, StackSize);
	
	StorageSlots[TmpSlotID].ActorName = ActorName;
	StorageSlots[TmpSlotID].SpawnedActor = NewActor;

	IRZ_ItemInterface* NewItemInterface = Cast<IRZ_ItemInterface>(NewActor);
	if (NewItemInterface)
	{
		NewItemInterface->OnAttachedToInventory(this);
	}

	OnInventoryUpdated.Broadcast();
}*/

void URZ_InventoryComponent::EquipStorageItem(int32 SlotID)
{
	//UE_LOG(LogTemp, Display, TEXT("URZ_InventoryComponent::EquipStorageItem - SlotID == %i"), SlotID);
	
	if (SlotID == EquippedStorageSlotID) { return; }
	if (!StorageSlots.IsValidIndex(SlotID)) { return; }
	//if (!StorageSlots[SlotID].SpawnedActor) { return; }

	const FRZ_ItemSettings* ItemSettings = InventorySystemSettings->GetItemSettingsFromTableRow(StorageSlots[SlotID].ActorName);
	if (ItemSettings)
	{
		if (!ItemSettings->bIsEquippable)
		{
			return;
		}
	}
	else
	{
		return;
	}

	//
	
	IRZ_InventoryItemInterface* LastInterface = Cast<IRZ_InventoryItemInterface>(StorageSlots[EquippedStorageSlotID].SpawnedActor);
	if (LastInterface)
		LastInterface->OnEquippedByInventory(false);

	IRZ_InventoryItemInterface* NewInterface = Cast<IRZ_InventoryItemInterface>(StorageSlots[SlotID].SpawnedActor);
	if (NewInterface)
		NewInterface->OnEquippedByInventory(true);

	EquippedStorageSlotID = SlotID;
	OnNewStorageItemEquipped.Broadcast(StorageSlots[SlotID]);
	OnInventoryUpdated.Broadcast();

	//UE_LOG(LogTemp, Display, TEXT(" URZ_InventoryComponent::SelectStorageItem end"));
}

void URZ_InventoryComponent::DropStorageItem(int32 SlotID)
{
	if (!StorageSlots.IsValidIndex(EquippedStorageSlotID)) { return; }
	if (!StorageSlots[EquippedStorageSlotID].SpawnedActor) { return; }
	
	//const IRZ_WorldInteractionInterface* ItemInterface = Cast<IRZ_WorldInteractionInterface>(StorageSlots[EquippedStorageSlotID].SpawnedActor);
	//if (!ItemInterface) { return; }
	
	StorageSlots[EquippedStorageSlotID].SpawnedActor = nullptr;
	
	SelectFirstSlotFromQuickBar();

	OnInventoryUpdated.Broadcast();

	if (InventorySystemSettings->bDebugInventoryComponent) {
		UE_LOG(LogTemp, Display, TEXT("URZ_InventoryComponent::DropSelectedSlot // SlotID == %i"), EquippedStorageSlotID);
	}
}

void URZ_InventoryComponent::SetWantsToUseEquippedStorageItem(bool bNewWantsToUse)
{
	IRZ_InventoryItemInterface* ItemInterface = Cast<IRZ_InventoryItemInterface>(
		StorageSlots[EquippedStorageSlotID].SpawnedActor
	);
	if (ItemInterface)
	{
		ItemInterface->OnWantToBeUsedByInventory(bNewWantsToUse);
	}

	//UE_LOG(LogTemp, Warning, TEXT("URZ_InventoryComponent::SetWantsToUseEquippedStorageItem %i"), bNewWantsToUse);
}

#pragma endregion

#pragma region +++++ Attachment slots ...

void URZ_InventoryComponent::AddActorToAttachmnentSlot(int32 SlotID, const FName& ActorName, int32 StackSize, AActor* SpawnedActor)
{
	if (!AttachedSlots.IsValidIndex(SlotID)) { return; }

	const FRZ_ItemSettings* ActorSettings = InventorySystemSettings->GetItemSettingsFromTableRow(ActorName);
	if (!ActorSettings) { return; }

	if (AttachedSlots[SlotID].SpawnedActor)
	{
		AttachedSlots[SlotID].SpawnedActor->Destroy();
	}

	AttachedSlots[SlotID].ActorName = ActorName;
	AActor* NewActor;
	SpawnedActor ? NewActor = SpawnedActor : NewActor = SpawnItemActor(ActorName, StackSize);

	AttachedSlots[SlotID].SpawnedActor = NewActor;

	OnInventoryUpdated.Broadcast();
}

#pragma endregion

#pragma region +++++ QuickBar ...


void URZ_InventoryComponent::SelectQuickBar(int32 QuickBarID)
{
	SelectedQuickBarID = QuickBarID;

	OnInventoryUpdated.Broadcast();
}

void URZ_InventoryComponent::SelectNextQuickBar(bool bIsNext)
{
	if (bIsNext)
	{
		SelectedQuickBarID + 1 >= MaxQuickBars ? SelectedQuickBarID = 0 : SelectedQuickBarID++;
	}
	else
	{
		SelectedQuickBarID - 1 < 0 ? SelectedQuickBarID = MaxQuickBars - 1 : SelectedQuickBarID--;
	}

	//UE_LOG(LogTemp, Warning, TEXT("URZ_InventoryComponent::SelectNextQuickBar - SelectedQuickBarID == %i" ), SelectedQuickBarID);
	
	OnInventoryUpdated.Broadcast();
}

bool URZ_InventoryComponent::IsSlotOnSelectedQuickBar(int32 SlotID) const
{
	if (SlotID > SelectedQuickBarID * InventoryComponentSettings.MaxQuickStorageSlots && SlotID <= (SelectedQuickBarID + 1) * InventoryComponentSettings.MaxQuickStorageSlots)
		return true;

	return false;
}

void URZ_InventoryComponent::SelectFirstSlotFromQuickBar()
{
	TArray<FRZ_InventorySlotData> SlotInfoArray;
	GetSlotsFromQuickBar(SelectedQuickBarID, SlotInfoArray);

	for (const auto& SlotInfo : SlotInfoArray)
	{
		if (SlotInfo.SpawnedActor)
		{
			EquipStorageItem(SlotInfo.InventorySlotSignature.SlotID);
		}
	}

	OnInventoryUpdated.Broadcast();
}

int32 URZ_InventoryComponent::GetQuickBarIDFromStorageSlotID(int32 StorageSlotID) const
{
	return StorageSlotID % InventoryComponentSettings.MaxQuickStorageSlots;
}

uint8 URZ_InventoryComponent::GetSlotIdOnQuickBar(int32 StorageSlotID) const
{
	//return GetQuickBarIDFromStorageSlotID() * InventoryComponentSettings.MaxQuickStorageSlots +
	return 1;
}

void URZ_InventoryComponent::GetSlotsFromQuickBar(int32 QuickBarID, TArray<FRZ_InventorySlotData>& ResultArray) const
{
	for (int32 Index = (QuickBarID * InventoryComponentSettings.MaxQuickStorageSlots) + 1; Index <= QuickBarID * InventoryComponentSettings.MaxQuickStorageSlots + InventoryComponentSettings.MaxQuickStorageSlots; Index++)
	{
		ResultArray.Add(StorageSlots[Index]);
	}
}

#pragma endregion

#pragma region +++++ Crafting ...

void URZ_InventoryComponent::StartCraftingItem(const FName& ItemToCraftName)
{
	if (CraftedItemName != "None") { return; }
	
	const FRZ_ItemSettings* ActorSettings = InventorySystemSettings->GetItemSettingsFromTableRow(ItemToCraftName);
	if (!ActorSettings) { return; }

	const TMap<FName, int32> Map = ActorSettings->CraftingComponents;
	if (HasItemsInStorage(Map))
	{
		for (const auto& ActorName : ActorSettings->CraftingComponents)
		{
			RemoveItemQuantityFromStorage(ActorName.Key, ActorName.Value);
		}

		CraftedItemName = ItemToCraftName;
		
		GetWorld()->GetTimerManager().SetTimer(
			CraftingTimer,
			this,
			&URZ_InventoryComponent::EndCraftingActor,
			ActorSettings->CraftingTime,
			false
		);
	}
}

void URZ_InventoryComponent::StopCraftingActor()
{
}

void URZ_InventoryComponent::EndCraftingActor()
{
	const FRZ_ItemSettings* ActorSettings = InventorySystemSettings->GetItemSettingsFromTableRow(CraftedItemName);
	if (!ActorSettings) { return; }
	
	AddItemQuantityToStorage(CraftedItemName, 1);
	CraftedItemName = "None";

	CraftingTimer.Invalidate();

	OnInventoryUpdated.Broadcast();
}

#pragma endregion

#pragma region +++++ Helpers ...

int32 URZ_InventoryComponent::GetFirstAvailableStorageSlotIndex() const
{
	uint8 Index = 0;
	for (const auto& StorageSlot : StorageSlots)
	{
		if (StorageSlot.ActorName == EMPTYITEMSLOTNAME)
		{
			return Index;
		}

		Index++;
	}

	return -1;
}

bool URZ_InventoryComponent::HasItemsInStorage(const TMap<FName, int32>& Items) const
{
	for (const TPair<FName, int32>& CraftingComp : Items)
	{
		if (GetItemQuantityInStorage(CraftingComp.Key) < CraftingComp.Value)
		{
			return false;
		}
	}
	
	return true;
}

int32 URZ_InventoryComponent::GetItemQuantityInStorage(const FName& ItemName) const
{
	int32 TempQuantity = 0;
	
	for (const auto& StorageSlot : StorageSlots)
	{
		if (StorageSlot.ActorName != EMPTYITEMSLOTNAME)
		{
			TempQuantity = TempQuantity + StorageSlot.StackSize;
		}
	}

	return TempQuantity;
}

void URZ_InventoryComponent::AddItemQuantityToStorage(const FName& ItemName, int32 QuantityToAdd)
{
	int32 RemainingQuantityToAdd = QuantityToAdd;
	
	const FRZ_ItemSettings* ItemSettings = InventorySystemSettings->GetItemSettingsFromTableRow(ItemName);
	if (!ItemSettings) { return; }

	// If stackable, browse through slots that have the same ActorName.
	
	if (ItemSettings->MaxStackSize > 1)
	{
		for (auto& StorageSlot : StorageSlots)
		{
			if (StorageSlot.ActorName == ItemName && StorageSlot.StackSize != ItemSettings->MaxStackSize)
			{
				int32 AddableQuantity = ItemSettings->MaxStackSize - StorageSlot.StackSize;
				
				if (RemainingQuantityToAdd <= AddableQuantity)
				{
					StorageSlot.StackSize = StorageSlot.StackSize + RemainingQuantityToAdd;
					return;
				}
				
				StorageSlot.StackSize = ItemSettings->MaxStackSize;
				RemainingQuantityToAdd = RemainingQuantityToAdd - AddableQuantity;
			}
		}
	}

	if (RemainingQuantityToAdd == 0) {return;}
	
	// When stacking options are exhausted, use free slots.

	const int32 SlotID = GetFirstAvailableStorageSlotIndex();
	if (SlotID == -1)
	{
		// Storage is full.
	}
	else
	{
		StorageSlots[SlotID].ActorName = ItemName;
		
		if (RemainingQuantityToAdd <= ItemSettings->MaxStackSize)
		{
			StorageSlots[SlotID].StackSize = RemainingQuantityToAdd;
		}
		else if (RemainingQuantityToAdd > ItemSettings->MaxStackSize)
		{
			StorageSlots[SlotID].StackSize = ItemSettings->MaxStackSize;
			AddItemQuantityToStorage(ItemName, RemainingQuantityToAdd - ItemSettings->MaxStackSize);
		}

		if (ItemSettings->ActorClass)
		{
			StorageSlots[SlotID].SpawnedActor = SpawnItemActor(ItemName, 1);
			StorageSlots[SlotID].SpawnedActor->SetOwner(GetOwner());
			IRZ_InventoryItemInterface* SpawnedItemInterface = Cast<IRZ_InventoryItemInterface>(StorageSlots[SlotID].SpawnedActor);
			if (SpawnedItemInterface)
			{
				SpawnedItemInterface->OnAttachedToInventory();
			}
			
			/*IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(StorageSlots[SlotID].SpawnedActor);
			if (TagInterface)
			{
				
			}*/
		}

		OnInventoryUpdated.Broadcast();
	}
}

void URZ_InventoryComponent::RemoveItemQuantityFromStorage(const FName& ItemName, int32 QuantityToRemove)
{
	int32 RemainingQuantityToRemove = QuantityToRemove;
	
	for (auto& StorageSlot : StorageSlots)
	{
		if (StorageSlot.ActorName == EMPTYITEMSLOTNAME) { continue; }
		
		if (StorageSlot.ActorName == ItemName)
		{
			if (StorageSlot.StackSize > RemainingQuantityToRemove)
			{
				StorageSlot.StackSize = StorageSlot.StackSize - RemainingQuantityToRemove;
				return;
			}

			if (StorageSlot.StackSize == RemainingQuantityToRemove)
			{
				StorageSlot.ActorName = EMPTYITEMSLOTNAME;
				return;
			}

			if (StorageSlot.StackSize < RemainingQuantityToRemove)
			{
				StorageSlot.ActorName = EMPTYITEMSLOTNAME;
				RemainingQuantityToRemove = RemainingQuantityToRemove - StorageSlot.StackSize;
				break;
			}
		}
	}

	OnInventoryUpdated.Broadcast();
}

void URZ_InventoryComponent::GetSlotDataFromSlotSignature(const FRZ_InventorySlotSignature& SlotSignature, FRZ_InventorySlotData& SlotData)
{
	FRZ_InventorySlotData InventorySlotData;

	if (SlotSignature.SlotType == ERZ_InventorySlotType::Storage)
	{
		SlotData = SlotSignature.OwnerInventoryComp->GetStorageSlots()[SlotSignature.SlotID];
	}
	else if (SlotSignature.SlotType == ERZ_InventorySlotType::Attached)
	{
		SlotData = SlotSignature.OwnerInventoryComp->GetAttachedSlots()[SlotSignature.SlotID];
	}
}

#pragma endregion

#pragma region +++++ Misc ...

void URZ_InventoryComponent::Debug(float DeltaTime)
{
	if (!Cast<ACharacter>(GetOwner())) { return; }
	if (!InventorySystemSettings->bDebugInventoryComponent) { return; }
	
	FString SlotInfoString;
	for (const auto& StorageSlot : StorageSlots)
	{
		if (StorageSlot.ActorName != EMPTYITEMSLOTNAME)
		{
			const FString String =
				"SlotActor :: SlotID == " + FString::FromInt(StorageSlot.InventorySlotSignature.SlotID) +
				" // ActorName == " + StorageSlot.ActorName.ToString();
			
			GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, String);
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, " ");

	const FString CraftedItemString = "CraftedItemName == " + CraftedItemName.ToString();
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, CraftedItemString);
	const FString EquippedStorageSlotIDString = "EquippedStorageSlotID == " + FString::FromInt(EquippedStorageSlotID);
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, EquippedStorageSlotIDString);
	
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, " ");
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, "--------------------------------------------------");
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, this->GetName());
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, "--------------------------------------------------");
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, " ");
}

#pragma endregion