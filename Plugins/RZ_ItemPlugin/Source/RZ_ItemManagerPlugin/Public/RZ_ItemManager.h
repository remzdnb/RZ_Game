/// RemzDNB
///
///	RZ_ItemManager.h
///	Pawn inventory manager.
///	V1 : Based on ItemActorPlugin types.
///	V2 : Standalone ?
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "RZ_ItemManagerPluginTypes.h"
#include "Components/ActorComponent.h"
#include "RZ_ItemManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSpawnedDelegate, ARZ_Item*, SpawnedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemEquippedDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RZ_ITEMMANAGERPLUGIN_API URZ_ItemManager : public UActorComponent
{
	GENERATED_BODY()

public:	

	URZ_ItemManager();

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
	const TArray<FRZ_SlotConfig>& GetAttachedSlots() const { return AttachedSlots; }
	
	//
	
	UFUNCTION()
	ARZ_Item* SpawnItem(const FName& ItemName);

	UFUNCTION()
	void ReplaceItem(uint8 SlotIndex, const FName& ItemName);
	
	UFUNCTION()
	void EquipItem(uint8 ItemIndex);

private:

	class URZ_ItemManagerPluginSettings* ItemManagerPluginSettings;
	class URZ_ItemActorPluginSettings* ItemActorPluginSettings;
	
	//

	UPROPERTY()
	uint8 EquippedSlotIndex;

	UPROPERTY()
	TArray<FRZ_SlotConfig> AttachedSlots;
};
