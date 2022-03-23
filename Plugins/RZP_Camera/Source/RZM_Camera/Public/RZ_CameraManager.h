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
	bool bIsAttached;

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
	
	FRZ_CameraSettingsPreset()
	{
		bIsAttached = false;
		ArmLengthDefault = 2500.0f;
		ArmLengthMin = 500.0f;
		ArmLengthMax = 2500.0f;
		ArmLengthStep = 100.0f;
		ArmLengthInterpSpeed = 1000.0f;
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

	void AddRotation(FRotator RotationToAdd);
	void PitchUp();
	void PitchDown();
	void SetDefaultPitch();
	void ZoomIn();
	void ZoomOut();
	void SetDefaultZoom();

	//

	FORCEINLINE const UDataTable* GetPresetDataTable() const { return PresetDataTable; }
	FORCEINLINE const FName& GetActivePresetName() const { return ActivePresetName; }

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

	//
	
	FVector CurrentOffsetX;
	FVector CurrentOffsetY;

	bool bIsDebugEnabled;
};
