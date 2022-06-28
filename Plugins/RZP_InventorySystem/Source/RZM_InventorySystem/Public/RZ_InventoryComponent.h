/// RemzDNB
///
///	RZ_InventoryComponent.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_Shared.h"
#include "RZM_InventorySystem.h"
#include "Components/ActorComponent.h"
#include "RZ_InventoryComponent.generated.h"

#define MAXINVENTORYSLOTS 46
#define MAXQUICKSLOTS 9
#define MAXQUICKBARS (MAXINVENTORYSLOTS / MAXQUICKSLOTS) // % ?

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdatedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAddedDelegate, AActor*, SpawnedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSelectedDelegate, AActor*, SelectedItem);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RZM_INVENTORYSYSTEM_API URZ_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	URZ_InventoryComponent();

	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* TickFunction) override;

	//

	FInventoryUpdatedDelegate OnInventoryUpdated;
	FOnItemAddedDelegate OnItemAdded;
	FOnItemSelectedDelegate OnItemSelected;

	//
	
	UFUNCTION() // ToInventory / ToSlot ?
	void AddItemFromDataTable(const FName& DataTableRowName);

	UFUNCTION()
	void AddItemFromWorld(AActor* ItemToAdd);

	UFUNCTION()
	void SwapItems(uint8 SourceIndex, uint8 TargetIndex);

	// Storage Slots
	
	UFUNCTION()
	void SelectSlot(int32 SlotID);

	UFUNCTION()
	void DropSelectedSlot();

	UFUNCTION()
	void DestroySelectedSlot();

	UFUNCTION()
	void SetWantToUseEquippedItem(bool bNewWantToUse);

	// QuickBar

	UFUNCTION()
	void SelectQuickBar(int32 QuickBarID);
	
	UFUNCTION()
	void SelectNextQuickBar(bool bIsNext);

	UFUNCTION()
	bool IsSlotOnSelectedQuickBar(int32 SlotID) const;

	UFUNCTION()
	void GetSlotsFromQuickBar(int32 QuickBarID, TArray<FRZ_InventorySlotInfo>& ResultArray) const;

	UFUNCTION()
	void SelectFirstSlotFromQuickBar();

	//

	FORCEINLINE const TArray<FRZ_InventorySlotInfo>& GetStorageSlots() const { return StorageSlots; }
	FORCEINLINE const TArray<FRZ_InventorySlotInfo>& GetAttachedSlots() const { return AttachedSlots; }
	
	UFUNCTION()
	const FRZ_InventorySlotInfo& GetSlotInfo(int32 SlotID) const;

	FORCEINLINE int32 GetSelectedSlotID() const { return SelectedSlotID; }
	FORCEINLINE int32 GetSelectedQuickBarID() const { return SelectedQuickBarID; }
	FORCEINLINE IRZ_ActorInterface* GetSelectedItemInterface() const { return SelectedItemInterface; } 



	// AttachedItemSlots / PermanentWeaponSlots
	// Create and link in BP - TArray<PermanentWeapons>

	UPROPERTY() // Must be updated from the owner.
	FVector PlayerTargetLocation;

	//

	UFUNCTION()
	void Debug(float DeltaTime);

	UFUNCTION()
	void DebugSlot(const FRZ_InventorySlotInfo& SlotInfo);
	
private:

	const URZ_SharedModuleSettings* SharedModuleSettings;
	const URZ_InventorySystemModuleSettings* InventorySystemSettings;

	//

	UPROPERTY() // Inactive + hidden if unequipped, equipped to hands if selected.
	TArray<FRZ_InventorySlotInfo> StorageSlots;

	UPROPERTY() // Attached to owner mesh, always visible (gear, permanent weapons...).
	TArray<FRZ_InventorySlotInfo> AttachedSlots;

	//

	int32 SelectedQuickBarID;
	int32 SelectedSlotID;
	IRZ_ActorInterface* SelectedItemInterface;

	//

	UFUNCTION()
	bool IsAnyAvailableSlot() const;
	
	UFUNCTION() // Returns -1 if no available slot was found.
	int32 GetFirstAvailableSlotIndex() const;



};

	


