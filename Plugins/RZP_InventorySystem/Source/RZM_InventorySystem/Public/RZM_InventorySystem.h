/// RemzDNB
///
/// RZM_InventorySystem.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_Shared.h"
#include "Engine/DataTable.h"
//#include "Modules/ModuleManager.h"
#include "RZM_InventorySystem.generated.h"

class URZ_InventoryComponent;

	/// Module setup
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FRZM_InventorySystemModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

	/// Module settings
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USTRUCT()
struct RZM_INVENTORYSYSTEM_API FRZ_InventorySlotInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 SlotID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FName> AllowedCategories;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* ItemActor;
	
	FRZ_InventorySlotInfo()
	{
		SlotID = 0;
		ItemName = "Empty";
		ItemActor = nullptr;
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
	bool bSnapDemoActorToGrid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDebugInventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDebugInventorySlotWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* InventorySlotSettingsDataTable;
	
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

/// Module Interfaces
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UINTERFACE(MinimalAPI)
class URZ_InventorySystemModuleInterface : public UInterface
{
	GENERATED_BODY()
};

class RZM_INVENTORYSYSTEM_API IRZ_InventorySystemModuleInterface
{
	GENERATED_BODY()

	// Desc

public:

	// Used to load editor data from a single DataAsset reference. Must be implemented in projects GameInstance.
	virtual URZ_InventorySystemModuleSettings* GetInventorySystemModuleSettings() = 0;
};

