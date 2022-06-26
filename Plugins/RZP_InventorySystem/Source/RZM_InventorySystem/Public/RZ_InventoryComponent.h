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

#define MAXINVENTORYSLOTS 30
#define MAXQUICKSLOTS 6
#define MAXQUICKBARS (MAXINVENTORYSLOTS / MAXQUICKSLOTS)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdatedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuickBarSelectedDelegate);
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
	
	UFUNCTION() // ToInventory / ToSlot ?
	void AddItemFromDataTable(const FName& DataTableRowName);

	UFUNCTION()
	void AddItemFromWorld(AActor* ItemToAdd);

	UFUNCTION()
	void SwapItems(uint8 SourceIndex, uint8 TargetIndex);

	//
	
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

	FInventoryUpdatedDelegate OnInventoryUpdated;
	FQuickBarSelectedDelegate OnQuickBarSelected;
	FOnItemAddedDelegate OnItemAdded;
	FOnItemSelectedDelegate OnItemSelected;

	//

	UFUNCTION()
	const FRZ_InventorySlotInfo& GetSlotInfo(int32 SlotID) const;

	TArray<FRZ_InventorySlotInfo> GetInventorySlots() const { return ItemSlots; }

	FORCEINLINE int32 GetSelectedSlotID() const { return SelectedSlotID; }
	FORCEINLINE int32 GetSelectedQuickBarID() const { return SelectedQuickBarID; }
	FORCEINLINE IRZ_ActorInterface* GetSelectedItemInterface() const { return SelectedItemInterface; } 

	UPROPERTY()
	TArray<FRZ_InventorySlotInfo> ItemSlots;

	UPROPERTY() // Must be updated from the owner.
	FVector PlayerTargetLocation;
	
private:

	UFUNCTION()
	bool IsAnyAvailableSlot() const;
	
	UFUNCTION() // Returns -1 if no available slot was found.
	int32 GetFirstAvailableSlotIndex() const;

	UFUNCTION()
	void Debug(float DeltaTime);

	//

	const URZ_SharedModuleSettings* SharedModuleSettings;
	URZ_InventorySystemModuleSettings* InventorySystemModuleSettings;
	
	//

	int32 SelectedQuickBarID;
	int32 SelectedSlotID;
	IRZ_ActorInterface* SelectedItemInterface;

};

	


