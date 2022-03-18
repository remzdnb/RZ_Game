#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RZ_ItemActorPluginTypes.generated.h"

#define CHARACTER_HAND_R_SOCKETNAME "hand_rSocket"
#define CHARACTER_HAND_L_SOCKETNAME "hand_lSocket"
#define WEAPON_FRONTGRIP_R_SOCKETNAME "RFrontGripSocket"
#define WEAPON_FRONTGRIP_L_SOCKETNAME "LFrontGripSocket"

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
};

//

USTRUCT(BlueprintType)
struct RZ_ITEMACTORPLUGIN_API FRZ_ItemData : public FTableRowBase
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

	FRZ_ItemData()
	{
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
struct RZ_ITEMACTORPLUGIN_API FRZ_ProjectileWeaponData : public FTableRowBase
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
struct RZ_ITEMACTORPLUGIN_API FRZ_LaserWeaponData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	FRZ_LaserWeaponData()
	{

	}
};

USTRUCT(BlueprintType)
struct RZ_ITEMACTORPLUGIN_API FRZ_MeleeWeaponData : public FTableRowBase
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