/// RemzDNB
///
///	RZ_InventoryComponent.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_InventorySystem.h"
#include "Components/ActorComponent.h"
#include "RZ_InventoryComponent.generated.h"

#define MAXINVENTORYSLOTS 30
#define MAXQUICKSLOTS 6

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryUpdatedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAddedDelegate, AActor*, SpawnedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemEquippedDelegate, AActor*, EquippedItem);

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
	
	UFUNCTION()
	void AddItem(const FName& DataTableRowName);

	UFUNCTION() // delete
	void ReplaceItem(uint8 SlotIndex, const FName& ItemName);

	UFUNCTION()
	void SwapItems(uint8 SourceIndex, uint8 TargetIndex);
	
	UFUNCTION() // equip slot instead
	void EquipItemSlot(uint8 SlotIndex);

	UFUNCTION()
	void DropEquippedItemAtTargetLocation();

	UFUNCTION()
	void SetWantToUseEquippedItem(bool bNewWantToUse);

	//

	FInventoryUpdatedDelegate OnInventoryUpdated;
	FOnItemAddedDelegate OnItemAdded;
	FOnItemEquippedDelegate OnItemEquipped;

	//

	//ARZ_Item* GetEquippedItem() const;
	TArray<FRZ_InventorySlotSettings> GetInventorySlots() const { return InventorySlots; }


	UPROPERTY()
	TArray<FRZ_InventorySlotSettings> InventorySlots;

	UPROPERTY() // Must be updated from the owner.
	FVector PlayerTargetLocation;
	
private:

	UFUNCTION()
	bool IsAnyAvailableSlot() const;
	
	UFUNCTION() // Returns -1 if no available slot was found.
	uint8 GetFirstAvailableSlotIndex() const;

	//

	UFUNCTION()
	void UpdateDemoActorLocation();

	//
	
	URZ_InventorySystemModuleSettings* InventorySystemModuleSettings;
	
	//

	UPROPERTY()
	uint8 EquippedSlotIndex;

	UPROPERTY()
	uint8 QuickSlotsIndex;

	IRZ_ItemActorInterface* EquippedItemInterface;

};

