/// RemzDNB
///
///	RZ_ItemActorPluginInterfaces.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_ItemActorPluginTypes.h"
///
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RZ_ItemActorPluginInterfaces.generated.h"

/// Used to load editor data from a single DataAsset reference.
/// Must be implemented in project BaseGameState.

UINTERFACE(MinimalAPI)
class URZ_ItemActorPluginSettingsInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_ITEMACTORPLUGIN_API IRZ_ItemActorPluginSettingsInterface
{
	GENERATED_BODY()

public:

	virtual class URZ_ItemActorPluginSettings* GetItemActorPluginSettings() = 0;
};

//

UINTERFACE(MinimalAPI)
class URZ_ItemManagerInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_ITEMACTORPLUGIN_API IRZ_ItemManagerInterface
{
	GENERATED_BODY()

public:

	virtual void UpdateItemState(ERZ_ItemState NewItemState) = 0;
};

//

UINTERFACE(MinimalAPI)
class URZ_PawnItemInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_ITEMACTORPLUGIN_API IRZ_PawnItemInterface
{
	GENERATED_BODY()

public:

	virtual void OnInventoryItemEquipped(AActor* const EquippedItem) = 0;
	virtual void OnItemUsed(const FName& RowName) = 0;

	//
	// Get InventoryComponent from interface owner
	//virtual class URZ_ItemManager* GetInventoryCT() = 0;

	// Get TargetInventoryComponent from interface owner
	//virtual class URZ_ItemManager* GetInventoryCT_Target() = 0;

	// Get SkeletalMeshComponent from interface owner
	//virtual class USkeletalMeshComponent* GetSkeletalMeshCT() = 0;

	// Get TargetLocation from interface owner
	//virtual FVector GetTargetLocation() = 0;

	// Can interface owner perform a Use/Fire action ?
	//virtual bool CanUse() = 0;

	// Callbacks

	//virtual void OnItemReload(float ReloadTime, ERZ_ItemAnimType ItemAnimType) = 0;

	//virtual void OnPrimaryItemEquipped(FName ItemName) = 0;
	//virtual void OnGearItemEquipped(FName ItemName) = 0;
};
