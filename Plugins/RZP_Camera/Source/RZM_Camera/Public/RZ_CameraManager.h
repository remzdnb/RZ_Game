/// RemzDNB
///
///	RZ_CameraManager.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Camera/PlayerCameraManager.h"
#include "Engine/DataTable.h"
#include "RZ_CameraManager.generated.h"

UENUM(BlueprintType)
enum class ERZ_CameraViewOffset : uint8
{
	Left,
	Center,
	Right
};

USTRUCT(BlueprintType)
struct FRZ_CameraSettingsPreset : public FTableRowBase
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
		PitchMin = 20.0f;
		PitchMax = 85.0f;
		PitchStep = 5.0f;
		PitchDefault = 45.0f;
		YawStep = 45.0f;
	}
};

UCLASS()
class RZM_CAMERA_API ARZ_CameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:

	ARZ_CameraManager();

	virtual void BeginPlay() override;
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

	void UpdateActivePreset(const FName& NewPresetName);
	
	void ZoomIn();
	void ZoomOut();

	//

	FORCEINLINE const UDataTable* GetPresetDataTable() const { return PresetDataTable; }
	FORCEINLINE const FName& GetActivePresetName() const { return ActivePresetName; }
	FORCEINLINE const FRotator& GetFinalCameraRotation() const { return CameraRotation; }

	//FORCEINLINE const FVector& GetCameraLocation() const { return CameraLocation; }

	//

	void Debug(float DeltaTime);

private:

	TWeakObjectPtr<AActor> TargetActor;
	
	UDataTable* PresetDataTable;
	FName ActivePresetName;
	FRZ_CameraSettingsPreset ActivePreset;

	float ArmLength;
	float SmoothedOutArmLength;
	FVector ArmLocation;
	FRotator ArmRotation;
	FVector CameraLocation;
	FRotator CameraRotation;

	FVector CursorOffsetX;
	FVector CursorOffsetY;

	bool bIsDebugEnabled;

	/// Local control rotation /// Independant from the player controller.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	
	void AddManualControlRotationPitch(float AxisValue);
	void AddManualControlRotationYaw(float AxisValue);
	void SetLocalControlRotationYaw(float NewYaw);

private:
	
	UPROPERTY()
	FRotator LocalControlRotation;

};