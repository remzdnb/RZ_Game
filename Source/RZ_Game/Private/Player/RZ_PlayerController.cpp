/// RemzDNB

#include "Player/RZ_PlayerController.h"
#include "Game/RZ_GameInstance.h"
#include "Game/RZ_GameMode.h"
#include "Game/RZ_GameState.h"
#include "Game/RZ_GameSettings.h"
#include "Game/RZ_WorldSettings.h"
#include "Character/RZ_Character.h"
#include "Character/RZ_CharacterMovementComponent.h"
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

#pragma region +++ Setup ...

ARZ_PlayerController::ARZ_PlayerController()
{
	PlayerCameraManagerClass = ARZ_CameraManager::StaticClass();
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	
	PlayerControllerMode = ERZ_PlayerControllerMode::None;

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
	
	/*bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
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

	// UI setup.
	
	if (IsLocalController())
	{
		UIManager = Cast<ARZ_UIManager>(GetHUD());
		if (UIManager)
		{
			InventoryMenuWidget = Cast<URZ_InventoryMenuWidget>(
				UIManager->CreateMenuWidget(GameSettings->LoadoutMenuWidgetClass, "Loadout")
			);
			InventoryHUDWidget = Cast<URZ_InventoryHUDWidget>(
				UIManager->CreateHUDWidget(GameSettings->LoadoutHUDWidgetClass)
			);
			//UIManager->CreateMenuWidget(GameSettings->DevWidgetClass, "Dev");
		}
	}

	// Spawn character.
	
	if (GetLocalRole() == ROLE_Authority)
	{
		GameMode->QueryRespawn(this);
	}
}

void ARZ_PlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	PossessedCharacter = Cast<ARZ_Character>(GetPawn());
	if (!PossessedCharacter.IsValid()) { return; }

	PlayerControllerMode = ERZ_PlayerControllerMode::PawnControl;
	
	// Subscribe to character components delegates.
	
	if (GetLocalRole() == ROLE_Authority)
	{
		if (PossessedCharacter.IsValid())
		{
			IRZ_PawnInterface* CombatInterface = Cast<IRZ_PawnInterface>(PossessedCharacter);
			if (CombatInterface)
			{
				CombatInterface->SetPawnOwnerShip(ERZ_PawnOwnership::Player);
			}
			
			if (PossessedCharacter->GetPawnCombatComponent())
			{
				PossessedCharacter->GetPawnCombatComponent()->OnDamageTaken.AddUniqueDynamic(
					this,
					&ARZ_PlayerController::OnCharacterDamaged
				);
			}

			if (PossessedCharacter->GetInventoryComponent())
			{
				PossessedCharacter->GetInventoryComponent()->OnItemSelected.AddUniqueDynamic(
					this,
					&ARZ_PlayerController::OnCharacterEquippedItem
				);

				for (const auto& ItemName : GameSettings->DefaultItems)
				{
					PossessedCharacter->GetInventoryComponent()->AddItemFromDataTable(ItemName);
				}
				
				PossessedCharacter->GetInventoryComponent()->SelectSlot(1);
				PossessedCharacter->GetInventoryComponent()->SelectSlot(0);
			}
		}
	}

	// Subscribe to UI delegates.
	
	if (IsLocalController()) // Crashes in multiplayer if not called on local controller.
		{
		InventoryMenuWidget->OnNewInventoryComponent(PossessedCharacter->GetInventoryComponent());
		InventoryHUDWidget->OnNewInventoryComponent(PossessedCharacter->GetInventoryComponent());
		}
	

	// Set initial controller rotation.

	if (CameraManager.IsValid() && ControlSettings.ControlRotationMode == ERZ_ControlRotationMode::Cursor)
	{
		CameraManager->SetLocalControlRotationYaw(GetPawn()->GetActorRotation().Yaw - 180.0f);
	}
	
	//CameraPawn->UpdateMode(ERZ_CameraMoveMode::Follow, WorldSettings->DefaultCameraViewMode, PossessedCharacter);
}

#pragma endregion

#pragma region +++ Tick ...

void ARZ_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/// Update TargetLocation

	UpdateCursorTraces(DeltaTime);
	UpdateScreenCenterTraces(DeltaTime);
	UpdateHoveredItem(DeltaTime);
	
	if (IsLocalController())
	{
		SetTargetLocation_Server(TargetLocation);
	}

	Debug(DeltaTime);
}

void ARZ_PlayerController::UpdateCursorTraces(float DeltaTime)
{
	if (!IsLocalController()) { return; }
	if (!bShowMouseCursor) { return; }

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
	
	if (PossessedCharacter.IsValid())
	{
		FHitResult TargetTraceHitResult;
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
			TargetTraceHitResult,
			TargetTraceStartLocation,
			TargetTraceEndLocation,
			ECC_Visibility,
			TraceParams
		);
		if (TargetTraceHitResult.bBlockingHit)
		{
			//boué
			TargetLocation = TargetTraceHitResult.ImpactPoint;
			SetControlRotation(UKismetMathLibrary::FindLookAtRotation(PossessedCharacter->GetActorLocation(), TargetTraceHitResult.ImpactPoint));
			PossessedCharacter->TargetLocation = TargetTraceHitResult.ImpactPoint;
		}
	}
}

void ARZ_PlayerController::UpdateScreenCenterTraces(float DeltaTime)
{
	/*if (IsLocalController() == false ||
		ControlSettings.ControlRotationMode == ERZ_ControlRotationMode::Cursor ||
		bShowMouseCursor)
	{
		return;
	}
	
	const FVector Start = CameraManager->GetCameraLocation();
	const FVector End = Start + (CameraManager->GetCameraRotation().Vector() * 50000);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetPawn());
	FHitResult HitResult;*/

	/* Get intersection between cursor and plane.
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);
	if (HitResult.bBlockingHit)
	{
		PossessedCharacter->TargetLocation = HitResult.Location;
	}*/
}

void ARZ_PlayerController::SetTargetLocation(const FVector& NewTargetLocation)
{
	TargetLocation = NewTargetLocation;

	if (!PossessedCharacter.IsValid()) { return; }
	
	PossessedCharacter->TargetLocation = NewTargetLocation;

	if (PossessedCharacter->GetInventoryComponent())
	{
		PossessedCharacter->GetInventoryComponent()->BuildTargetLocation = CursorToGroundHitResult.Location;
		
		if (PossessedCharacter->GetInventoryComponent()->GetSelectedItemInterface())
		{
			PossessedCharacter->GetInventoryComponent()->GetSelectedItemInterface()
			                  ->SetControllerTargetLocation(NewTargetLocation);
		}
	}
}

void ARZ_PlayerController::SetTargetLocation_Server_Implementation(const FVector& NewTargetLocation)
{
	SetTargetLocation(NewTargetLocation);
}

void ARZ_PlayerController::UpdateHoveredItem(float DeltaTime)
{
	IRZ_ItemInterface* ItemInterface = Cast<IRZ_ItemInterface>(CursorToWorldHitResult.Actor);
	
	if (LastHoveredItemInterface && LastHoveredItemInterface != ItemInterface)
	{
		LastHoveredItemInterface->OnHoverEnd();
		LastHoveredItemInterface->SetBuildMeshVisibility(false);
	}
	
	if (ItemInterface && ItemInterface != LastHoveredItemInterface)
	{
		ItemInterface->OnHoverStart();
		ItemInterface->SetBuildMeshVisibility(true);
	}

	LastHoveredItemInterface = ItemInterface;
}

#pragma endregion

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

	if (CameraManager.IsValid())
	{
		CameraManager->UpdateActivePreset(ControlSettings.CameraSettingsPresetName);
	}

	// UI

	if (UIManager)
		UIManager->ToggleMenu(false);
	
	SetShowMouseCursor(ControlSettings.bShowMouseCursor);
}

void ARZ_PlayerController::ToggleSpawnMode(bool bNewIsEnabled, AActor* DemoActor)
{
	if (bNewIsEnabled && DemoActor)
	{
		PlayerControllerMode = ERZ_PlayerControllerMode::Spawn;
		//SpawnManagerComp->UpdateDemoActor(GameSettings->DefaultSpawnManagerDemoActor);
		//SpawnManagerComp->ShowDemoActor(true);
	}
	else
	{
		PlayerControllerMode = ERZ_PlayerControllerMode::PawnControl;
		//SpawnManagerComp->ShowDemoActor(false);
	}

	OnPlayerControllerModeUpdated.Broadcast(PlayerControllerMode);
}

void ARZ_PlayerController::OnCharacterDamaged(const FRZ_DamageInfo& DamageInfo)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ARZ_PlayerController::OnCharacterDamaged"));

	ARZ_PlayerController* InstigatorPlayerController = Cast<ARZ_PlayerController>(DamageInfo.InstigatorController);
	if (InstigatorPlayerController)
	{
		InstigatorPlayerController->OnDamageDealt_Client(DamageInfo.Amount, DamageInfo.Location);
	}
	
	//OnDamageDealt_Client(DamageInfo.Amount, DamageInfo.Location);
}

void ARZ_PlayerController::OnCharacterEquippedItem(AActor* EquippedItem)
{
	IRZ_ItemInterface* ItemInterface = Cast<IRZ_ItemInterface>(EquippedItem);
	if (!ItemInterface) { return; }
	
	const FRZ_ItemSettings ItemSettings = ItemInterface->GetItemSettings();
	
	if (ItemSettings.Type == ERZ_ItemType::Building)
	{
		ToggleSpawnMode(true, EquippedItem);
	}
	else
	{
		ToggleSpawnMode(false);
	}
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

	InputComponent->BindAction("ToggleMenuKey", IE_Pressed, this, &ARZ_PlayerController::OnToggleMenuKeyPressed);
	InputComponent->BindAction("QuickSlot1Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot1KeyPressed);
	InputComponent->BindAction("QuickSlot2Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot2KeyPressed);
	InputComponent->BindAction("QuickSlot3Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot3KeyPressed);
	InputComponent->BindAction("QuickSlot4Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot4KeyPressed);
	InputComponent->BindAction("QuickSlot5Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot5KeyPressed);
	InputComponent->BindAction("QuickSlot6Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickSlot6KeyPressed);
	InputComponent->BindAction("QuickBarUpKey", IE_Pressed, this, &ARZ_PlayerController::OnQuickBarUpKeyPressed);
	InputComponent->BindAction("QuickBarDownKey", IE_Pressed, this, &ARZ_PlayerController::OnQuickBarDownKeyPressed);
	
	InputComponent->BindAction("ToggleSpawnModeKey", IE_Pressed, this, &ARZ_PlayerController::OnToggleSpawnModeKeyPressed);
	InputComponent->BindAction("ToggleSpawnModeKey", IE_Released, this, &ARZ_PlayerController::OnToggleSpawnModeKeyReleased);
}

void ARZ_PlayerController::LookUpAxis(float AxisValue)
{
	if (ControlSettings.ControlRotationMode == ERZ_ControlRotationMode::Mouse)
		AddPitchInput(AxisValue);
}

void ARZ_PlayerController::LookRightAxis(float AxisValue)
{
	if (ControlSettings.ControlRotationMode == ERZ_ControlRotationMode::Mouse)
		AddYawInput(AxisValue);
}

void ARZ_PlayerController::OnLookUpKeyPressed()
{
	if (ControlSettings.ControlRotationMode == ERZ_ControlRotationMode::Cursor && CameraManager.IsValid())
	{
		CameraManager->AddManualControlRotationPitch(1.0f);
	}
}

void ARZ_PlayerController::OnLookDownKeyPressed()
{
	if (ControlSettings.ControlRotationMode == ERZ_ControlRotationMode::Cursor && CameraManager.IsValid())
	{
		CameraManager->AddManualControlRotationPitch(-1.0f);
	}
}

void ARZ_PlayerController::OnLookRightKeyPressed()
{
	if (ControlSettings.ControlRotationMode == ERZ_ControlRotationMode::Cursor && CameraManager.IsValid())
	{
		CameraManager->AddManualControlRotationYaw(1.0f);
	}
}

void ARZ_PlayerController::OnLookLeftKeyPressed()
{
	if (ControlSettings.ControlRotationMode == ERZ_ControlRotationMode::Cursor && CameraManager.IsValid())
	{
		CameraManager->AddManualControlRotationYaw(-1.0f);
	}
}

void ARZ_PlayerController::OnZoomInKeyPressed()
{
	if (PlayerControllerMode == ERZ_PlayerControllerMode::Spawn)
	{
		if (PossessedCharacter.IsValid() && PossessedCharacter->GetInventoryComponent())
		{
			PossessedCharacter->GetInventoryComponent()->RotateBuildActor(true);
		}

		return;
	}
	
	if (CameraManager.IsValid())
	{
		CameraManager->ZoomIn();
	}
}

void ARZ_PlayerController::OnZoomOutKeyPressed()
{
	if (PlayerControllerMode == ERZ_PlayerControllerMode::Spawn)
	{
		if (PossessedCharacter.IsValid() && PossessedCharacter->GetInventoryComponent())
		{
			PossessedCharacter->GetInventoryComponent()->RotateBuildActor(false);
		}

		return;
	}
	
	if (CameraManager.IsValid())
	{
		CameraManager->ZoomOut();
	}
}

void ARZ_PlayerController::MoveForwardAxis(float AxisValue)
{
	if (PossessedCharacter.IsValid() && CameraManager.IsValid())
	{
		const FRotator YawRotation(0, CameraManager->GetFinalCameraRotation().Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);

		PossessedCharacter->AddMovementInput(Direction, AxisValue);
	}
}

void ARZ_PlayerController::MoveRightAxis(float AxisValue)
{
	if (PossessedCharacter.IsValid())
	{
		const FRotator YawRotation(0, CameraManager->GetFinalCameraRotation().Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);

		PossessedCharacter->AddMovementInput(Direction, AxisValue);
	}
}

void ARZ_PlayerController::OnRunKeyPressed()
{
	if (PossessedCharacter.IsValid() && PossessedCharacter->GetCharacterMovement())
	{
		URZ_CharacterMovementComponent* MovementComp = Cast<URZ_CharacterMovementComponent>(
			PossessedCharacter->GetCharacterMovement()
		);
		if (MovementComp)
		{
			MovementComp->SetSprinting(true);
		}
	}
}

void ARZ_PlayerController::OnRunKeyReleased()
{
	if (PossessedCharacter.IsValid() && PossessedCharacter->GetCharacterMovement())
	{
		URZ_CharacterMovementComponent* MovementComp = Cast<URZ_CharacterMovementComponent>(
			PossessedCharacter->GetCharacterMovement()
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

void ARZ_PlayerController::OnLeftMouseButtonPressed()
{
	if (!PossessedCharacter.IsValid()) { return; }
	if (!PossessedCharacter->GetInventoryComponent()) { return; }

	if (PlayerControllerMode == ERZ_PlayerControllerMode::Spawn)
	{
		PossessedCharacter->GetInventoryComponent()->DropEquippedItemAtTargetLocation();
	}
	else
	{
		PossessedCharacter->GetInventoryComponent()->SetWantToUseEquippedItem(true);
	}
}

void ARZ_PlayerController::OnLeftMouseButtonReleased()
{
	if (!PossessedCharacter.IsValid()) { return; }
	if (!PossessedCharacter->GetInventoryComponent()) { return; }

	PossessedCharacter->GetInventoryComponent()->SetWantToUseEquippedItem(false);
}

void ARZ_PlayerController::OnRightMouseButtonPressed()
{
	if (!PossessedCharacter.IsValid()) { return; }
	if (!PossessedCharacter->GetInventoryComponent()) { return; }
	if (!CursorToWorldHitResult.Actor.IsValid()) { return; }
	
	PossessedCharacter->GetInventoryComponent()->AddItemFromWorld(CursorToWorldHitResult.Actor.Get());
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
		if (UIManager->IsMenuOpen())
		{
			UIManager->ToggleMenu(false);
			SetShowMouseCursor(ControlSettings.bShowMouseCursor);
			DefaultMouseCursor = EMouseCursor::Default;
		}
		else
		{
			UIManager->ToggleMenu(true);
			SetShowMouseCursor(true);
			DefaultMouseCursor = EMouseCursor::Crosshairs;
		}
	}
}

void ARZ_PlayerController::OnQuickSlot1KeyPressed()
{
	if (PossessedCharacter.IsValid() && PossessedCharacter->GetInventoryComponent())
	{
		PossessedCharacter->GetInventoryComponent()->SelectSlot(0);
	}
}

void ARZ_PlayerController::OnQuickSlot2KeyPressed()
{
	if (PossessedCharacter.IsValid() && PossessedCharacter->GetInventoryComponent())
	{
		PossessedCharacter->GetInventoryComponent()->SelectSlot(1);
	}
}

void ARZ_PlayerController::OnQuickSlot3KeyPressed()
{
	if (PossessedCharacter.IsValid() && PossessedCharacter->GetInventoryComponent())
	{
		PossessedCharacter->GetInventoryComponent()->SelectSlot(2);
	}
}

void ARZ_PlayerController::OnQuickSlot4KeyPressed()
{
	if (PossessedCharacter.IsValid() && PossessedCharacter->GetInventoryComponent())
	{
		PossessedCharacter->GetInventoryComponent()->SelectSlot(3);
	}
}

void ARZ_PlayerController::OnQuickSlot5KeyPressed()
{
	if (PossessedCharacter.IsValid() && PossessedCharacter->GetInventoryComponent())
	{
		PossessedCharacter->GetInventoryComponent()->SelectSlot(4);
	}
}

void ARZ_PlayerController::OnQuickSlot6KeyPressed()
{
	if (PossessedCharacter.IsValid() && PossessedCharacter->GetInventoryComponent())
	{
		PossessedCharacter->GetInventoryComponent()->SelectSlot(5);
	}
}

void ARZ_PlayerController::OnQuickBarUpKeyPressed()
{
	if (PossessedCharacter.IsValid() && PossessedCharacter->GetInventoryComponent())
	{
		PossessedCharacter->GetInventoryComponent()->SelectNextQuickBar(true);
	}
}

void ARZ_PlayerController::OnQuickBarDownKeyPressed()
{
	if (PossessedCharacter.IsValid() && PossessedCharacter->GetInventoryComponent())
	{
		PossessedCharacter->GetInventoryComponent()->SelectNextQuickBar(false);
	}
}

void ARZ_PlayerController::OnToggleSpawnModeKeyPressed()
{
	//PlayerControllerMode = ERZ_PlayerControllerMode::Spawn;

	//OnPlayerControllerModeUpdated.Broadcast(PlayerControllerMode);
}

void ARZ_PlayerController::OnToggleSpawnModeKeyReleased()
{
	//PlayerControllerMode = ERZ_PlayerControllerMode::PawnControl;

	//OnPlayerControllerModeUpdated.Broadcast(PlayerControllerMode);
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
	if (!PossessedCharacter.IsValid()) { return; }
	if (!PossessedCharacter->GetInventoryComponent()) { return; }

	PossessedCharacter->GetInventoryComponent()->AddItemFromDataTable(ItemName);
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
	
	const FString PlayerControllerModeString = RZ_UtilityLibrary::GetEnumAsString("ERZ_PlayerControllerMode", PlayerControllerMode);
	RZ_UtilityLibrary::PrintStringToScreen("PlayerControllerMode == ", PlayerControllerModeString, FColor::Cyan, -1, DeltaTime);
	//const FString String = RZ_UtilityLibrary::GetEnumAsString("ERZ_ControlRotationMode", ControlRotationMode);
}