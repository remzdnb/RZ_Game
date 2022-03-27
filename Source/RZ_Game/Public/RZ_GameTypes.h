#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RZ_GameTypes.generated.h"

// Character

UENUM(BlueprintType)
enum class ERZ_PawnOwnership : uint8
{
	Player,
	WaveAI,
	FreeAI
};

UENUM(BlueprintType)
enum class ERZ_ : uint8
{
	Player,
	AI
};

//

UENUM(BlueprintType)
enum class ERZ_GameType : uint8
{
	Lobby,
	Battle
};

UENUM(BlueprintType)
enum class ERZ_ControlRotationMode : uint8
{
	None,
	Mouse, // Use direct mouse input as control rotation.
	Cursor // Use cursor world position as a target for control rotation.
};

/*UENUM(BlueprintType)
enum class ERZ_ControlSettingsPresetName : uint8
{
	FirstPerson,
	ThirdPerson,
	TopDown,
	Strategic
};*/

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