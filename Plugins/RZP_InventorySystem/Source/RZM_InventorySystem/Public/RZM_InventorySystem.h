/// RemzDNB
///
/// RZM_InventorySystem.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Modules/ModuleManager.h"
#include "RZM_InventorySystem.generated.h"

class URZ_InventorySystemModuleSettings;

	/// Module setup
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FRZM_InventorySystemModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

	/// Module types
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
enum class ERZ_ItemType : uint8
{
	Weapon,
	Attachment,
	Gear,
	Building
};

UENUM(BlueprintType)
enum class ERZ_ItemAnimType : uint8
{
	Hands,
	Sword,
	Pistol,
	Rifle
};

	/// Module settings
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USTRUCT() // Inventory slot settings + attached item infos.
struct RZM_INVENTORYSYSTEM_API FRZ_InventorySlotSettings : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	// From DataTable
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//FName SlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> AllowedCategories;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//FName DefaultItemName;

	// Transient

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 SlotID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* ItemActor;
	
	FRZ_InventorySlotSettings()
	{
		//SlotName = "DefaultSlotName";
		//DefaultItemName = "DefaultItemName";
		SlotID = 0;
		ItemName = "Empty";
		ItemActor = nullptr;
	}
};

USTRUCT(BlueprintType) // Item settings, DataTable row.
struct RZM_INVENTORYSYSTEM_API FRZ_InventoryItemSettings : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ERZ_ItemType Type;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanStack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxStack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float EquipTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float UseTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bVisibleInShop;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Price;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AActor> ItemClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTexture2D* ThumbnailTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FIntPoint ThumbnailSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ERZ_ItemAnimType AnimType;

	FRZ_InventoryItemSettings()
	{
		UseTime = 0.0f;
		DisplayName = "Default";
		Type = ERZ_ItemType::Weapon;
		bCanStack = false;
		MaxStack = 1;
		EquipTime = 1.5f;
		bVisibleInShop = true;
		Price = 100;
		ItemClass = nullptr;
		ThumbnailTexture = nullptr;
		AnimType = ERZ_ItemAnimType::Hands;
	}
};

UCLASS() // Global module settings DataAsset
class RZM_INVENTORYSYSTEM_API URZ_InventorySystemModuleSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	
	URZ_InventorySystemModuleSettings();

	const FRZ_InventoryItemSettings* GetInventoryItemSettingsFromDataTable(const FName& RowName) const;

	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSnapDemoActorToGrid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDebugInventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDebugInventorySlotWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* InventoryItemSettingsDataTable;

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

///

UINTERFACE(MinimalAPI)
class URZ_ItemActorInterface : public UInterface
{
	GENERATED_BODY()
};

class RZM_INVENTORYSYSTEM_API IRZ_ItemActorInterface
{
	GENERATED_BODY()

	// Desc

public:
	
	virtual void OnEquipped() = 0;
	virtual void OnHolstered() = 0;
	virtual void SetWantsToUse(bool bNewWantsTouse) = 0;
	virtual const FRZ_InventoryItemSettings& GetItemSettings() = 0;
	virtual void ToggleDemoMode(bool bNewIsDemoMode) = 0;
};