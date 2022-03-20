/// RemzDNB
///
///	RZ_PlayerController.cpp
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Player/RZ_PlayerController.h"
#include "Game/RZ_GameInstance.h"
#include "Game/RZ_GameMode.h"
#include "Game/RZ_GameState.h"
#include "Game/RZ_GameSettings.h"
#include "Game/RZ_WorldSettings.h"
#include "Character/RZ_Character.h"
/// ItemPlugin
#include "RZ_Item.h"
#include "RZ_ItemManagerComponent.h"
/// CameraManagerPlugin
#include "RZ_CameraManager.h"
/// UIPlugin
#include "RZ_UIManager.h"
/// Engine
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ARZ_PlayerController::ARZ_PlayerController()
{
	PlayerCameraManagerClass = ARZ_CameraManager::StaticClass();
	
	/*bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = ETickingGroup::TG_PostUpdateWork;*/
}

void ARZ_PlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	GameMode = Cast<ARZ_GameMode>(GetWorld()->GetAuthGameMode());


	WorldSettings = Cast<ARZ_WorldSettings>(GetWorld()->GetWorldSettings());
	CameraManager = Cast<ARZ_CameraManager>(PlayerCameraManager);
}

void ARZ_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	GameState = GetWorld()->GetGameState<ARZ_GameState>();
	GameSettings = Cast<URZ_GameInstance>(GetGameInstance())->GetGameSettings();

	UpdateControlSettings(WorldSettings->DefaultControlSettingsPresetName);
	
	if (IsLocalController())
	{
		UIManager = Cast<ARZ_UIManager>(GetHUD());

		UIManager->CreateMenuWidget(GameSettings->LoadoutMenuWidgetClass, "Loadout");
		//UIManager->CreateMenuWidget(GameSettings->DevWidgetClass, "Dev");
		UIManager->CreateHUDWidget(GameSettings->LoadoutHUDWidgetClass);
	}
	
	if (GetLocalRole() == ROLE_Authority)
	{
		PossessedCharacter = Cast<ARZ_Character>(GameMode->SpawnPawn(nullptr));
		if (PossessedCharacter.IsValid())
		{
			Possess(PossessedCharacter.Get());
			OnRep_Pawn();
		}
	}
}

void ARZ_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/// Update TargetLocation
	
	if (IsLocalController())
	{
		if (bShowMouseCursor)
		{
			FHitResult CursorHitResult;
			GetHitResultUnderCursorByChannel(
				UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2),
				false,
				CursorHitResult
			);
			TargetLocation = CursorHitResult.Location;
		}
		else
		{
			TargetLocation = FVector::ZeroVector;
		}

		UpdateTargetLocation(TargetLocation);
		
		if (GetLocalRole() < ROLE_Authority)
			UpdateTargetLocation_Server(TargetLocation);
	}

	/// Lerp ControlRotation

	if (ControlSettings.bLerpControlRotation)
	{
		SetControlRotation(UKismetMathLibrary::RLerp(GetControlRotation(), TargetControlRotation, DeltaTime * 5.0f, true));
	}
}

void ARZ_PlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	PossessedCharacter = Cast<ARZ_Character>(GetPawn());
	//CameraPawn->UpdateMode(ERZ_CameraMoveMode::Follow, WorldSettings->DefaultCameraViewMode, PossessedCharacter);
	
	/*if (PossessedCharacter)
	{
		
		if (WorldSettings->DefaultPawnControlMode == ERZ_PawnControlMode::TopDown)
			CameraPawn->UpdateMode(ERZ_CameraMoveMode::Follow, ERZ_CameraViewMode::ThirdPersonCenter, PossessedCharacter);
		else if (WorldSettings->DefaultPawnControlMode == ERZ_PawnControlMode::ThirdPerson)
			CameraPawn->UpdateMode(ERZ_CameraMoveMode::Follow, ERZ_CameraViewMode::ThirdPersonRight, PossessedCharacter);
		//else if (WorldSettings->DefaultPawnControlMode == ERZ_PawnControlMode::Strategic)
	}*/
}

void ARZ_PlayerController::UpdateControlSettings(const FName& NewPresetName)
{
	const UDataTable* ControlSettingsPresetsDT = GameSettings->ControlSettingsPresets;
	if (ControlSettingsPresetsDT == nullptr)
		return;

	const FString ContextString;
	const FRZ_ControlSettings* ControlSettingsPreset = ControlSettingsPresetsDT->FindRow<FRZ_ControlSettings>(
		NewPresetName,
		ContextString
	);
	if (ControlSettingsPreset == nullptr)
		return;

	ControlSettingsPresetName = NewPresetName;
	ControlSettings = *ControlSettingsPreset;
	
	// Control

	if (ControlSettings.bLerpControlRotation)
	{
		if (ControlSettings.bResetYaw)
		{
			TargetControlRotation = FRotator(ControlSettings.PitchDefault, 0.0f, 0.0f);
		}
		else
		{
			TargetControlRotation = FRotator(ControlSettings.PitchDefault, GetControlRotation().Yaw, 0.0f);
		}	
	}
	else
	{
		if (ControlSettings.bResetYaw)
		{
			SetControlRotation(FRotator(ControlSettings.PitchDefault, 0.0f, 0.0f));
		}
		else
		{
			SetControlRotation(FRotator(ControlSettings.PitchDefault, GetControlRotation().Yaw, 0.0f));
		}	
	}


	// Camera
	
	if (PossessedCharacter.IsValid())
		PossessedCharacter->bRotateToTarget = !ControlSettings.bShowMouseCursor;

	CameraManager->UpdateActivePreset(ControlSettings.DefaultCameraSettingsPresetName);

	// UI

	if (UIManager)
		UIManager->ToggleMenu(false);
	
	SetShowMouseCursor(ControlSettings.bShowMouseCursor);
}

void ARZ_PlayerController::UpdateTargetLocation(const FVector& NewTargetLocation)
{
	TargetLocation = NewTargetLocation;

	if (PossessedCharacter.IsValid())
	{
		PossessedCharacter->TargetLocation = NewTargetLocation;
	}
}

void ARZ_PlayerController::UpdateTargetLocation_Server_Implementation(const FVector& NewTargetLocation)
{
	UpdateTargetLocation(NewTargetLocation);
}

#pragma region +++++ Input ...

void ARZ_PlayerController::UpdateTargetFromCursor()
{
	FHitResult CursorHitResult;
	GetHitResultUnderCursorByChannel(
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2),
		false,
		CursorHitResult
	);
//ECC_GameTraceChannel2
	if (PossessedCharacter.IsValid())
	{
		PossessedCharacter->TargetLocation = CursorHitResult.Location;
	}
	
	/*RotationPoint = CursorToPlaneHit.ImpactPoint;

	FVector CharacterLocation(PossessedCharacter->GetActorLocation().X, PossessedCharacter->GetActorLocation().Y,
	                          GInstance->GetGlobalData().ProjectilePlaneHeight);
	if (FVector::Dist(RotationPoint, CharacterLocation) < 150.0f)
	{
		//FVector Direction = PossessedCharacter->GetActorRotation().Vector(); // look at rotation
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(CharacterLocation, RotationPoint);
		FVector Direction = Rotation.Vector();
		FVector Location = CharacterLocation + (Direction * 150.0f);
		TargetPoint = Location;
	}
	else
		TargetPoint = CursorToPlaneHit.ImpactPoint;

	PossessedCharacter->SetTargetPoint(TargetPoint);

	if (GetLocalRole() < ROLE_Authority)
		PossessedCharacter->SetTargetPoint_Server(TargetPoint);*/
	
}

void ARZ_PlayerController::UpdateTargetFromScreenCenter()
{
	const FVector Start = CameraManager->GetCameraLocation();
	const FVector End = Start + (CameraManager->GetCameraRotation().Vector() * 50000);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetPawn());
	FHitResult HitResult;

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);
	if (HitResult.bBlockingHit)
	{
		if (PossessedCharacter.IsValid())
		{
			PossessedCharacter->TargetLocation = HitResult.Location;
		}
	}
}

void ARZ_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	InputComponent->BindAxis("LookUpAxis", this, &ARZ_PlayerController::LookUpAxis).bConsumeInput = false;
	InputComponent->BindAxis("LookRightAxis", this, &ARZ_PlayerController::LookRightAxis).bConsumeInput = false;
	InputComponent->BindAxis("CameraZoomAxis", this, &ARZ_PlayerController::CameraZoomAxis);
	InputComponent->BindAxis("CameraRotationAxis", this, &ARZ_PlayerController::CameraRotationAxis);
	InputComponent->BindAxis("MoveForwardAxis", this, &ARZ_PlayerController::MoveForwardAxis);
	InputComponent->BindAxis("MoveRightAxis", this, &ARZ_PlayerController::MoveRightAxis);
	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &ARZ_PlayerController::OnLeftMouseButtonPressed);
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &ARZ_PlayerController::OnLeftMouseButtonReleased);
	InputComponent->BindAction("RightMouseButton", IE_Pressed, this, &ARZ_PlayerController::OnRightMouseButtonPressed);
	InputComponent->BindAction("RightMouseButton", IE_Released, this, &ARZ_PlayerController::OnRightMouseButtonReleased);
	InputComponent->BindAction("MiddleMouseButton", IE_Pressed, this, &ARZ_PlayerController::OnMiddleMouseButtonPressed);
	InputComponent->BindAction("MiddleMouseButton", IE_Released, this, &ARZ_PlayerController::OnMiddleMouseButtonReleased);
	InputComponent->BindAction("CameraRotateRight", IE_Pressed, this, &ARZ_PlayerController::OnCameraRotateRightKeyPressed);
	InputComponent->BindAction("CameraRotateLeft", IE_Pressed, this, &ARZ_PlayerController::OnCameraRotateLeftKeyPressed);
	InputComponent->BindAction("Use", IE_Pressed, this, &ARZ_PlayerController::OnUseKeyPressed);
	InputComponent->BindAction("OpenMenu", IE_Pressed, this, &ARZ_PlayerController::OnTabKeyPressed);
	InputComponent->BindAction("Shift", IE_Pressed, this, &ARZ_PlayerController::OnShiftKeyPressed);
	InputComponent->BindAction("SpaceBar", IE_Pressed, this, &ARZ_PlayerController::OnSpaceBarKeyPressed);
	InputComponent->BindAction("QuickSlot1", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot1Pressed);
	InputComponent->BindAction("QuickSlot2", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot2Pressed);
	InputComponent->BindAction("QuickSlot3", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot3Pressed);
	InputComponent->BindAction("QuickSlot4", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot4Pressed);
	InputComponent->BindAction("QuickSlot5", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot5Pressed);
}

void ARZ_PlayerController::LookUpAxis(float AxisValue)
{
	if (ControlSettings.bPitchFromMouseInput)
		AddPitchInput(AxisValue);
}

void ARZ_PlayerController::LookRightAxis(float AxisValue)
{
	if (ControlSettings.bYawFromMouseInput)
		AddYawInput(AxisValue);
}

void ARZ_PlayerController::CameraZoomAxis(float AxisValue)
{
	if (CameraManager)
	{
		if (AxisValue > 0)
			CameraManager->ZoomIn();

		if (AxisValue < 0)
			CameraManager->ZoomOut();
	}
}

void ARZ_PlayerController::CameraRotationAxis(float AxisValue)
{
}

void ARZ_PlayerController::MoveForwardAxis(float AxisValue)
{
	if (PossessedCharacter.IsValid())
	{
		const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);

		PossessedCharacter->AddMovementInput(Direction, AxisValue);
	}
}

void ARZ_PlayerController::MoveRightAxis(float AxisValue)
{
	if (PossessedCharacter.IsValid())
	{
		const FRotator YawRotation(0, GetControlRotation().Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);

		PossessedCharacter->AddMovementInput(Direction, AxisValue);
	}
}

void ARZ_PlayerController::OnLeftMouseButtonPressed()
{
	if (PossessedCharacter.IsValid() &&
		PossessedCharacter->GetItemManager() &&
		PossessedCharacter->GetItemManager()->GetEquippedItem())
	{
		PossessedCharacter->GetItemManager()->GetEquippedItem()->SetWantsToUse(true);
	}
}

void ARZ_PlayerController::OnLeftMouseButtonReleased()
{
	if (PossessedCharacter.IsValid() &&
		PossessedCharacter->GetItemManager() &&
		PossessedCharacter->GetItemManager()->GetEquippedItem())
	{
		PossessedCharacter->GetItemManager()->GetEquippedItem()->SetWantsToUse(false);
	}
}

void ARZ_PlayerController::OnRightMouseButtonPressed()
{
}

void ARZ_PlayerController::OnRightMouseButtonReleased()
{
}

void ARZ_PlayerController::OnMiddleMouseButtonPressed()
{
}

void ARZ_PlayerController::OnMiddleMouseButtonReleased()
{
}

void ARZ_PlayerController::OnUseKeyPressed()
{
}

void ARZ_PlayerController::OnTabKeyPressed()
{
	if (UIManager && WorldSettings->DefaultGameType == ERZ_GameType::Battle)
	{
		if (UIManager->IsMenuOpen())
		{
			UIManager->ToggleMenu(false);
			SetShowMouseCursor(ControlSettings.bShowMouseCursor);
		}
		else
		{
			UIManager->ToggleMenu(true);
			SetShowMouseCursor(true);
		}
	}
}

void ARZ_PlayerController::OnShiftKeyPressed()
{
}

void ARZ_PlayerController::OnSpaceBarKeyPressed()
{
}

void ARZ_PlayerController::OnCameraRotateRightKeyPressed()
{
	TargetControlRotation.Yaw = TargetControlRotation.Yaw + 90.0f;
}

void ARZ_PlayerController::OnCameraRotateLeftKeyPressed()
{
	TargetControlRotation.Yaw = TargetControlRotation.Yaw - 90.0f;
}

void ARZ_PlayerController::OnQuickSlot1Pressed()
{
	if (PossessedCharacter.IsValid())
	{
		PossessedCharacter->GetItemManager()->EquipItem(0);
	}
}

void ARZ_PlayerController::OnQuickSlot2Pressed()
{
	if (PossessedCharacter.IsValid())
	{
		PossessedCharacter->GetItemManager()->EquipItem(1);
	}
}

void ARZ_PlayerController::OnQuickSlot3Pressed()
{
	if (PossessedCharacter.IsValid())
	{
		PossessedCharacter->GetItemManager()->EquipItem(2);
	}
}

void ARZ_PlayerController::OnQuickSlot4Pressed()
{
	if (PossessedCharacter.IsValid())
	{
		PossessedCharacter->GetItemManager()->EquipItem(3);
	}
}

void ARZ_PlayerController::OnQuickSlot5Pressed()
{
	if (PossessedCharacter.IsValid())
	{
		PossessedCharacter->GetItemManager()->EquipItem(4);
	}
}

#pragma endregion
