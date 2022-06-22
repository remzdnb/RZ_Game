/// RemzDNB
///
///	Everything that needs to be shared between all plugins and game modules.
///	Trying to keep it as light as possible, with only some basic data structures / interfaces usable everywhere.
///
///	Item : Any actor that both implements IRZ_ItemInterface / holds a FRZ_ItemSettings struct. They will be used
///	to store shared information for game and UI / provide common logic.
///	ToDo : Store ItemSettings in interface ?
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RZM_Shared.generated.h"

#define BASEVIEWHEIGHT 140.0f // Reference height for top-down gameplay.

	/// Module setup
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FRZM_SharedModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

	/// Types
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM()
enum class ERZ_ItemType : uint8
{
	Default, // Hands / Construction Item
	Weapon,
	Attachment,
	Gear,
	Building
};

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
enum class ERZ_ItemMode : uint8
{
	Hidden,
	Construction,
	Visible
};

	/// DataTable structures
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct RZM_SHARED_API FRZ_ItemSettings : public FTableRowBase
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

	FRZ_ItemSettings()
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
		AnimType = ERZ_ItemAnimType::Default;
	}
};

UCLASS()
class RZM_SHARED_API URZ_SharedModuleSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	
	URZ_SharedModuleSettings();

	const FRZ_ItemSettings* GetItemSettingsFromTableRow(const FName& RowName) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* InventoryItemSettingsDataTable;
};

	/// Interfaces
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UINTERFACE(MinimalAPI)
class URZ_ItemInterface : public UInterface
{
	GENERATED_BODY()
};

class RZM_SHARED_API IRZ_ItemInterface
{
	GENERATED_BODY()

public:

	IRZ_ItemInterface();
	
	// Settings.
	void InitItemSettings(const UWorld* World, const FName& TableRowName);
	virtual const FRZ_ItemSettings& GetItemSettings() { return ItemSettings; }
	virtual const FName& GetTableRowName() = 0;
	
	// ControllerTargetLocation
	// Calculated by controllers.
	// Then sent to server by remote local controllers.
	// Then sent to owned pawns by server controllers.
	// Then replicated by owned pawns.
	// Then sent to child actors (items, weapons, ...)
	virtual void SetPlayerTargetLocation(const FVector& NewPlayerTargetLocation) = 0;

	//
	virtual void SetItemMode(ERZ_ItemMode NewItemMode);
	
	// Called when this Item is selected/unselected by an InventoryComponent.
	virtual void OnInventorySelection(bool bNewIsSelected);

	//
	virtual void OnDetachedFromInventory();
	
	// Called by player / AI controllers to start / stop using this item.
	virtual void SetWantToUse(bool bNewWantTouse); // use type ? primary/secondary/reload

	// Same for secondary action.
	virtual void SetWantToUse_Secondary(bool bNewWantToUse);

	// Interaction
	virtual void OnHoverStart();
	virtual void OnHoverEnd();

protected:

	FRZ_ItemSettings ItemSettings;
	ERZ_ItemMode ItemMode;


	bool bIsSelected;

public:
	
	UActorComponent* OwnerInventory;
	
};

//

UINTERFACE(MinimalAPI)
class URZ_SharedModuleInterface : public UInterface
{
	GENERATED_BODY()
};

class RZM_SHARED_API IRZ_SharedModuleInterface
{
	GENERATED_BODY()

	// Desc

public:

	// Used to load editor data from a single DataAsset reference. Must be implemented in projects GameInstance.
	virtual const URZ_SharedModuleSettings* GetSharedModuleSettings() = 0;
};

// Utility


class RZM_SHARED_API RZ_UtilityLibrary
{
	
public:

	RZ_UtilityLibrary();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Conversions
	
	template<typename TEnum>
	static FORCEINLINE FString GetEnumAsString(const FString& Name, TEnum Value)
	{
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
		if (!enumPtr)
		{
			return FString("Invalid");
		}
		FString Str = enumPtr->GetNameByValue((int64)Value).ToString();
		Str.RemoveAt(0, Name.Len() + 2, true);
		return Str;
	}
	
	static FORCEINLINE FString GetFloatAsStringWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero = true)
	{
		//Round to integral if have something like 1.9999 within precision
		float Rounded = roundf(TheFloat);
		if (FMath::Abs(TheFloat - Rounded) < FMath::Pow(10, -1 * Precision))
		{
			TheFloat = Rounded;
		}
		FNumberFormattingOptions NumberFormat;					//Text.h
		NumberFormat.MinimumIntegralDigits = (IncludeLeadingZero) ? 1 : 0;
		NumberFormat.MaximumIntegralDigits = 10000;
		NumberFormat.MinimumFractionalDigits = Precision;
		NumberFormat.MaximumFractionalDigits = Precision;
		return FText::AsNumber(TheFloat, &NumberFormat).ToString();
	}
	
	static FORCEINLINE FText GetFloatAsTextWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero = true)
	{
		//Round to integral if have something like 1.9999 within precision
		float Rounded = roundf(TheFloat);
		if (FMath::Abs(TheFloat - Rounded) < FMath::Pow(10, -1 * Precision))
		{
			TheFloat = Rounded;
		}
		FNumberFormattingOptions NumberFormat;					//Text.h
		NumberFormat.MinimumIntegralDigits = (IncludeLeadingZero) ? 1 : 0;
		NumberFormat.MaximumIntegralDigits = 10000;
		NumberFormat.MinimumFractionalDigits = Precision;
		NumberFormat.MaximumFractionalDigits = Precision;
		return FText::AsNumber(TheFloat, &NumberFormat);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Math

	static bool CompareFloats(float Value1, float Value2, float Acceptance);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Actor

	static TArray<AActor*> OrderActorsByDistance(TArray<AActor*> ActorsToOrder, FVector TargetLocation);
	static AActor* GetClosestActorFromLocation(TArray<AActor*>, FVector TargetLocation);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// Debug

	static void PrintBoolToScreen(bool bBoolToPrint, FString PrefixString = "", float TimeToDisplay = 0.1f);
	static void PrintStringToScreen(const FString& PrefixString, const FString& InputString, FColor Color = FColor::Red, int32 Key = -1, float TimeToDisplay = 0.1f);
	static void PrintFloatToScreen(const FString& PrefixString, float InputFloat, FColor Color = FColor::Red, int32 Key = 1, float TimeToDisplay = 0.1f);
	static void PrintVectorToScreen(const FString& PrefixString, FVector InputVector, FColor Color = FColor::Red, int32 Key = 1, float TimeToDisplay = 1.0f);
	static void PrintRotatorToScreen(FRotator RotatorToPrint, FString PrefixString = "", int32 Key = -1, float TimeToDisplay = 0.1f);
	
};
