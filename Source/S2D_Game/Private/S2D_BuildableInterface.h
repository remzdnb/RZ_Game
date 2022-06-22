// RemzDNB

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "S2D_BuildableInterface.generated.h"

UINTERFACE(MinimalAPI)
class US2D_BuildableInterface : public UInterface
{
	GENERATED_BODY()
};

class IS2D_BuildableInterface
{
	GENERATED_BODY()

public:
	
	virtual void OnBuildStart() = 0;
	virtual void OnBuildStop() = 0;
	virtual void OnBuildEnd() = 0;

	virtual uint8 GetGridSize() = 0;

private:

	FIntPoint GridPosition;
};

/*

class RZM_SHARED_API IRZ_ItemInterface
{
	GENERATED_BODY()

public:

	IRZ_ItemInterface();
	
	// Settings.

	virtual const FName& GetTableRowName() = 0;

	virtual void SetItemMode(ERZ_ItemMode NewItemMode);
	
	// Called when this Item is selected/unselected by an InventoryComponent.
	virtual void OnInventorySelection(bool bNewIsSelected);

	//
	virtual void OnDetachedFromInventory();

	// ControllerTargetLocation
	// Calculated by controllers.
	// Then sent to server by remote local controllers.
	// Then sent to owned pawns by server controllers.
	// Then replicated by owned pawns.
	// Then sent to child actors (items, weapons, ...)

	virtual void SetPlayerTargetLocation(const FVector& NewPlayerTargetLocation) = 0;
	
	// Called by player / AI controllers to start / stop using this item.
	virtual void SetWantToUse(bool bNewWantTouse); // use type ? primary/secondary/reload

	// Same for secondary action.
	virtual void SetWantToUse_Secondary(bool bNewWantToUse);

	// Interaction
	virtual void OnHoverStart();
	virtual void OnHoverEnd();
	
	virtual void UpdateBuildModeLocation(const FVector& SpawnLocation, const FVector& LerpedItemLocation);
	virtual void SetBuildMeshVisibility(bool bNewIsVisible);
	virtual bool IsValidBuildLocation();

protected:

	FRZ_ItemSettings ItemSettings;
	ERZ_ItemMode ItemMode;
	bool bIsSelected;
};*/