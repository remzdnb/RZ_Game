/// RemzDNB
///
/// RZM_InventorySystem.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_Shared.h"
#include "Engine/DataTable.h"
#include "RZM_InventorySystem.generated.h"

class URZ_InventoryComponent;
class URZ_AttachedSlotComponent;

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FRZM_InventorySystemModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USTRUCT()
struct RZM_INVENTORYSYSTEM_API FRZ_InventorySlotInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 SlotID;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* AttachedActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FName> AllowedCategories;
	
	FRZ_InventorySlotInfo()
	{
		SlotID = 0;
		AttachedActor = nullptr;
	}
};

UCLASS() // Global module settings DataAsset
class RZM_INVENTORYSYSTEM_API URZ_InventorySystemModuleSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	
	URZ_InventorySystemModuleSettings();

	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* InventorySlotSettingsDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSnapDemoActorToGrid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDebugInventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDebugInventorySlotWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> InventoryHUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> InventoryMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> InventorySlot_Menu_WidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> InventorySlot_HUD_WidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> InventorySlot_Drag_WidgetClass;
};

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UINTERFACE(MinimalAPI)
class URZ_InventorySystemModuleInterface : public UInterface
{
	GENERATED_BODY()
};

class RZM_INVENTORYSYSTEM_API IRZ_InventorySystemModuleInterface
{
	GENERATED_BODY()

	// Used to load editor data from a single DataAsset reference. Must be implemented in projects GameInstance.

public:
	
	virtual URZ_InventorySystemModuleSettings* GetInventorySystemModuleSettings() = 0;
};

UINTERFACE(MinimalAPI)
class URZ_InventoryOwnerInterface : public UInterface
{
	GENERATED_BODY()
};

class RZM_INVENTORYSYSTEM_API IRZ_InventoryOwnerInterface
{
	GENERATED_BODY()

	// Must be implemented by any Actor that owns an InventoryComponent.

public:

	virtual URZ_InventoryComponent* GetInventoryComponent() = 0;
};

UINTERFACE(MinimalAPI)
class URZ_InventoryActorInterface : public UInterface
{
	GENERATED_BODY()
};

class RZM_INVENTORYSYSTEM_API IRZ_InventoryActorInterface
{
	GENERATED_BODY()

	// Must be implemented by any Actor handled by InventoryComponents.

public:

	virtual void OnAttachedToInventory(URZ_InventoryComponent* InventoryCompRef);
	virtual void OnInventorySelection(bool bNewIsSelected) = 0;
	virtual void SetWantToUse(bool bNewWantToUse, ERZ_UseType UseType = ERZ_UseType::Primary) = 0;

	virtual URZ_InventoryComponent* GetOwnerInventory() { return OwnerInventory; }
	
private:
		
	URZ_InventoryComponent* OwnerInventory = nullptr;
};

