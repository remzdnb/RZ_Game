/// RemzDNB
///
///	RZ_CameraManager.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_CameraSystem.h"
#include "Camera/PlayerCameraManager.h"
#include "RZ_CameraManager.generated.h"

UCLASS()
class RZ_CAMERASYSTEM_API ARZ_CameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:

	ARZ_CameraManager();

	virtual void BeginPlay() override;
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

	void UpdateActivePreset(const FName& NewPresetName);

	void SetBaseViewHeight(float NewViewHeight);
	
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

	//
	
	float ArmLength;
	float SmoothedOutArmLength;
	FVector ArmLocation;
	FRotator ArmRotation;
	FVector CameraLocation;
	FRotator CameraRotation;

	FVector CursorOffsetX;
	FVector CursorOffsetY;

	bool bIsDebugEnabled;

	float BaseViewHeight;

	/// Manual movement.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	void MoveManualTargetLocationForward(float AxisValue);
	void MoveManualTargetLocationRight(float AxisValue);

private:
	
	FVector ManualTargetLocation;
	
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