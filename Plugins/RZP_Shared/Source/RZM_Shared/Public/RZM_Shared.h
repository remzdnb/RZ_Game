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

#define RZ_GRIDTILESIZE 100.0f // Reference grid unit size for 2D gameplay.
#define BASEVIEWHEIGHT 140.0f // Reference height for top-down gameplay.

class URZ_SensingComponent;
class URZ_CombatComponent;

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
enum class ERZ_ActorType : uint8
{
	Default, // Hands / Construction Item // No, in animtype
	Actor,
	Pawn,
	Character,
	Weapon
};

UENUM()
enum class ERZ_ActorMode : uint8
{
	Hidden_Disabled,// Hidden + disabled, when in inventory.
	Visible_Disabled, // Building mode.
	Visible_Enabled
};

UENUM()
enum class ERZ_UseType : uint8
{
	Primary,
	Secondary,
	Reload
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

	/// DataTable structures
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct RZM_SHARED_API FRZ_ActorSettings : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ERZ_ActorType Type;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ERZ_ItemAnimType AnimType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector NormalizedWorldSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UTexture2D* ThumbnailTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FIntPoint ThumbnailSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanStack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxStack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float UseTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float EquipTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bVisibleInShop;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Price;
	
	FRZ_ActorSettings()
	{
		UseTime = 0.0f;
		DisplayName = "Default";
		Type = ERZ_ActorType::Weapon;
		bCanStack = false;
		MaxStack = 1;
		EquipTime = 1.5f;
		bVisibleInShop = true;
		Price = 100;
		ActorClass = nullptr;
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

	const FRZ_ActorSettings* GetActorSettingsFromTableRow(const FName& RowName) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* ActorSettingsDataTable;
};

	/// Interfaces
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UINTERFACE(MinimalAPI)
class URZ_ActorInterface : public UInterface
{
	GENERATED_BODY()
};

class RZM_SHARED_API IRZ_ActorInterface
{
	GENERATED_BODY()

public:

	IRZ_ActorInterface();
	
	// Settings.
	void InitActorSettings(const UWorld* World, const FName& TableRowName);
	const FRZ_ActorSettings& GetActorSettings() { return ActorSettings; }
	virtual const FName& GetTableRowName() = 0;
	
	// ControllerTargetLocation // Controller interface ?
	// Calculated by controllers.
	// Then sent to server by remote local controllers.
	// Then sent to owned pawns by server controllers.
	// Then replicated by owned pawns.
	// Then sent to child actors (items, weapons, ...)
	virtual void SetPlayerTargetLocation(const FVector& NewPlayerTargetLocation) = 0;

	UFUNCTION() // to bind delegate to
	virtual void SetWantToUse(bool bNewWantsTouse);
	
	virtual void OnHoverStart();
	virtual void OnHoverEnd();

	virtual ERZ_ActorMode GetActorMode() const { return ActorMode; }
	virtual void SetActorMode(ERZ_ActorMode NewActorMode); // false = hidden & inactive, true = visible and active in world. // should be const = 0

	//virtual void SetTeamID(uint8 NewTeamID);
	//virtual uint8 GetTeamID();

protected:

	ERZ_ActorMode ActorMode;
	
	//uint8 TeamID;// Replication ? :/
	FRZ_ActorSettings ActorSettings;
};

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
