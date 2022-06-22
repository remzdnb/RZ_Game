/// RemzDNB

#include "S3D_PlayerController.h"
// RZ_Game
#include "Core/RZ_GameInstance.h"
#include "Pawn/RZ_Character.h"
#include "RZ_CharacterAnimInstance.h"
#include "RZ_CameraManager.h"
#include "RZ_UIManager.h"
// Engine
#include "Blueprint/WidgetBlueprintLibrary.h"

AS3D_PlayerController::AS3D_PlayerController() :
	bIsFirstPerson(true)
{

	//bShowMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AS3D_PlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	S3DGameSettings = Cast<US3D_GameSettings>(Cast<URZ_GameInstance>(GetGameInstance())->GetS3DGameSettings());
}

void AS3D_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	UpdateControlSettings("FirstPerson");
	//UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this);

	FInputModeGameOnly InputMode;
	//InputMode.SetHideCursorDuringCapture(true);
	//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(InputMode);
	SetShowMouseCursor(false);
	
	if (IsLocalController())
	{
		if (CameraManager)
		{
			CameraManager->UpdateActivePreset("AttachedFP");
			CameraManager->SetBaseViewHeight(S3DCHARACTERVIEWHEIGHT);
		}
		
		if (UIManager)
		{
			UIManager->CreateHUDWidget(S3DGameSettings->HUDLayoutWidget);
		}
	}

	if (UIManager)
		UIManager->ToggleMenu(false);
}

void AS3D_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCrosshairTrace(S3DCHARACTERVIEWHEIGHT, DeltaTime);
	
	if (IsLocalController())
	{
		SetTargetLocation(CrosshairToWorldHitResult.Location);
	}
}

void AS3D_PlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	if (PossessedCharacter.IsValid())
	{
		PossessedCharacter->GetCharacterAnimInstance()->SetHeadVisibility(false);
	}
}

void AS3D_PlayerController::OnMenuOpened(bool bNewIsVisible)
{
	Super::OnMenuOpened(bNewIsVisible);

	bShowMouseCursor = bNewIsVisible;

	bNewIsVisible
		? UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this)
		: UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
}

void AS3D_PlayerController::LookUpAxis(float AxisValue)
{
	Super::LookUpAxis(AxisValue);

	if (!UIManager || UIManager->IsMenuOpen()) { return; }

	AddPitchInput(AxisValue);
}

void AS3D_PlayerController::LookRightAxis(float AxisValue)
{
	Super::LookRightAxis(AxisValue);

	if (!UIManager || UIManager->IsMenuOpen()) { return; }

	AddYawInput(AxisValue);
}

void AS3D_PlayerController::OnToggleViewKeyPressed()
{
	Super::OnToggleViewKeyPressed();

	if (CameraManager)
	{
		bIsFirstPerson
			? CameraManager->UpdateActivePreset("AttachedTP")
			: CameraManager->UpdateActivePreset("AttachedFP");

		bIsFirstPerson = !bIsFirstPerson;
	}

	if (PossessedCharacter.IsValid())
	{
		PossessedCharacter->GetCharacterAnimInstance()->SetHeadVisibility(!bIsFirstPerson);
	}
}
