/// RemzDNB
///
/// RZM_WeaponSystem.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/Interface.h"
#include "Modules/ModuleManager.h"
#include "RZM_WeaponSystem.generated.h"

class ARZ_Weapon;
class ARZ_ProjectileWeapon;
class ARZ_Projectile;
class ARZ_Attachment;

	/// Module setup
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FRZM_WeaponSystemModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

	/// Module types
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
enum class ERZ_WeaponState : uint8 // declare in inventoryitem interface
{
	Ready,
	Firing,
	Reloading
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

	/// Module settings
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct RZM_WEAPONSYSTEM_API FRZ_ProjectileWeaponSettings : public FTableRowBase
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
	uint8 MuzzleCount;

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

	FRZ_ProjectileWeaponSettings()
	{
		Damage = 100.0f;
		FireTime = 0.2f;
		RecoilAmount = 1.0f;
		bIsAutoFire = true;
		MaxClipAmmo = 100;
		MaxStockAmmo = 100;
		ReloadTime = 3.0f;
		MuzzleCount = 1;
		ProjectilesPerShot = 1;
		ProjectileSpread = 0.1f;
		ProjectileSpeed = 10000.0f;
		MeshScale = FVector(1.0f, 1.0f, 1.0f);
		MeshCenterOffset = FVector(0.0f, 0.0f, 0.0f);
	}
};

USTRUCT(BlueprintType)
struct RZM_WEAPONSYSTEM_API FRZ_MeleeWeaponSettings : public FTableRowBase
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

	FRZ_MeleeWeaponSettings()
	{

	}
};

USTRUCT(BlueprintType)
struct RZM_WEAPONSYSTEM_API FRZ_LaserWeaponSettings : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	FRZ_LaserWeaponSettings()
	{

	}
};

UCLASS()
class RZM_WEAPONSYSTEM_API URZ_WeaponSystemModuleSettings : public UDataAsset
{
	GENERATED_BODY()

public:

	URZ_WeaponSystemModuleSettings();
	
	const FRZ_ProjectileWeaponSettings* GetProjectileWeaponInfoFromRow(const FName& RowName) const;
	const FRZ_MeleeWeaponSettings* GetMeleeWeaponInfoFromRow(const FName& RowName) const;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* ProjectileWeaponInfoDT;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* MeleeWeaponInfoDT;

	//
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* EngineCylinderMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMaterialInterface* ViewSplineMaterial;


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

	/// Module interfaces
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UINTERFACE(MinimalAPI)
class URZ_WeaponSystemModuleInterface : public UInterface
{
	GENERATED_BODY()
};

class RZM_WEAPONSYSTEM_API IRZ_WeaponSystemModuleInterface
{
	GENERATED_BODY()

public:

	// Used to load editor data from a single DataAsset reference. Must be implemented in projects GameInstance.
	virtual const URZ_WeaponSystemModuleSettings* GetWeaponSystemModuleSettings() = 0;
};

UINTERFACE(MinimalAPI)
class URZ_WeaponDamageInterface : public UInterface // Weapon interface .. combat interface ?
{
	GENERATED_BODY()
};

class RZM_WEAPONSYSTEM_API IRZ_WeaponDamageInterface
{
	GENERATED_BODY()

public:

	virtual void OnProjectileCollision(float ProjectileDamage, const FVector& HitLocation, AController* InstigatorController) = 0;
};	

