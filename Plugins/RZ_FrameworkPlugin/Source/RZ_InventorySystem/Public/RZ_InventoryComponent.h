/// RemzDNB
///
///	RZ_InventoryComponent.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_BaseFramework.h"
#include "RZ_InventorySystem.h"
#include "Components/ActorComponent.h"
#include "RZ_InventoryComponent.generated.h"

#define EMPTYITEMSLOTNAME "None"
#define MAXINVENTORYSLOTS 46
#define MAXQUICKSLOTS 9
#define MAXQUICKBARS (MAXINVENTORYSLOTS / MAXQUICKSLOTS) // % ?

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdatedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewTargetInventoryDelegate, const URZ_InventoryComponent*, NewTargetInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewStorageItemEquippedDelegate, const FRZ_InventorySlotData&, SlotData);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RZ_INVENTORYSYSTEM_API URZ_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	URZ_InventoryComponent();

	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunction) override;

protected:
	
	/// InventoryComponent.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	
	FInventoryUpdatedDelegate OnInventoryUpdated;
	FNewTargetInventoryDelegate OnNewTargetInventory;
	FNewStorageItemEquippedDelegate OnNewStorageItemEquipped;

	//

	UPROPERTY() // Must be updated from the owner.
	FVector PlayerTargetLocation;

private:
	
	//

	const URZ_InventorySystemSettings* InventorySystemSettings;
	TWeakObjectPtr<URZ_InventoryComponent> TargetInventory;
	
public:
	
	FORCEINLINE URZ_InventoryComponent* GetTargetInventory() const { return TargetInventory.Get(); }
	FORCEINLINE void SetTargetInventory(URZ_InventoryComponent* NewTargetInventory)
	{
		TargetInventory = NewTargetInventory;
		OnInventoryUpdated.Broadcast();
		OnNewTargetInventory.Broadcast(NewTargetInventory);
	}

	//
	
	UFUNCTION()
	static void NewSlotOperation(URZ_InventoryComponent* SourceInventoryComp, uint8 SourceIndex,
								 URZ_InventoryComponent* TargetInventoryComp, uint8 TargetIndex);

	UFUNCTION()
	AActor* SpawnItemActor(const FName& ActorName, int32 StackSize);

	/// Storage slots.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:

	UPROPERTY() // Inactive + hidden if unequipped, equipped to hands if selected.
	TArray<FRZ_InventorySlotData> StorageSlots;

	UPROPERTY()
	int32 EquippedStorageSlotID;
	
public:

	//UFUNCTION()
	//void AddActorToStorageSlot(int32 SlotID, const FName& ActorName, int32 StackSize = 1, AActor* SpawnedActor = nullptr);
	
	UFUNCTION()
	void EquipStorageItem(int32 SlotID);
	
	UFUNCTION()
	void DropStorageItem(int32 SlotID);

	//

	UFUNCTION() // Returns -1 if no available slot was found.
	int32 GetFirstAvailableStorageSlotIndex() const;
	
	UFUNCTION()
	bool HasItemsInStorage(const TMap<FName, int32>& Items) const;
	
	UFUNCTION()
	int32 GetItemQuantityInStorage(const FName& ItemName) const;

	UFUNCTION()
	void AddItemQuantityToStorage(const FName& ItemName, int32 QuantityToAdd);

	UFUNCTION()
	void RemoveItemQuantityFromStorage(const FName& ItemName, int32 QuantityToRemove);
	
	//

	FORCEINLINE const TArray<FRZ_InventorySlotData>& GetStorageSlots() const { return StorageSlots; }
	FORCEINLINE int32 GetEquippedStorageSlotID() const { return EquippedStorageSlotID; }

	/// Attached slots.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) // Attached to owner mesh, always visible (ammo, attachments, gear...).
	TArray<FRZ_InventorySlotData> AttachmentSlots;
	
public:

	UFUNCTION()
	void AddActorToAttachmnentSlot(int32 SlotID, const FName& ActorName, int32 StackSize = 1, AActor* SpawnedActor = nullptr);

	FORCEINLINE const TArray<FRZ_InventorySlotData>& GetAttachedSlots() const { return AttachmentSlots; }
	
	/// Helpers.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	
	UFUNCTION()
	static void GetSlotDataFromSlotSignature(const FRZ_InventorySlotSignature& SlotSignature, FRZ_InventorySlotData& SlotData);

	/// QuickBar.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	
	UFUNCTION()
	void SelectQuickBar(int32 QuickBarID);
	
	UFUNCTION()
	void SelectNextQuickBar(bool bIsNext);

	UFUNCTION()
	bool IsSlotOnSelectedQuickBar(int32 SlotID) const;

	UFUNCTION()
	void GetSlotsFromQuickBar(int32 QuickBarID, TArray<FRZ_InventorySlotData>& ResultArray) const;

	UFUNCTION()
	void SelectFirstSlotFromQuickBar();

	//

	FORCEINLINE int32 GetSelectedQuickBarID() const { return SelectedQuickBarID; }

private:

	UPROPERTY()
	int32 SelectedQuickBarID;
	
	/// Crafting.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	
	UFUNCTION()
	void StartCraftingItem(const FName& ItemToCraftName);

	UFUNCTION()
	void StopCraftingActor();

	UFUNCTION()
	void EndCraftingActor();

	//

	FORCEINLINE FName GetCraftedItemName() const { return CraftedItemName; }
	FORCEINLINE FTimerHandle GetCraftingTimer() const { return CraftingTimer; }

private:

	UPROPERTY()
	TMap<FName, int32> CraftingQueue;
	
	UPROPERTY()
	FTimerHandle CraftingTimer;
	
	UPROPERTY()
	FName CraftedItemName;
	
	/// Misc.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:
	
	UFUNCTION()
	void Debug(float DeltaTime);
};

	


