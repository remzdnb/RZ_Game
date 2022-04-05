#include "S2D_PlayerController.h"
// Plugins
#include "RZ_CameraManager.h"
#include "RZ_UIManager.h"
// Engine
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

AS2D_PlayerController::AS2D_PlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	DefaultInputMode.SetHideCursorDuringCapture(false);
	//DefaultInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
}

void AS2D_PlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AS2D_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	// Mouse /cursor modes ?
	
	SetInputMode(DefaultInputMode);
	SetShowMouseCursor(true);
	
	UpdateControlSettings("TopDown");
	CameraManager->UpdateActivePreset("AttachedTD");

	if (UIManager)
		UIManager->ToggleMenu(false);
}

void AS2D_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetPawn())
	{
		SetControlRotation(UKismetMathLibrary::FindLookAtRotation(
			GetPawn()->GetActorLocation(),
			S2DTargetHitResult.ImpactPoint
		));
	}

	SetTargetLocation(S2DTargetHitResult.Location);
	
}

void AS2D_PlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

}

void AS2D_PlayerController::OnMenuOpened(bool bNewIsVisible)
{
	Super::OnMenuOpened(bNewIsVisible);
	
	bNewIsVisible
		? DefaultMouseCursor = EMouseCursor::Default
		: DefaultMouseCursor = EMouseCursor::Crosshairs;

	bNewIsVisible
		? SetInputMode(DefaultInputMode)
		: SetInputMode(DefaultInputMode);
}

void AS2D_PlayerController::OnLookUpKeyPressed()
{
	Super::OnLookUpKeyPressed();

	if (CameraManager)
	{
		CameraManager->AddManualControlRotationPitch(1.0f);
	}
}

void AS2D_PlayerController::OnLookDownKeyPressed()
{
	Super::OnLookDownKeyPressed();

	if (CameraManager)
	{
		CameraManager->AddManualControlRotationPitch(-1.0f);
	}
}

void AS2D_PlayerController::OnLookRightKeyPressed()
{
	Super::OnLookRightKeyPressed();

	if (CameraManager)
	{
		CameraManager->AddManualControlRotationYaw(1.0f);
	}
}

void AS2D_PlayerController::OnLookLeftKeyPressed()
{
	Super::OnLookLeftKeyPressed();

	if (CameraManager)
	{
		CameraManager->AddManualControlRotationYaw(-1.0f);
	}
}
