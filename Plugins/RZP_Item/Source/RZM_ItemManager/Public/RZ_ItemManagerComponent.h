/// RemzDNB
///
///	Item inventory manager.
///	V1 : Based on ItemActorPlugin types.
///	V2 : Standalone ?
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_ItemManager.h"
#include "Components/ActorComponent.h"
#include "RZ_ItemManagerComponent.generated.h"

class ARZ_Item;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSpawnedDelegate, ARZ_Item*, SpawnedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemEquippedDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RZM_ITEMMANAGER_API URZ_ItemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	URZ_ItemManagerComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	FOnItemSpawnedDelegate OnItemSpawned;
	FOnItemEquippedDelegate OnItemEquipped;

	//

	ARZ_Item* GetEquippedItem() const;
	const TArray<FRZ_ItemSlotSettings>& GetAttachedSlots() const { return AttachedSlots; }
	
	//
	
	UFUNCTION()
	ARZ_Item* SpawnItem(const FName& ItemName);

	UFUNCTION() // delete
	void ReplaceItem(uint8 SlotIndex, const FName& ItemName);
	
	UFUNCTION() // equip slot instead
	void EquipItem(uint8 ItemIndex);

private:

	class URZ_ItemActorModuleSettings* ItemActorModuleSettings;
	class URZ_ItemManagerModuleSettings* ItemManagerModuleSettings;
	
	//

	UPROPERTY()
	uint8 EquippedSlotIndex;

	UPROPERTY()
	TArray<FRZ_ItemSlotSettings> AttachedSlots;
};
