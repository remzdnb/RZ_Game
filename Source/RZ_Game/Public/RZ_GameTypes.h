#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RZ_GameTypes.generated.h"

UENUM(BlueprintType)
enum class ERZ_GameType : uint8
{
	Lobby,
	Battle
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
	bool bShowMouseCursor;

	UPROPERTY(EditDefaultsOnly)
	bool bYawFromMouseInput;

	UPROPERTY(EditDefaultsOnly)
	bool bPitchFromMouseInput;

	UPROPERTY(EditDefaultsOnly)
	bool bResetYaw;
	
	UPROPERTY(EditDefaultsOnly)
	bool bLerpControlRotation;
	
	UPROPERTY(EditDefaultsOnly)
	float PitchDefault;

	UPROPERTY(EditDefaultsOnly)
	float PitchMin;

	UPROPERTY(EditDefaultsOnly)
	float PitchMax;

	UPROPERTY(EditDefaultsOnly)
	float PitchStep;

	UPROPERTY(EditDefaultsOnly)
	FName DefaultCameraSettingsPresetName;
	
	FRZ_ControlSettings()
	{
		bShowMouseCursor = false;
		bYawFromMouseInput = false;
		bPitchFromMouseInput = false;
		bLerpControlRotation = false;
		bResetYaw = false;
		PitchDefault = -75.0f;
		PitchMin = -85.0f;
		PitchMax = -20.0f;
		PitchStep = 5.0f;
		DefaultCameraSettingsPresetName = "None";
	}
};