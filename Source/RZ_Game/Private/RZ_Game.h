/// RemzDNB
///
///	Base game framework for a TopDown/TPS/FPS shooter.
///	Implements several plugins (WeaponSystem, InventorySystem, UIManager, ...) into main base
///	classes (PlayerController, Pawns, ...)
///	This module demonstrates how to make these plugins work together, and also has a lot of core logic that can't
///	be contained in plugins, or is not good enough to deserve its own plugin. It is designed to be extended by other
///	modules that will define a more specific gameplay.
///	IDK if anyone will ever try to read this but I'm still quite a noob with C++, I did my best and any feedback
///	is appreciated.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once // I'm bored

#include "CoreMinimal.h" // https://soundcloud.com/twofeetmusic/fire-in-my-head
#include "RZ_ImageWidget.h"
#include "Engine/DataTable.h"
#include "RZ_Game.generated.h" // https://soundcloud.com/closetomonday/guns-biicla-remix

class URZ_GameInstance;
class URZ_GameSettings;
class ARZ_GameMode;
class ARZ_GameState;

class ARZ_Actor;
class ARZ_Pawn;
class ARZ_Character;
class ARZ_Vehicle;
class ARZ_Weapon;

class ARZ_PawnAIController;

class ARZ_Actor2DRenderer;
class URZ_OTM_Pawn_Widget;
class URZ_OTM_Character_Widget;
class URZ_ActorManager_MainWidget;

class UParticleSystem;
class USoundCue;

#pragma region +++++ RZGame ...

UENUM(BlueprintType) // Game
enum class ERZ_GameType : uint8
{
	Lobby,
	Battle
};

UENUM(BlueprintType)
enum class ERZ_ControllerInteractionMode : uint8
{
	None,
	Selection,
	Construction,
};

UENUM(BlueprintType) // Controller
enum class ERZ_ControlRotationMode : uint8
{
	None,
	Mouse, // Use direct mouse input as control rotation.
	Cursor // Use cursor world position as a target for control rotation.
};

UENUM(BlueprintType) // GAS
enum class ERZ_AbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Use
};


USTRUCT(BlueprintType)
struct FRZ_ControlSettings : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	bool bResetYaw;
	
	UPROPERTY(EditDefaultsOnly)
	float PitchDefault;

	UPROPERTY(EditDefaultsOnly)
	float PitchMin;

	UPROPERTY(EditDefaultsOnly)
	float PitchMax;

	UPROPERTY(EditDefaultsOnly)
	float PitchStep;

	UPROPERTY(EditDefaultsOnly)
	FName CameraSettingsPresetName;
	
	FRZ_ControlSettings()
	{
		bResetYaw = false;
		PitchDefault = -75.0f;
		PitchMin = -85.0f;
		PitchMax = -20.0f;
		PitchStep = 5.0f;
		CameraSettingsPresetName = "None";
	}
};

USTRUCT(BlueprintType)
struct FRZ_PawnSettings : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	float DefaultMaxHealth;
	
	FRZ_PawnSettings()
	{
		DefaultMaxHealth = 0.0f;
	}
};

#pragma endregion

#pragma region +++++ Weapon ...

UENUM(BlueprintType)
enum class ERZ_WeaponState : uint8 // declare in inventoryitem interface
{
	Disabled,
	Ready,
	Firing,
	Reloading
};

UENUM()
enum class ERZ_RangedWeaponType : uint8
{
	Melee,
	PhysicsProjectile,
	SplineProjectile,
	SingleTrace,
	MultiTrace
};

USTRUCT(BlueprintType)
struct RZ_GAME_API FRZ_WeaponSettings : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base")
	bool bIsTurretWeapon;

	//

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

	FRZ_WeaponSettings()
	{
		bIsTurretWeapon = false;
	}
};

USTRUCT(BlueprintType)
struct RZ_GAME_API FRZ_DistanceWeaponSettings : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	float Damage;

	//
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	bool bIsAutoFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	float DelayBetweenShots;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	int32 MaxClipAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	int32 MaxStockAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	float ReloadTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	uint8 BarrelCount; // How many barrels this weapon has.

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	uint8 TraceCountPerBarrel; // How many projectiles/traces will be spawned from each barrel.

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base")
	float TraceSpread; // Spacing between projectiles/traces for each barrel.
	
	// Projectile

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<AActor> ProjectileBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	float ProjectileSpeed;

	// Trace
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	FVector MeshScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	FVector MeshCenterOffset;

	FRZ_DistanceWeaponSettings()
	{
		Damage = 100.0f;
		bIsAutoFire = true;
		DelayBetweenShots = 0.2f;

		MaxClipAmmo = 100;
		MaxStockAmmo = 100;
		ReloadTime = 3.0f;
		BarrelCount = 1;
		TraceCountPerBarrel = 1;
		TraceSpread = 0.1f;
		ProjectileSpeed = 10000.0f;
		MeshScale = FVector(1.0f, 1.0f, 1.0f);
		MeshCenterOffset = FVector(0.0f, 0.0f, 0.0f);
	}
};

USTRUCT(BlueprintType)
struct RZ_GAME_API FRZ_MeleeWeaponSettings : public FTableRowBase
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

#pragma endregion

#pragma region +++++ AI ...

UENUM(BlueprintType) // GAS
enum class ERZ_AIActionType : uint8
{
	Move,
	Attack,
	Build
};

UENUM(BlueprintType) // AI
enum class ERZ_TargetAcquisitionMode : uint8
{
	Closest,
	FirstDetected,
	LastDamaged
};

USTRUCT(BlueprintType)
struct FRZ_AIAction
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FName ActionName;
	
	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY()
	AActor* TargetActor;
	
	FRZ_AIAction()
	{
		ActionName = "DefaultActionName";
		TargetActor = nullptr;
	}

	FRZ_AIAction(FName NewActionName, const FVector& NewTargetLocation, AActor* NewTargetActor)
	{
		ActionName = NewActionName;
		TargetLocation = NewTargetLocation;
		TargetActor = NewTargetActor;
	}
};

UINTERFACE(MinimalAPI)
class URZ_AIOwnerInterface : public UInterface
{
	GENERATED_BODY()
};

class IRZ_AIOwnerInterface
{
	GENERATED_BODY()

public:

	IRZ_AIOwnerInterface();

	virtual class UBehaviorTree* GetBehaviorTree() = 0;
	virtual AActor* GetTargetActor() const = 0;
	virtual void SetTargetActor(AActor* NewTargetActor) = 0;
};

#pragma endregion

UCLASS()
class RZ_GAME_API URZ_GameSettings : public UDataAsset
{
	GENERATED_BODY()

public:

	URZ_GameSettings();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plugins")
	TSubclassOf<class ARZ_PowerManager> PowerManagerClass;
	
	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	class UDataTable* ControlSettingsPresets;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	uint8 AISpawnCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	uint8 AIWaveDelay;

	//

	const FRZ_DistanceWeaponSettings* GetProjectileWeaponSettingsFromTableRow(const FName& RowName) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	UDataTable* ProjectileWeaponSettingsDataTable;
	
	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* CharacterBehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* TurretBehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TMap<FName, UMaterialInterface*> ActionTypeThumbnails;
	
	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<ARZ_Actor2DRenderer> SelectedCharacterRenderer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<ARZ_Actor2DRenderer> SelectedTargetPawnRendererClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<ARZ_Actor2DRenderer> SelectedItemRendererClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDLayoutWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> LoadoutHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> LoadoutMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> ItemButton_HUD_WidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> ItemButton_Menu_WidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> ActorManager_MainWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> ActorManager_ButtonWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> ActorManager_GridWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> ActorManager_ActorWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> DamageMarkerWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<URZ_ImageWidget> ActionSlotWidgetClass;
	
	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Misc")
	UStaticMesh* EngineCubeMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Misc")
	UStaticMesh* EngineSphereMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Misc")
	UStaticMesh* EnginePlaneMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Misc")
	UStaticMesh* EngineCylinderMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Misc")
	UStaticMesh* EngineConeMesh;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDebugGameState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDebugPlayerController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDebugWeapons;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDebugPerceptionComponent;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid")
	TSubclassOf<AActor> GridManagerClass;
};

//

UINTERFACE(MinimalAPI)
class URZ_WorldInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_GAME_API IRZ_WorldInteractionInterface // controller interface
{
	GENERATED_BODY()

public:

	IRZ_WorldInteractionInterface();
	
	virtual void OnWorldHover(bool bNewIsHovered) { return; }
	virtual void OnWorldSelection(bool bNewSelected) { return; }
};

UINTERFACE(MinimalAPI)
class URZ_ActorInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_GAME_API IRZ_ActorInterface
{
	GENERATED_BODY()

public:

	IRZ_ActorInterface();

	//
	
	virtual bool GetIsGameplayActor() const = 0; // tag
	virtual void SetIsGameplayActor(bool bNewIsGameplayActor) = 0;
};