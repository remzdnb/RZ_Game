#include "RZ_CameraManager.h"
//
#include "Kismet/KismetMathLibrary.h"

ARZ_CameraManager::ARZ_CameraManager() :
	bIsDebugEnabled(true)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> PresetDataTableObject(TEXT(
		"DataTable'/RZ_CameraPlugin/RZ_CameraSettingsPresets_DT.RZ_CameraSettingsPresets_DT'"
	));
	if (PresetDataTableObject.Succeeded())
	{
		PresetDataTable = PresetDataTableObject.Object;
	}
}

void ARZ_CameraManager::BeginPlay()
{
	//UpdateActivePreset("TDFree");
	//TargetRotation = FRotator(Settings.PitchDefault, 180.0f, 0.0f);
	//CurrentRotation = TargetRotation;
	//TargetZoom = Settings.ZoomDefault;
	//CurrentZoom = TargetZoom;
}

void ARZ_CameraManager::UpdateActivePreset(const FName& NewPresetName)
{
	if (PCOwner == nullptr || PresetDataTable == nullptr)
		return;
	
	const FString ContextString;
	const FRZ_CameraSettingsPreset* CameraPreset = PresetDataTable->FindRow<FRZ_CameraSettingsPreset>(NewPresetName, ContextString);
	if (CameraPreset)
	{
		ActivePresetName = NewPresetName;
		ActivePreset = *CameraPreset;

		ArmLength = ActivePreset.ArmLengthDefault;
		//PCOwner->SetControlRotation(FRotator(ActivePreset.PitchDefault, PCOwner->GetControlRotation().Yaw, 0.0f));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ARZ_CameraManager::UpdateActivePreset - PresetName not found : %s"), *NewPresetName.ToString());
	}
}

void ARZ_CameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	if (PCOwner == nullptr)
		return;

	//

	if (ActivePreset.ArmLengthInterpSpeed == 0)
	{
		SmoothedOutArmLength = ArmLength;
	}
	else
	{
		SmoothedOutArmLength = UKismetMathLibrary::Lerp(
			SmoothedOutArmLength,
			ArmLength,
			DeltaTime * ActivePreset.ArmLengthInterpSpeed
		);
	}
	
	/// Calc arm location.

	if (ActivePreset.bIsAttached)
	{
		TargetActor = PCOwner->GetPawn();
		if (TargetActor.IsValid())
		{
			ArmLocation =
				TargetActor->GetActorLocation() +
					TargetActor->GetActorForwardVector() * ActivePreset.ArmOffset.X +
						TargetActor->GetActorRightVector() * ActivePreset.ArmOffset.Y +
							TargetActor->GetActorUpVector() * ActivePreset.ArmOffset.Z;// ? no, follow control rotation
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ARZ_CameraManager::UpdateViewTarget : Invalid target actor."));
		}
	}

	/// Calc arm rotation.
	
	ArmRotation = PCOwner->GetControlRotation().Add(0.0f, 180.0f, 0.0f);
	ArmRotation.Pitch = PCOwner->GetControlRotation().Pitch * -1;

	// Camera Location
	
	CameraLocation = ArmLocation + ArmRotation.Vector() * SmoothedOutArmLength;

	// Camera Rotation
	
	CameraRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, ArmLocation);
	
	OutVT.POV.Location = CameraLocation;
	OutVT.POV.Rotation = CameraRotation;

	//if (bIsDebugEnabled)
		//Debug(DeltaTime);
/*
	// Offset from cursor screen position

	float MouseX;
	float MouseY;
	int32 SizeX;
	int32 SizeY;

	GetOwningPlayerController()->GetMousePosition(MouseX, MouseY);
	GetOwningPlayerController()->GetViewportSize(SizeX, SizeY);

	MouseX = MouseX - (SizeX / 2);
	MouseY = MouseY - (SizeY / 2);

	const FRotator CameraYawRotation(0, GetCameraRotation().Yaw, 0);
	const FVector TargetOffsetX = FRotationMatrix(CameraYawRotation).GetScaledAxis(EAxis::X) * MouseY;
	const FVector TargetOffsetY = FRotationMatrix(CameraYawRotation).GetScaledAxis(EAxis::Y) * MouseX;

	// Calc location

	CurrentOffsetX = FMath::Lerp(CurrentOffsetX, TargetOffsetX, 5.0f * DeltaTime);
	CurrentOffsetY = FMath::Lerp(CurrentOffsetY, TargetOffsetY, 5.0f * DeltaTime);
	const FVector TargetViewLocation = TargetActor->GetActorLocation() - CurrentOffsetX + CurrentOffsetY;

	// Calc rotation

	CurrentRotation = FMath::Lerp(CurrentRotation, TargetRotation, 10.0f * DeltaTime);

	// Calc zoom

	CurrentZoom = FMath::Lerp(CurrentZoom, TargetZoom, 10.0f * DeltaTime);

	//

	const FRotator InvertedCameraRotation = CurrentRotation * (-1.0f);

	OutVT.POV.Location = TargetViewLocation + (InvertedCameraRotation.Vector() * CurrentZoom);
	OutVT.POV.Rotation = UKismetMathLibrary::FindLookAtRotation(OutVT.POV.Location, TargetViewLocation);*/
}

void ARZ_CameraManager::AddRotation(FRotator RotationToAdd)
{
    //TargetRotation += RotationToAdd;
}

void ARZ_CameraManager::PitchUp()
{
	//if (TargetRotation.Pitch - ActivePreset.PitchStep >= ActivePreset.PitchMin)
		//TargetRotation -= FRotator(ActivePreset.PitchStep, 0.0f, 0.0f);
}

void ARZ_CameraManager::PitchDown()
{
	//if (TargetRotation.Pitch + ActivePreset.PitchStep <= ActivePreset.PitchMax)
		//TargetRotation += FRotator(ActivePreset.PitchStep, 0.0f, 0.0f);
}

void ARZ_CameraManager::SetDefaultPitch()
{
	//TargetRotation = FRotator(ActivePreset.PitchDefault, TargetRotation.Yaw, 0.0f);
}

void ARZ_CameraManager::ZoomIn()
{
	if (ArmLength - ActivePreset.ArmLengthStep >= ActivePreset.ArmLengthMin)
		ArmLength -= ActivePreset.ArmLengthStep;
}

void ARZ_CameraManager::ZoomOut()
{
	if (ArmLength + ActivePreset.ArmLengthStep <= ActivePreset.ArmLengthMax)
		ArmLength += ActivePreset.ArmLengthStep;
}

void ARZ_CameraManager::SetDefaultZoom()
{
	//TargetZoom = ActivePreset.ArmLengthDefault;
}

void ARZ_CameraManager::Debug(float DeltaTime)
{

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, FString::Printf(TEXT("ARZ_CameraManager - Arm Location = %s"), *ArmLocation.ToString()));;
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, FString::Printf(TEXT("ARZ_CameraManager - Arm Rotation = %s"), *ArmRotation.ToString()));;
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, FString::Printf(TEXT("ARZ_CameraManager - bIsAttached = %s"), *FString::FromInt(ActivePreset.bIsAttached)));
}

//GEngine->AddOnScreenDebugMessage(12, 5.f, FColor::Red, FString::Printf(TEXT("Mouse: x: %f, y: %f"), MouseX, MouseY));
//GEngine->AddOnScreenDebugMessage(13, 5.f, FColor::Red, FString::Printf(TEXT("Viewport size: x: %i, y: %i"), SizeX, SizeY));
//GEngine->AddOnScreenDebugMessage(14, 5.f, FColor::Red, FString::Printf(TEXT("Mouse with VP: x: %f, y: %f"), MouseX, MouseY));