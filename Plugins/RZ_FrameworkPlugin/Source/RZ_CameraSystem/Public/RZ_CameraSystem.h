#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RZ_CameraSystem.generated.h"

class FRZ_CameraSystemModule : public IModuleInterface
{
	
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

UENUM(BlueprintType)
enum class ERZ_CameraViewOffset : uint8
{
	Left,
	Center,
	Right
};

USTRUCT(BlueprintType)
struct RZ_CAMERASYSTEM_API FRZ_CameraSettingsPreset : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	bool bUseControllerRotation;
	
	UPROPERTY(EditDefaultsOnly) 
	bool bIsAttached;

	UPROPERTY(EditDefaultsOnly)
	bool bEnableMouseOffset;

	UPROPERTY(EditDefaultsOnly)
	float ArmLengthDefault;

	UPROPERTY(EditDefaultsOnly)
	float ArmLengthMin;

	UPROPERTY(EditDefaultsOnly)
	float ArmLengthMax;

	UPROPERTY(EditDefaultsOnly)
	float ArmLengthStep;

	UPROPERTY(EditDefaultsOnly)
	float ArmLengthInterpSpeed;

	UPROPERTY(EditDefaultsOnly)
	FVector ArmOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Manual control")
	float ManualMoveSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Manual control")
	float PitchMin;

	UPROPERTY(EditDefaultsOnly, Category = "Manual control")
	float PitchMax;

	UPROPERTY(EditDefaultsOnly, Category = "Manual control")
	float PitchStep;

	UPROPERTY(EditDefaultsOnly, Category = "Manual control")
	float PitchDefault;

	UPROPERTY(EditDefaultsOnly, Category = "Manual control")
	float YawStep;
	
	FRZ_CameraSettingsPreset()
	{
		bUseControllerRotation = false;
		bIsAttached = false;
		bEnableMouseOffset = false;
		ArmLengthDefault = 2500.0f;
		ArmLengthMin = 500.0f;
		ArmLengthMax = 2500.0f;
		ArmLengthStep = 100.0f;
		ArmLengthInterpSpeed = 1000.0f;
		ManualMoveSpeed = 10.0f;
		PitchMin = 20.0f;
		PitchMax = 85.0f;
		PitchStep = 5.0f;
		PitchDefault = 45.0f;
		YawStep = 45.0f;
	}
};

