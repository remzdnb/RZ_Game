#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/Interface.h"
#include "Modules/ModuleManager.h"
#include "RZM_ItemActor.generated.h"

#pragma region +++++ Module ...
///
///
///

class FRZM_ItemActorModule : public IModuleInterface
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

// SharedItemData
// UniqueItemData
// ItemSlotData_Manager
// ItemSlotData_UI
// ItemPluginSettings

UENUM(BlueprintType)
enum class ERZ_ItemType : uint8
{
	Weapon,
	Attachment,
	Gear,
	Building
};

UENUM(BlueprintType)
enum class ERZ_ItemState : uint8
{
	PickUp,
	Holstered,
	Equipping,
	Ready,
	Firing,
	Reloading
};

UENUM(BlueprintType)
enum class ERZ_ItemAnimType : uint8
{
	Hands,
	Sword,
	Pistol,
	Rifle
};

/*
UENUM(BlueprintType)
enum class ERZ_SlotType : uint8
{
	Weapon,
	Gear,
	Storage,
	Attachment,
	Quick,
	Shop,
	Drag
};*/

//

USTRUCT(BlueprintType)
struct RZM_ITEMACTOR_API FRZ_ItemInfo : public FTableRowBase
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
	bool bIsLargeThumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ERZ_ItemAnimType AnimType;

	FRZ_ItemInfo()
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
		bIsLargeThumbnail = true;
		AnimType = ERZ_ItemAnimType::Hands;
	}
};

USTRUCT(BlueprintType)
struct RZM_ITEMACTOR_API FRZ_ProjectileWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	float FireTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	float RecoilAmount;

	// Ranged

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	TSubclassOf<AActor> ProjectileBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	bool bIsAutoFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	int32 MaxClipAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	int32 MaxStockAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	float ReloadTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	uint8 ProjectilesPerShot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	float ProjectileSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ranged")
	float ProjectileSpeed;

	// FX

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
		class UParticleSystem* MuzzleParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
		class UParticleSystem* CharacterImpactParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
		class UParticleSystem* WorldImpactParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
		class USoundCue* FireSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
		class USoundCue* ReloadStartSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
		class USoundCue* ReloadEndSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
		class USoundCue* CharacterImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
		class USoundCue* WorldImpactSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
		class USoundAttenuation* FireSoundAttenuation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
		class USoundAttenuation* ReloadSoundAttenuation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
		class USoundAttenuation* ImpactSoundAttenuation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
		class USoundConcurrency* FireSoundConcurrency;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
		class USoundConcurrency* ReloadSoundConcurrency;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
		class USoundConcurrency* ImpactSoundConcurrency;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
		FVector MeshScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
		FVector MeshCenterOffset;

	FRZ_ProjectileWeaponData()
	{
		Damage = 100.0f;
		FireTime = 0.2f;
		RecoilAmount = 1.0f;
		bIsAutoFire = true;
		MaxClipAmmo = 100;
		MaxStockAmmo = 100;
		ReloadTime = 3.0f;
		ProjectilesPerShot = 1;
		ProjectileSpread = 0.1f;
		ProjectileSpeed = 10000.0f;
		MeshScale = FVector(1.0f, 1.0f, 1.0f);
		MeshCenterOffset = FVector(0.0f, 0.0f, 0.0f);
	}
};

USTRUCT(BlueprintType)
struct RZM_ITEMACTOR_API FRZ_LaserWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	FRZ_LaserWeaponData()
	{

	}
};

USTRUCT(BlueprintType)
struct RZM_ITEMACTOR_API FRZ_MeleeWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DamageStartTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DamageStopTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystem* CharacterImpactParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystem* WorldImpactParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundCue* AttackSound;

	FRZ_MeleeWeaponData()
	{

	}
};

#pragma endregion

#pragma region +++++ Settings ...
/// 
///	Module settings
///

UCLASS()
class RZM_ITEMACTOR_API URZ_ItemActorModuleSettings : public UDataAsset
{
	GENERATED_BODY()

public:

	URZ_ItemActorModuleSettings();
	
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	const FRZ_ItemInfo* GetItemInfoFromRow(const FName& RowName) const;
	const FRZ_ProjectileWeaponData* GetProjectileWeaponInfoFromRow(const FName& RowName) const;
	const FRZ_MeleeWeaponData* GetMeleeWeaponInfoFromRow(const FName& RowName) const;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* ItemInfoDT;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* ProjectileWeaponInfoDT;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* MeleeWeaponInfoDT;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bDebugPlayerItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bDebugAIItems;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bDebugEquippedItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bDebugHolsteredItems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bDebugWorldItems;
};

///
///
///
#pragma endregion

#pragma region +++++ Interfaces ...
/// 
///	Module interfaces
///

// Allows to load editor data from a single DataAsset reference, from any module class.
// Must be implemented in projects GameInstance.

UINTERFACE(MinimalAPI)
class URZ_ItemActorModuleInterface : public UInterface
{
	GENERATED_BODY()
};

class RZM_ITEMACTOR_API IRZ_ItemActorModuleInterface
{
	GENERATED_BODY()

public:

	// Used to load editor data from a single DataAsset reference. Must be implemented in projects GameInstance.
	virtual URZ_ItemActorModuleSettings* GetItemActorModuleSettings() = 0;
};

// Useless, call delegate from pawn class ?

UINTERFACE(MinimalAPI)
class URZ_PawnItemInterface : public UInterface
{
	GENERATED_BODY()
};

class RZM_ITEMACTOR_API IRZ_PawnItemInterface
{
	GENERATED_BODY()

public:

	virtual void OnInventoryItemEquipped(AActor* const EquippedItem) = 0;
	virtual void OnItemUsed(const FName& RowName) = 0;

	//
	// Get InventoryComponent from interface owner
	//virtual class URZ_ItemManagerComponent* GetInventoryComponent() = 0;

	// Get TargetInventoryComponent from interface owner
	//virtual class URZ_ItemManagerComponent* GetInventoryCT_Target() = 0;

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

// Useless, call delegate from pawn class ?

UINTERFACE(MinimalAPI)
class URZ_ProjectileInterface : public UInterface // Weapon interface ..
{
	GENERATED_BODY()
};

class RZM_ITEMACTOR_API IRZ_ProjectileInterface
{
	GENERATED_BODY()

public:

	virtual void OnProjectileCollision(float ProjectileDamage, const FVector& HitLocation, AController* InstigatorController) = 0;
};	

///
///
///
#pragma endregion
