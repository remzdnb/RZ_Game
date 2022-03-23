#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/Interface.h"
#include "Modules/ModuleManager.h"
#include "RZM_ItemManager.generated.h"

#pragma region +++++ Module ...
///
/// Module setup
///

class FRZM_ItemManagerModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

#pragma endregion

#pragma region +++++ Types ...
///
/// Module types
///

USTRUCT(BlueprintType)
struct RZM_ITEMMANAGER_API FRZ_ItemSlotSettings : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 SlotID;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> AllowedCategories;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class ARZ_Item* AttachedItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName DefaultItemName;
	
	FRZ_ItemSlotSettings()
	{
		SlotID = 0;
		SlotName = "DefaultSlotName";
		AttachedItem = nullptr;
		DefaultItemName = "DefaultItemName";
	}
};

#pragma endregion

#pragma region +++++ Settings ...
///
///	Module settings
///

UCLASS()
class RZM_ITEMMANAGER_API URZ_ItemManagerModuleSettings : public UDataAsset
{
	GENERATED_BODY()

public:

	URZ_ItemManagerModuleSettings();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	class UDataTable* ItemSlotsConfigDT;

	//
	
	const FRZ_ItemSlotSettings* GetItemSlotConfigFromRow(const FName& RowName) const;
};

#pragma endregion

#pragma region +++++ Interfaces ...
///
///	Module interfaces
///

UINTERFACE(MinimalAPI)
class URZ_ItemManagerModuleInterface : public UInterface
{
	GENERATED_BODY()
};

class RZM_ITEMMANAGER_API IRZ_ItemManagerModuleInterface
{
	GENERATED_BODY()

public:

	// Used to load editor data from a single DataAsset reference. Must be implemented in projects GameInstance.
	virtual URZ_ItemManagerModuleSettings* GetItemManagerModuleSettings() = 0;
};

#pragma endregion