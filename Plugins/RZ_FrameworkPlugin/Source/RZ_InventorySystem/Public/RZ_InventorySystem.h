/// RemzDNB
///
/// RZ_InventorySystem.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_BaseWidgets.h"
#include "Engine/DataTable.h"
#include "RZ_InventorySystem.generated.h"

class URZ_InventoryComponent;
class URZ_AttachedSlotComponent;
class URZ_ItemSlot_Inventory_Widget;
class URZ_ItemSlot_Crafting_Widget;
class URZ_ItemSlot_Display_Widget;

class FRZ_InventorySystemModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM()
enum class ERZ_ItemUseType : uint8
{
	Primary,
	Secondary,
	Reload
};

UENUM()
enum class ERZ_ItemAnimType : uint8
{
	Default,
	Sword,
	Pistol,
	Rifle
};

UENUM()
enum class ERZ_InventorySlotType : uint8
{
	Storage,
	Attached
};

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct RZ_INVENTORYSYSTEM_API FRZ_ItemSettings : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> ActorClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* ThumbnailTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ThumbnailRenderScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ThumbnailRenderAngle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxStackSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsEquippable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float EquipTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsCraftable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CraftingTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, int32> CraftingComponents;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    ERZ_ItemAnimType AnimType;
	
	FRZ_ItemSettings()
	{
		ActorClass = nullptr;
		DisplayName = "Default";
		ThumbnailTexture = nullptr;
		ThumbnailRenderScale = 1.0f;
		ThumbnailRenderAngle = 0.0f;
		MaxStackSize = 1;
		bIsEquippable = true;
		EquipTime = 1.5f;
		bIsCraftable = true;
		CraftingTime = 3.0f;
		AnimType = ERZ_ItemAnimType::Default;
	}
};

USTRUCT()
struct RZ_INVENTORYSYSTEM_API FRZ_InventorySlotSignature : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URZ_InventoryComponent* OwnerInventoryComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ERZ_InventorySlotType SlotType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 SlotID;

	bool IsEqual(const FRZ_InventorySlotSignature& InventorySlotSignature) const
	{
		if (OwnerInventoryComp != InventorySlotSignature.OwnerInventoryComp) { return false; }
		if (SlotType != InventorySlotSignature.SlotType) { return false; }
		if (SlotID != InventorySlotSignature.SlotID) { return false; }

		return true;
	}
	
	FRZ_InventorySlotSignature()
	{
		OwnerInventoryComp = nullptr;
		SlotType = ERZ_InventorySlotType::Storage;
		SlotID = 0;	
	}
};

USTRUCT()
struct RZ_INVENTORYSYSTEM_API FRZ_InventorySlotData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FRZ_InventorySlotSignature InventorySlotSignature;

	// Transient

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName ActorName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 StackSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* SpawnedActor;

	// Defaults

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName DefaultActorName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> ActorNamesWhitelist;

	//
	
	FRZ_InventorySlotData()
	{
		ActorName = "None";
		StackSize = 0;
		SpawnedActor = nullptr;
		DefaultActorName = "DefaultActorName";
	}
};

USTRUCT(BlueprintType)
struct RZ_INVENTORYSYSTEM_API FRZ_InventoryComponentSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxStorageSlots;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxQuickStorageSlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<FName, int32> DefaultStorageItems;

	FRZ_InventoryComponentSettings()
	{
		MaxStorageSlots = 46;
		MaxQuickStorageSlots = 6;
	}
};

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UINTERFACE(MinimalAPI)
class URZ_InventoryOwnerInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_INVENTORYSYSTEM_API IRZ_InventoryOwnerInterface
{
	GENERATED_BODY()

	// Must be implemented by any Actor that owns an InventoryComponent.

public:

	virtual URZ_InventoryComponent* GetInventoryComponent() const = 0;
	virtual USceneComponent* GetAttachComponent() const = 0;
};

UINTERFACE(MinimalAPI)
class URZ_InventoryItemInterface : public UInterface // item interface
{
	GENERATED_BODY()
};

class RZ_INVENTORYSYSTEM_API IRZ_InventoryItemInterface
{
	GENERATED_BODY()

	// Must be implemented by any Actor handled by InventoryComponents.

public:

	virtual const FName& GetItemName() const = 0;
	virtual const FRZ_ItemSettings& GetItemSettings() const = 0;
	virtual URZ_InventoryComponent* GetOwnerInventory() const { return nullptr; }
	virtual void SetOwnerInventory(URZ_InventoryComponent* NewOwnerInventory) { return; }

	virtual void OnAttachedToInventory() { return; }
	virtual void OnDetachedFromInventory() { return; }
	virtual void OnEquippedByInventory(bool bNewIsEquipped) { return; }
	virtual void OnWantToBeUsedByInventory(bool bNewWantTouse) { return; }
	
	//virtual int32 GetStackSize() const; // ?
	//virtual void SetStackSize(int32 NewStackSize); // ?
};

/// System.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class RZ_INVENTORYSYSTEM_API URZ_InventorySystemSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	
	URZ_InventorySystemSettings();

	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* ItemSettingsDataTable;

	const FRZ_ItemSettings* GetItemSettingsFromTableRow(const FName& RowName) const;

	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSnapDemoActorToGrid;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> InventoryHUDWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> InventoryMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<URZ_ItemSlot_Inventory_Widget> StorageSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<URZ_ItemSlot_Inventory_Widget> AttachedSlotWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<URZ_ItemSlot_Inventory_Widget> QuickSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<URZ_ItemSlot_Crafting_Widget> CraftingSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<URZ_ItemSlot_Display_Widget> CraftingComponentSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<URZ_ItemSlot_Display_Widget> DragSlotWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDebugInventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDebugActorSlotWidget;
};

UINTERFACE(MinimalAPI)
class URZ_InventorySystemInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_INVENTORYSYSTEM_API IRZ_InventorySystemInterface
{
	GENERATED_BODY()

	// Used to load editor data from a single DataAsset reference. Must be implemented in projects GameInstance.

public:
	
	virtual const URZ_InventorySystemSettings* GetInventorySystemSettings() const = 0;
};

