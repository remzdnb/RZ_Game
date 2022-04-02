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
#include "Engine/DataTable.h"
#include "RZ_Game.generated.h" // https://soundcloud.com/closetomonday/guns-biicla-remix

class URZ_GameInstance;
class URZ_GameSettings;
class ARZ_GameMode;
class ARZ_GameState;

class ARZ_Character;
class ARZ_Building;
class ARZ_Vehicle;

	/// Types
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType) // Game
enum class ERZ_GameType : uint8
{
	Lobby,
	Battle
};

UENUM(BlueprintType) // Controller
enum class ERZ_PlayerControllerMode : uint8
{
	None,
	PawnControl, // Is possessing and actively controlling a pawn.
	Spawn, // Is using a SpawnManagerComponent.
};

UENUM(BlueprintType) // Controller
enum class ERZ_ControlRotationMode : uint8
{
	None,
	Mouse, // Use direct mouse input as control rotation.
	Cursor // Use cursor world position as a target for control rotation.
};

UENUM(BlueprintType) // Pawn
enum class ERZ_PawnOwnership : uint8
{
	Player,
	WaveAI, // AI targeting players/buildings.
	FreeAI // Roaming AI.
};

UENUM(BlueprintType) // AI
enum class ERZ_AIAttitude : uint8
{
	Friendly,
	Neutral,
	Defensive,
	Aggressive
};

UENUM(BlueprintType) // AI
enum class ERZ_TargetAcquisitionMode : uint8
{
	Closest,
	FirstDetected,
	LastDamaged
};

UENUM(BlueprintType) // GAS
enum class ERZ_AbilityInputID : uint8
{
	None,
	Confirm,
	Cancel,
	Use
};

USTRUCT()
struct FRZ_DamageInfo
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	float Amount;

	UPROPERTY()
	float NewHealth;

	UPROPERTY()
	float MaxHealth;

	UPROPERTY()
	FVector Location;
	
	UPROPERTY()
	AController* InstigatorController;

	UPROPERTY()
	APawn* InstigatorPawn;

	FRZ_DamageInfo()
	{
		Amount = 0.0f;
		NewHealth = 0.0f;
		MaxHealth = 0.0f;
		Location = FVector::ZeroVector;
		InstigatorController = nullptr;
		InstigatorPawn = nullptr;
	}
};

	/// Settings
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FRZ_ControlSettings : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	ERZ_ControlRotationMode ControlRotationMode;
	
	UPROPERTY(EditDefaultsOnly)
	bool bShowMouseCursor;

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
		ControlRotationMode = ERZ_ControlRotationMode::Mouse;
		bShowMouseCursor = false;
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

	/// Game interfaces
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////


UINTERFACE(MinimalAPI)
class URZ_PawnInterface : public UInterface
{
	GENERATED_BODY()
};

class IRZ_PawnInterface // AI interface ?
{
	GENERATED_BODY()

public:

	IRZ_PawnInterface();

	virtual void Init(ERZ_PawnOwnership NewPawnOwnerShip, uint8 NewTeamID) = 0;
	
	// Combat

	void SetTeamID(uint8 NewTeamID);
	uint8 GetTeamID() const;
	void SetPawnOwnerShip(ERZ_PawnOwnership NewPawnOwnership);
	ERZ_PawnOwnership GetPawnOwnership() const;
	
	// AI

	virtual class UBehaviorTree* GetBehaviorTree() = 0;
	virtual void SetActiveTarget(AActor* NewActiveTarget) = 0; // Called from AIController.
	virtual void SetWantToFire(bool bNewWantToFire) = 0; // Called from AIController.

	virtual AActor* GetAssignedTarget() const { return AssignedTarget.Get(); }
	void SetAssignedTarget(AActor* NewAssignedTarget); // Called from GameMode.

	//

	static ERZ_AIAttitude GetAIAttitude(
		const IRZ_PawnInterface* OriginActorInterface,
		const IRZ_PawnInterface* TargetActorInterface
	);

private:
	
	uint8 TeamID;
	ERZ_PawnOwnership PawnOwnerShip;
	TWeakObjectPtr<AActor> AssignedTarget;
	
	//virtual void ToggleOnHitEffects() = 0;
	//virtual void StartHover() = 0;
	//virtual void StopHover() = 0;
	//virtual void Use(class ARZ_PlayerController* InstigatorController) = 0;
	//virtual void ToggleDemoMode(bool bNewIsEnabled) = 0;
};

//

UINTERFACE(MinimalAPI)
class URZ_PawnAIInterface : public UInterface
{
	GENERATED_BODY()
};

class IRZ_PawnAIInterface // AI interface ?
{
	GENERATED_BODY()

public:	//virtual void ToggleDemoMode(bool bNewIsEnabled) = 0;

};

//

//UE_LOG(LogTemp, Warning, TEXT("DDD"));
//UE_LOG(LogTemp, Warning, TEXT("Debug : %i" ), CrashInfo.BuiltFromCL);
// GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, "Test"))
//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, *FString::Printf(TEXT("URZ_InventoryComponent::AddItem : %s"), *DataTableRowName.ToString()));
