/// RemzDNB

#include "Core/RZ_PlayerController.h"
#include "Core/RZ_GameInstance.h"
#include "Core/RZ_GameMode.h"
#include "Core/RZ_GameState.h"
#include "Core/RZ_WorldSettings.h"
#include "Character/RZ_Character.h"
#include "Character/RZ_CharacterMovementComponent.h"
#include "UI/RZ_HUDLayoutWidget.h"
// CameraManager plugin
#include "RZ_CameraManager.h"
// InventorySystem plugin
#include "RZ_InventoryComponent.h"
// UserInterface plugin
#include "RZ_UIManager.h"
#include "RZ_InventoryMenuWidget.h"
#include "RZ_InventoryHUDWidget.h"
#include "RZ_DamageMarkerWidget.h"
// Engine
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "AI/RZ_PawnAIController.h"
#include "UI/RZ_ActorManager_MainWidget.h"

#pragma region +++ Setup ...

ARZ_PlayerController::ARZ_PlayerController()
{
	PlayerCameraManagerClass = ARZ_CameraManager::StaticClass();

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	
	InteractionMode = ERZ_ControllerInteractionMode::Selection;
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

	DefaultInputMode.SetHideCursorDuringCapture(false);
	SetInputMode(DefaultInputMode);
	SetShowMouseCursor(true);
	
	if (CameraManager)
	{
		UpdateControlSettings("TopDown");
		CameraManager->UpdateActivePreset("AttachedTD");

		if (UIManager)
			UIManager->ToggleMenu(false);
	}
	
	// UI setup.
	
	if (IsLocalController())
	{
		UIManager = Cast<ARZ_UIManager>(GetHUD());
		if (UIManager)
		{
			/*InventoryMenuWidget = Cast<URZ_InventoryMenuWidget>(
				UIManager->CreateMenuWidget(GameSettings->LoadoutMenuWidgetClass, "Loadout")
			);
			ActorManagerWidget = Cast<URZ_ActorManager_MainWidget>(
				UIManager->CreateMenuWidget(GameSettings->ActorManager_MainWidgetClass, "Grid Management")
			);
			InventoryHUDWidget = Cast<URZ_InventoryHUDWidget>(
				UIManager->CreateHUDWidget(GameSettings->LoadoutHUDWidgetClass)
			);*/
			HUDLayoutWidget = Cast<URZ_HUDLayoutWidget>(UIManager->CreateHUDWidget(GameSettings->HUDLayoutWidgetClass));
		}
	}

	// Spawn character.
	
	if (GetLocalRole() == ROLE_Authority)
	{
		//GameMode->QueryRespawn(this);
	}
}

void ARZ_PlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	
	if (CameraManager)
	{
		// Set initial controller rotation. // IDK TODO : delete ?
		//CameraManager->UpdateActivePreset("TDFree");
		//CameraManager->SetLocalControlRotationYaw(GetPawn()->GetActorRotation().Yaw - 180.0f);
		//CameraPawn->UpdateMode(ERZ_CameraMoveMode::Follow, WorldSettings->DefaultCameraViewMode, PossessedCharacter);
	}
}

#pragma endregion

#pragma region +++ Tick ...

void ARZ_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/// Update TargetLocation

	UpdateCursorTraces(DeltaTime);
	UpdateHoveredActor(DeltaTime);
	
	if (IsLocalController())
	{
		//SetTargetLocation_Server(TargetLocation);
	}

	Debug(DeltaTime);
}

void ARZ_PlayerController::UpdateCursorTraces(float DeltaTime)
{
	if (!IsLocalController()) { return; }

	// Calc hit result between the cursor and any world actor, used by interaction system.
	
	GetHitResultUnderCursorByChannel(
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		CursorToWorldHitResult
	);
	
	// Intersection between the cursor and the ground, used by building system.
	
	GetHitResultUnderCursorByChannel(
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel6),
		false,
		CursorToGroundHitResult
	);
	
	// Cursor to ViewPlane : used to calculate the characters control rotation.
	
	GetHitResultUnderCursorByChannel(
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2),
		false,
		CursorToViewPlaneHitResult
	);
	
	/*if (PossessedCharacter.IsValid())
	{
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(GetPawn());
		const FVector TargetTraceStartLocation = FVector(
			PossessedCharacter->GetMesh()->GetBoneLocation("hand_r").X,
			PossessedCharacter->GetMesh()->GetBoneLocation("hand_r").Y,
			BASEVIEWHEIGHT
		);
		const FVector TargetTraceEndLocation =
			TargetTraceStartLocation +
			UKismetMathLibrary::FindLookAtRotation(TargetTraceStartLocation, CursorToViewPlaneHitResult.ImpactPoint).
			Vector() * 10000.0f;
		GetWorld()->LineTraceSingleByChannel(
			S2DTargetHitResult,
			TargetTraceStartLocation,
			TargetTraceEndLocation,
			ECC_Visibility,
			TraceParams
		);
	}*/
}

void ARZ_PlayerController::UpdateCrosshairTrace(float TraceStartHeight, float DeltaTime)
{
	// no, everyone trace, no remote controller if (!IsLocalController() && GetLocalRole() != ROLE_Authority) { return; }
	// both server and local clients
	
	if (!GetPawn()) { return; }
	
	const FVector TraceStart = FVector(
		GetPawn()->GetActorLocation().X,
		GetPawn()->GetActorLocation().Y,
		TraceStartHeight
	);
	const FVector TraceEnd = TraceStart + (GetControlRotation().Vector() * TRACEMAXLENGTH);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetPawn());

	GetWorld()->LineTraceSingleByChannel(
		CrosshairToWorldHitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		TraceParams
	);

	if (GameSettings->bDebugPlayerController)
	{
		DrawDebugSphere(GetWorld(), CrosshairToWorldHitResult.ImpactPoint, 20.0f, 10, FColor::Red, false, DeltaTime + 0.05f);
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, .3f, 0, 3.0f);
	}
}

void ARZ_PlayerController::SetTargetLocation(const FVector& NewTargetLocation)
{
	TargetLocation = NewTargetLocation;

	if (!SelectedCharacter.IsValid()) { return; }

	if (SelectedCharacter->GetInventoryComponent())
	{
		SelectedCharacter->GetInventoryComponent()->PlayerTargetLocation = CursorToGroundHitResult.Location;
	}
}

void ARZ_PlayerController::SetTargetLocation_Server_Implementation(const FVector& NewTargetLocation)
{
	SetTargetLocation(NewTargetLocation);
}

void ARZ_PlayerController::UpdateHoveredActor(float DeltaTime)
{
	IRZ_WorldInteractionInterface* LastHoveredActorInterface = Cast<IRZ_WorldInteractionInterface>(LastHoveredActor);
	if (LastHoveredActorInterface && LastHoveredActor != CursorToWorldHitResult.Actor)
	{
		LastHoveredActorInterface->OnWorldHover(false);
	}

	IRZ_WorldInteractionInterface* NewHoveredActorInterface = Cast<IRZ_WorldInteractionInterface>(CursorToWorldHitResult.Actor);
	if (NewHoveredActorInterface && LastHoveredActor != CursorToWorldHitResult.Actor)
	{
		NewHoveredActorInterface->OnWorldHover(true);
	}

	LastHoveredActor = CursorToWorldHitResult.Actor;
}

#pragma endregion

void ARZ_PlayerController::SetSelectedCharacter(ARZ_Character* CharacterToSelect)
{
	if (SelectedCharacter.Get() == CharacterToSelect) { return; }
	
	SelectedCharacter = CharacterToSelect;
	OnNewCharacterSelected.Broadcast(CharacterToSelect);
}

void ARZ_PlayerController::SetSelectedTargetPawn(APawn* TargetPawnToSelect)
{
			if (SelectedTargetPawn.Get() == TargetPawnToSelect) { return; }
		SelectedTargetPawn = TargetPawnToSelect;
		OnNewTargetPawnSelected.Broadcast(TargetPawnToSelect);

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

	if (ControlSettings.bResetYaw)
	{
		SetControlRotation(FRotator(ControlSettings.PitchDefault, 0.0f, 0.0f));
	}
	else
	{
		SetControlRotation(FRotator(ControlSettings.PitchDefault, GetControlRotation().Yaw, 0.0f));
	}	
	
	// Camera

	if (CameraManager)
	{
		//CameraManager->UpdateActivePreset(ControlSettings.CameraSettingsPresetName);
	}
}

void ARZ_PlayerController::UpdateInteractionMode(ERZ_ControllerInteractionMode NewInteractionMode)
{
	if (NewInteractionMode == InteractionMode) { return; }

	InteractionMode = NewInteractionMode;
	
	OnControllerInteractionModeUpdated.Broadcast(InteractionMode);
}

#pragma region +++ UI ...

void ARZ_PlayerController::OnDamageDealt_Client_Implementation(float Amount, const FVector& Location)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("ARZ_PlayerController::OnDamageDealt_Client_Implementation"));;
	
	URZ_DamageMarkerWidget* NewDamageMarker = CreateWidget<URZ_DamageMarkerWidget>(
		GetWorld(),
		GameSettings->DamageMarkerWidgetClass
	);
	if (NewDamageMarker)
	{
		NewDamageMarker->Init(Amount, Location);
		NewDamageMarker->AddToViewport();
		//UIManager->AddHUDWidget(NewDamageMarker, false);
	}
}

void ARZ_PlayerController::OnMenuOpened(bool bNewIsVisible)
{
}

#pragma endregion

#pragma region +++ Input ...

void ARZ_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);
	
	InputComponent->BindAxis("LookUpAxis", this, &ARZ_PlayerController::LookUpAxis).bConsumeInput = false;
	InputComponent->BindAxis("LookRightAxis", this, &ARZ_PlayerController::LookRightAxis).bConsumeInput = false;
	InputComponent->BindAction("LookUpKey", IE_Pressed, this, &ARZ_PlayerController::OnLookUpKeyPressed);
	InputComponent->BindAction("LookDownKey", IE_Pressed, this, &ARZ_PlayerController::OnLookDownKeyPressed);
	InputComponent->BindAction("LookRightKey", IE_Pressed, this, &ARZ_PlayerController::OnLookRightKeyPressed);
	InputComponent->BindAction("LookLeftKey", IE_Pressed, this, &ARZ_PlayerController::OnLookLeftKeyPressed);
	InputComponent->BindAction("ZoomInKey", IE_Pressed, this, &ARZ_PlayerController::OnZoomInKeyPressed);
	InputComponent->BindAction("ZoomOutKey", IE_Pressed, this, &ARZ_PlayerController::OnZoomOutKeyPressed);
	InputComponent->BindAction("ToggleViewKey", IE_Pressed, this, &ARZ_PlayerController::OnToggleViewKeyPressed);
	
	InputComponent->BindAxis("MoveForwardAxis", this, &ARZ_PlayerController::MoveForwardAxis);
	InputComponent->BindAxis("MoveRightAxis", this, &ARZ_PlayerController::MoveRightAxis);
	InputComponent->BindAction("RunKey", IE_Pressed, this, &ARZ_PlayerController::OnRunKeyPressed);
	InputComponent->BindAction("RunKey", IE_Released, this, &ARZ_PlayerController::OnRunKeyReleased);
	InputComponent->BindAction("JumpKey", IE_Pressed, this, &ARZ_PlayerController::OnJumpKeyPressed);
	
	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &ARZ_PlayerController::OnLeftMouseButtonPressed);
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &ARZ_PlayerController::OnLeftMouseButtonReleased);
	InputComponent->BindAction("RightMouseButton", IE_Pressed, this, &ARZ_PlayerController::OnRightMouseButtonPressed);
	InputComponent->BindAction("RightMouseButton", IE_Released, this, &ARZ_PlayerController::OnRightMouseButtonReleased);
	InputComponent->BindAction("MiddleMouseButton", IE_Pressed, this, &ARZ_PlayerController::OnMiddleMouseButtonPressed);
	InputComponent->BindAction("MiddleMouseButton", IE_Released, this, &ARZ_PlayerController::OnMiddleMouseButtonReleased);
	InputComponent->BindAction("Use", IE_Pressed, this, &ARZ_PlayerController::OnUseKeyPressed);

	InputComponent->BindAction("RKey", IE_Pressed, this, &ARZ_PlayerController::OnRKeyPressed);
	
	InputComponent->BindAction("ToggleMenuKey", IE_Pressed, this, &ARZ_PlayerController::OnToggleMenuKeyPressed);
	InputComponent->BindAction("QuickSlot1Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot1KeyPressed);
	InputComponent->BindAction("QuickSlot2Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot2KeyPressed);
	InputComponent->BindAction("QuickSlot3Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot3KeyPressed);
	InputComponent->BindAction("QuickSlot4Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot4KeyPressed);
	InputComponent->BindAction("QuickSlot5Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot5KeyPressed);
	InputComponent->BindAction("QuickSlot6Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot6KeyPressed);
	InputComponent->BindAction("QuickSlot7Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot7KeyPressed);
	InputComponent->BindAction("QuickSlot8Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot8KeyPressed);
	InputComponent->BindAction("QuickSlot9Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot9KeyPressed);
	InputComponent->BindAction("HolsterKey", IE_Pressed, this, &ARZ_PlayerController::OnHolsterKeyPressed);
	InputComponent->BindAction("QuickBarUpKey", IE_Pressed, this, &ARZ_PlayerController::OnQuickBarUpKeyPressed);
	InputComponent->BindAction("QuickBarDownKey", IE_Pressed, this, &ARZ_PlayerController::OnQuickBarDownKeyPressed);
}

void ARZ_PlayerController::LookUpAxis(float AxisValue)
{
}

void ARZ_PlayerController::LookRightAxis(float AxisValue)
{
}

void ARZ_PlayerController::OnLookUpKeyPressed()
{

}

void ARZ_PlayerController::OnLookDownKeyPressed()
{

}

void ARZ_PlayerController::OnLookRightKeyPressed()
{

}

void ARZ_PlayerController::OnLookLeftKeyPressed()
{
}

void ARZ_PlayerController::OnZoomInKeyPressed()
{
	if (CameraManager && UIManager->IsMenuOpen() == false)
	{
		CameraManager->ZoomIn();
	}
}

void ARZ_PlayerController::OnZoomOutKeyPressed()
{
	if (CameraManager && UIManager->IsMenuOpen() == false)
	{
		CameraManager->ZoomOut();
	}
}

void ARZ_PlayerController::OnToggleViewKeyPressed()
{
}

void ARZ_PlayerController::MoveForwardAxis(float AxisValue)
{
	if (CameraManager)
	{
		CameraManager->MoveManualTargetLocationForward(AxisValue);
	}
	/*if (PossessedCharacter.IsValid() && CameraManager)
	{
		const FRotator YawRotation(0, CameraManager->GetFinalCameraRotation().Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);

		PossessedCharacter->AddMovementInput(Direction, AxisValue);
	}*/
}

void ARZ_PlayerController::MoveRightAxis(float AxisValue)
{
	if (CameraManager)
	{
		CameraManager->MoveManualTargetLocationRight(AxisValue);
	}
	/*if (PossessedCharacter.IsValid())
	{
		const FRotator YawRotation(0, CameraManager->GetFinalCameraRotation().Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);

		PossessedCharacter->AddMovementInput(Direction, AxisValue);
	}*/
}

void ARZ_PlayerController::OnRunKeyPressed()
{
	if (SelectedCharacter.IsValid() && SelectedCharacter->GetCharacterMovement())
	{
		URZ_CharacterMovementComponent* MovementComp = Cast<URZ_CharacterMovementComponent>(
			SelectedCharacter->GetCharacterMovement()
		);
		if (MovementComp)
		{
			MovementComp->SetSprinting(true);
		}
	}
}

void ARZ_PlayerController::OnRunKeyReleased()
{
	if (SelectedCharacter.IsValid() && SelectedCharacter->GetCharacterMovement())
	{
		URZ_CharacterMovementComponent* MovementComp = Cast<URZ_CharacterMovementComponent>(
			SelectedCharacter->GetCharacterMovement()
		);
		if (MovementComp)
		{
			MovementComp->SetSprinting(false);
		}
	}
}

void ARZ_PlayerController::OnJumpKeyPressed()
{
}

void ARZ_PlayerController::OnRKeyPressed()
{
}

void ARZ_PlayerController::OnLeftMouseButtonPressed()
{
	if (LastHoveredActor.IsValid())
	{
		ARZ_Character* LastHoveredCharacter = Cast<ARZ_Character>(LastHoveredActor);
		if (LastHoveredCharacter)
		{
			SetSelectedCharacter(LastHoveredCharacter);
			return;
		}
	}

	//
	
	if (!SelectedCharacter.IsValid()) { return; }
	
	const URZ_InventoryComponent* CharacterInventoryComp = SelectedCharacter->GetInventoryComponent();
	if (!SelectedCharacter->GetInventoryComponent()) { return; }

	IRZ_InventoryItemInterface* EquippedInventoryItemInterface = Cast<IRZ_InventoryItemInterface>(
		CharacterInventoryComp->GetStorageSlots()[CharacterInventoryComp->GetEquippedStorageSlotID()].SpawnedActor
	);
	if (EquippedInventoryItemInterface)
	{
		EquippedInventoryItemInterface->OnWantToBeUsedByInventory(true);
		return;
	}
	
	//


}

void ARZ_PlayerController::OnLeftMouseButtonReleased()
{
	if (LastHoveredActor.IsValid())
	{
		APawn* LastHoveredPawn = Cast<APawn>(LastHoveredActor);
		if (LastHoveredPawn)
		{
			//SetSelectedCharacter(Last)
		}
	}
	
	if (!SelectedCharacter.IsValid()) { return; }
	
	const URZ_InventoryComponent* CharacterInventoryComp = SelectedCharacter->GetInventoryComponent();
	if (!SelectedCharacter->GetInventoryComponent()) { return; }
	
	IRZ_InventoryItemInterface* EquippedInventoryItemInterface = Cast<IRZ_InventoryItemInterface>(
		CharacterInventoryComp->GetStorageSlots()[CharacterInventoryComp->GetEquippedStorageSlotID()].SpawnedActor
	);
	if (EquippedInventoryItemInterface)
	{
		EquippedInventoryItemInterface->OnWantToBeUsedByInventory(false);
		return;
	}
}

void ARZ_PlayerController::OnRightMouseButtonPressed()
{
	if (LastHoveredActor.IsValid())
	{
		APawn* LastHoveredPawn = Cast<APawn>(LastHoveredActor);
		if (LastHoveredPawn)
		{
			SetSelectedTargetPawn(LastHoveredPawn);
		}
	}
	
	if (SelectedCharacter.IsValid())
	{
		ARZ_PawnAIController* PawnAIController = Cast<ARZ_PawnAIController>(SelectedCharacter->GetController());
		if (PawnAIController)
		{
			//PawnAIController->AddNewDelayedAction("Move", CursorToGroundHitResult.Location, nullptr);;
			PawnAIController->SetMoveToLocation(CursorToGroundHitResult.Location);
		}
	}
	
	//PossessedCharacter->GetInventoryComponent()->AddActorToStorageSlot(false, CursorToWorldHitResult.Actor.Get());
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

void ARZ_PlayerController::OnToggleMenuKeyPressed()
{
	if (UIManager && WorldSettings->DefaultGameType == ERZ_GameType::Battle)
	{
		const bool bIsMenuOpen = UIManager->IsMenuOpen();
		UIManager->ToggleMenu(!bIsMenuOpen);
		UIManager->ToggleHUD(bIsMenuOpen);
		OnMenuOpened(!bIsMenuOpen);
	}
}

void ARZ_PlayerController::OnQuickSlot1KeyPressed()
{
	if (SelectedCharacter.IsValid() && SelectedCharacter->GetInventoryComponent())
	{
		SelectedCharacter->GetInventoryComponent()->EquipStorageItem(1);
	}
}

void ARZ_PlayerController::OnQuickSlot2KeyPressed()
{
	if (SelectedCharacter.IsValid() && SelectedCharacter->GetInventoryComponent())
	{
		SelectedCharacter->GetInventoryComponent()->EquipStorageItem(2);
	}
}

void ARZ_PlayerController::OnQuickSlot3KeyPressed()
{
	if (SelectedCharacter.IsValid() && SelectedCharacter->GetInventoryComponent())
	{
		SelectedCharacter->GetInventoryComponent()->EquipStorageItem(3);
	}
}

void ARZ_PlayerController::OnQuickSlot4KeyPressed()
{
	if (SelectedCharacter.IsValid() && SelectedCharacter->GetInventoryComponent())
	{
		SelectedCharacter->GetInventoryComponent()->EquipStorageItem(4);
	}
}

void ARZ_PlayerController::OnQuickSlot5KeyPressed()
{
	if (SelectedCharacter.IsValid() && SelectedCharacter->GetInventoryComponent())
	{
		SelectedCharacter->GetInventoryComponent()->EquipStorageItem(5);
	}
}

void ARZ_PlayerController::OnQuickSlot6KeyPressed()
{
	if (SelectedCharacter.IsValid() && SelectedCharacter->GetInventoryComponent())
	{
		SelectedCharacter->GetInventoryComponent()->EquipStorageItem(6);
	}
}

void ARZ_PlayerController::OnQuickSlot7KeyPressed()
{
	if (SelectedCharacter.IsValid() && SelectedCharacter->GetInventoryComponent())
	{
		SelectedCharacter->GetInventoryComponent()->EquipStorageItem(7);
	}
}

void ARZ_PlayerController::OnQuickSlot8KeyPressed()
{
	if (SelectedCharacter.IsValid() && SelectedCharacter->GetInventoryComponent())
	{
		SelectedCharacter->GetInventoryComponent()->EquipStorageItem(8);
	}
}

void ARZ_PlayerController::OnQuickSlot9KeyPressed()
{
	if (SelectedCharacter.IsValid() && SelectedCharacter->GetInventoryComponent())
	{
		SelectedCharacter->GetInventoryComponent()->EquipStorageItem(9);
	}
}

void ARZ_PlayerController::OnHolsterKeyPressed()
{
	if (SelectedCharacter.IsValid() && SelectedCharacter->GetInventoryComponent())
	{
		SelectedCharacter->GetInventoryComponent()->EquipStorageItem(0);
	}
}

void ARZ_PlayerController::OnQuickBarUpKeyPressed()
{
	if (SelectedCharacter.IsValid() && SelectedCharacter->GetInventoryComponent())
	{
		SelectedCharacter->GetInventoryComponent()->SelectNextQuickBar(true);
	}
}

void ARZ_PlayerController::OnQuickBarDownKeyPressed()
{
	if (SelectedCharacter.IsValid() && SelectedCharacter->GetInventoryComponent())
	{
		SelectedCharacter->GetInventoryComponent()->SelectNextQuickBar(false);
	}
}

#pragma endregion

void ARZ_PlayerController::SpawnAIController()
{
	if (GameMode)
	{
		GameMode->AddAIController();
	}
}

void ARZ_PlayerController::AddInventoryItem(const FName& ItemName)
{
	if (!SelectedCharacter.IsValid()) { return; }
	if (!SelectedCharacter->GetInventoryComponent()) { return; }

	//PossessedCharacter->GetInventoryComponent()->AddStorageItem(true, nullptr, ItemName);
}


void ARZ_PlayerController::Debug(float DeltaTime)
{
	if (!GameSettings) { return; }
	if (!GameSettings->bDebugPlayerController) { return; }
	
	if (CursorToWorldHitResult.Actor.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan,
			FString::Printf(TEXT("ARZ_PlayerController::CalcCursorToWorld : Hit Actor = %s"),
				*CursorToWorldHitResult.Actor->GetName())
		);;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan,
			FString::Printf(TEXT("ARZ_PlayerController::CalcCursorToWorld : Hit Actor = nullptr")));
	}
	
	UKismetSystemLibrary::DrawDebugSphere(GetWorld(), CursorToGroundHitResult.ImpactPoint, 25.0f, 8, FColor::Yellow, DeltaTime + 0.1f, 1.0f);
	UKismetSystemLibrary::DrawDebugSphere(GetWorld(), CursorToViewPlaneHitResult.ImpactPoint, 5.0f, 8, FColor::White, DeltaTime + 0.1f, 0.5f);
	
	//const FString PlayerControllerModeString = RZ_CommonLibrary::GetEnumAsString("ERZ_PlayerControllerMode", InteractionMode);
	//RZ_CommonLibrary::PrintStringToScreen("PlayerControllerMode == ", PlayerControllerModeString, FColor::Cyan, -1, DeltaTime);
	//const FString String = RZ_CommonLibrary::GetEnumAsString("ERZ_ControlRotationMode", ControlRotationMode);

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, " ");
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, "--------------------------------------------------");
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, this->GetName());
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, "--------------------------------------------------");
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, " ");
}