/// RemzDNB

#include "Player/RZ_PlayerController.h"
#include "Game/RZ_GameInstance.h"
#include "Game/RZ_GameMode.h"
#include "Game/RZ_GameState.h"
#include "Game/RZ_GameSettings.h"
#include "Game/RZ_WorldSettings.h"
#include "Character/RZ_Character.h"
// Character plugin
#include "RZ_CharacterMovementComponent.h"
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

ARZ_PlayerController::ARZ_PlayerController()
{
	PlayerCameraManagerClass = ARZ_CameraManager::StaticClass();
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	
	PlayerControllerMode = ERZ_PlayerControllerMode::None;
	
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

void ARZ_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/// Update TargetLocation
	
	if (IsLocalController())
	{
		UpdateTargetFromCursor();
		UpdateTargetFromScreenCenter();
		UpdateTargetSpawnLocation();
		
		//SetTargetLocation(NewTargetLocation);
		
		if (GetLocalRole() < ROLE_Authority)
			SetTargetLocation_Server(TargetLocation);

		if (PossessedCharacter.IsValid())
		{
			if (PossessedCharacter->GetInventoryComponent())
			{
				PossessedCharacter->GetInventoryComponent()->PlayerTargetLocation = CursorToGroundHit.Location;
			}
		}
	}
}

void ARZ_PlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	PossessedCharacter = Cast<ARZ_Character>(GetPawn());
	if (PossessedCharacter.IsValid())
	{
		PlayerControllerMode = ERZ_PlayerControllerMode::PawnControl;
	}

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
				PossessedCharacter->GetInventoryComponent()->OnItemEquipped.AddUniqueDynamic(
					this,
					&ARZ_PlayerController::OnCharacterEquippedItem
				);
				
				PossessedCharacter->GetInventoryComponent()->AddItem("Handgun_00");
				PossessedCharacter->GetInventoryComponent()->AddItem("Rifle_00");
				PossessedCharacter->GetInventoryComponent()->AddItem("Turret_00");
				//PossessedCharacter->GetInventoryComponent()->AddItem("Turret_00");
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

void ARZ_PlayerController::UpdateTargetFromCursor()
{
	if (IsLocalController() == false ||
		ControlSettings.ControlRotationMode != ERZ_ControlRotationMode::Cursor ||
		!bShowMouseCursor)
	{
		return;
	}
	
	FHitResult CursorHitResult; // class variable ?
	GetHitResultUnderCursorByChannel(
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2),
		false,
		CursorHitResult
	);
	//UKismetSystemLibrary::DrawDebugSphere(GetWorld(), CursorHitResult.ImpactPoint, 10.0f, 10, FColor::Red, .15f, 0.3f);
	
	if (PossessedCharacter.IsValid())
	{
		FHitResult TargetTraceHitResult;
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(GetPawn());
		const FVector TargetTraceStartLocation = FVector(
			/*PossessedCharacter->GetActorLocation().X,
			PossessedCharacter->GetActorLocation().Y,*/
		PossessedCharacter->GetMesh()->GetBoneLocation("hand_r").X,
		PossessedCharacter->GetMesh()->GetBoneLocation("hand_r").Y,
			BASEVIEWHEIGHT
		);
		const FVector TargetTraceEndLocation =
			TargetTraceStartLocation +
			UKismetMathLibrary::FindLookAtRotation(TargetTraceStartLocation, CursorHitResult.ImpactPoint).Vector() *
			10000.0f;
		GetWorld()->LineTraceSingleByChannel(
			TargetTraceHitResult,
			TargetTraceStartLocation,
			TargetTraceEndLocation,
			ECC_Visibility,
			TraceParams
		);
		if (TargetTraceHitResult.bBlockingHit)
		{
			UKismetSystemLibrary::DrawDebugSphere(GetWorld(), TargetTraceHitResult.ImpactPoint, 10.0f, 10, FColor::Green, .15f, 0.3f);
			//boué
			TargetLocation = TargetTraceHitResult.ImpactPoint;
			SetControlRotation(UKismetMathLibrary::FindLookAtRotation(PossessedCharacter->GetActorLocation(), TargetTraceHitResult.ImpactPoint));
			PossessedCharacter->TargetLocation = TargetTraceHitResult.ImpactPoint;
		}
		
		//PossessedCharacter->TargetLocation = CursorHitResult.Location;
	}
}

void ARZ_PlayerController::UpdateTargetFromScreenCenter()
{
	if (IsLocalController() == false ||
		ControlSettings.ControlRotationMode == ERZ_ControlRotationMode::Cursor ||
		bShowMouseCursor)
	{
		return;
	}
	
	const FVector Start = CameraManager->GetCameraLocation();
	const FVector End = Start + (CameraManager->GetCameraRotation().Vector() * 50000);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetPawn());
	FHitResult HitResult;

	/* Get intersection between cursor and plane.
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, TraceParams);
	if (HitResult.bBlockingHit)
	{
		PossessedCharacter->TargetLocation = HitResult.Location;
	}*/
}

void ARZ_PlayerController::UpdateTargetSpawnLocation()
{
	if (!IsLocalController()) { return; }
	if (!bShowMouseCursor) { return; }
	if (PlayerControllerMode != ERZ_PlayerControllerMode::Spawn) { return; }
	
	GetHitResultUnderCursorByChannel(
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel6), // CursorToGround trace
		false,
		CursorToGroundHit
	);

	//UKismetSystemLibrary::DrawDebugSphere(GetWorld(), CursorToGroundHit.ImpactPoint, 50.0f, 10, FColor::Orange, .15f, 0.5f);

	/*ARZ_WorldGrid* WorldGrid = Cast<ARZ_WorldGrid>(CursorToGroundHit.Actor);
	if (WorldGrid)
	{
		TArray<int32> InstanceIndexes = WorldGrid->GetInstancedGroundMeshesComponent()->GetInstancesOverlappingSphere(
			CursorToGroundHit.Location + FVector(0.0f, 0.0f, -10.0f),
			1.0f);
		
		for (const auto& InstanceIndex : InstanceIndexes)
		{
			WorldGrid->GetInstancedGroundMeshesComponent()->SetCustomDataValue(InstanceIndex, 0, 1.0f, true);
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("RZ_PlayerController::UpdateTargetSpawnLocation 1"));;
			//WorldGrid->GetInstancedGroundMeshesComponent()->RemoveInstance(InstanceIndex);
			FTransform OutTransform;
			WorldGrid->GetInstancedGroundMeshesComponent()->GetInstanceTransform(InstanceIndex, OutTransform, true);

			if (SpawnManagerComp)
			{
				//SpawnManagerComp->PlayerTargetLocation = CursorToGroundHitResult.ImpactPoint;
				SpawnManagerComp->PlayerTargetLocation = OutTransform.GetLocation();
			}
		}
	}*/
}

void ARZ_PlayerController::SetTargetLocation(const FVector& NewTargetLocation)
{
	TargetLocation = NewTargetLocation;

	if (PossessedCharacter.IsValid())
	{
		PossessedCharacter->TargetLocation = NewTargetLocation;
	}
}

void ARZ_PlayerController::SetTargetLocation_Server_Implementation(const FVector& NewTargetLocation)
{
	SetTargetLocation(NewTargetLocation);
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
	IRZ_ItemActorInterface* ItemInterface = Cast<IRZ_ItemActorInterface>(EquippedItem);
	if (!ItemInterface) { return; }
	
	const FRZ_InventoryItemSettings ItemSettings = ItemInterface->GetItemSettings();
	
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

	// Look
	
	InputComponent->BindAxis("LookUpAxis", this, &ARZ_PlayerController::LookUpAxis).bConsumeInput = false;
	InputComponent->BindAxis("LookRightAxis", this, &ARZ_PlayerController::LookRightAxis).bConsumeInput = false;
	InputComponent->BindAction("LookUpKey", IE_Pressed, this, &ARZ_PlayerController::OnLookUpKeyPressed);
	InputComponent->BindAction("LookDownKey", IE_Pressed, this, &ARZ_PlayerController::OnLookDownKeyPressed);
	InputComponent->BindAction("LookRightKey", IE_Pressed, this, &ARZ_PlayerController::OnLookRightKeyPressed);
	InputComponent->BindAction("LookLeftKey", IE_Pressed, this, &ARZ_PlayerController::OnLookLeftKeyPressed);
	InputComponent->BindAction("ZoomInKey", IE_Pressed, this, &ARZ_PlayerController::OnZoomInKeyPressed);
	InputComponent->BindAction("ZoomOutKey", IE_Pressed, this, &ARZ_PlayerController::OnZoomOutKeyPressed);

	// Movement
	
	InputComponent->BindAxis("MoveForwardAxis", this, &ARZ_PlayerController::MoveForwardAxis);
	InputComponent->BindAxis("MoveRightAxis", this, &ARZ_PlayerController::MoveRightAxis);
	InputComponent->BindAction("RunKey", IE_Pressed, this, &ARZ_PlayerController::OnRunKeyPressed);
	InputComponent->BindAction("RunKey", IE_Released, this, &ARZ_PlayerController::OnRunKeyReleased);
	InputComponent->BindAction("JumpKey", IE_Pressed, this, &ARZ_PlayerController::OnJumpKeyPressed);

	// Interaction

	InputComponent->BindAction("Use", IE_Pressed, this, &ARZ_PlayerController::OnUseKeyPressed);
	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &ARZ_PlayerController::OnLeftMouseButtonPressed);
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &ARZ_PlayerController::OnLeftMouseButtonReleased);
	InputComponent->BindAction("RightMouseButton", IE_Pressed, this, &ARZ_PlayerController::OnRightMouseButtonPressed);
	InputComponent->BindAction("RightMouseButton", IE_Released, this, &ARZ_PlayerController::OnRightMouseButtonReleased);
	InputComponent->BindAction("MiddleMouseButton", IE_Pressed, this, &ARZ_PlayerController::OnMiddleMouseButtonPressed);
	InputComponent->BindAction("MiddleMouseButton", IE_Released, this, &ARZ_PlayerController::OnMiddleMouseButtonReleased);
	InputComponent->BindAction("Quickslot1Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickslot1KeyPressed);
	InputComponent->BindAction("Quickslot2Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickslot2KeyPressed);
	InputComponent->BindAction("Quickslot3Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickslot3KeyPressed);
	InputComponent->BindAction("Quickslot4Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickslot4KeyPressed);
	InputComponent->BindAction("Quickslot5Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickslot5KeyPressed);
	InputComponent->BindAction("Quickslot6Key", IE_Pressed, this, &ARZ_PlayerController::OnQuickslot6KeyPressed);

	InputComponent->BindAction("ToggleMenuKey", IE_Pressed, this, &ARZ_PlayerController::OnToggleMenuKeyPressed);
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
	if (CameraManager.IsValid())
	{
		CameraManager->ZoomIn();
	}
}

void ARZ_PlayerController::OnZoomOutKeyPressed()
{
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
	
	if (PlayerControllerMode == ERZ_PlayerControllerMode::Spawn)
	{
		if (PossessedCharacter->GetInventoryComponent())
		{
			PossessedCharacter->GetInventoryComponent()->DropEquippedItemAtTargetLocation();
		}
	}
	else
	{
		/*if (PossessedCharacter.IsValid() &&
	PossessedCharacter->GetItemManagerComponent() &&
	PossessedCharacter->GetItemManagerComponent()->GetEquippedItem())
{
	PossessedCharacter->GetItemManagerComponent()->GetEquippedItem()->SetWantsToUse(true);
}*/
	}
}

void ARZ_PlayerController::OnLeftMouseButtonReleased()
{
	/*if (PossessedCharacter.IsValid() &&
		PossessedCharacter->GetItemManagerComponent() &&
		PossessedCharacter->GetItemManagerComponent()->GetEquippedItem())
	{
		PossessedCharacter->GetItemManagerComponent()->GetEquippedItem()->SetWantsToUse(false);
	}*/
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

void ARZ_PlayerController::OnQuickslot1KeyPressed()
{
	if (PossessedCharacter.IsValid() && PossessedCharacter->GetInventoryComponent())
	{
		PossessedCharacter->GetInventoryComponent()->EquipItemSlot(0);
	}
}

void ARZ_PlayerController::OnQuickslot2KeyPressed()
{
	if (PossessedCharacter.IsValid() && PossessedCharacter->GetInventoryComponent())
	{
		PossessedCharacter->GetInventoryComponent()->EquipItemSlot(1);
	}
}

void ARZ_PlayerController::OnQuickslot3KeyPressed()
{
	if (PossessedCharacter.IsValid() && PossessedCharacter->GetInventoryComponent())
	{
		PossessedCharacter->GetInventoryComponent()->EquipItemSlot(2);
	}
}

void ARZ_PlayerController::OnQuickslot4KeyPressed()
{
	if (PossessedCharacter.IsValid() && PossessedCharacter->GetInventoryComponent())
	{
		PossessedCharacter->GetInventoryComponent()->EquipItemSlot(3);
	}
}

void ARZ_PlayerController::OnQuickslot5KeyPressed()
{
	if (PossessedCharacter.IsValid() && PossessedCharacter->GetInventoryComponent())
	{
		PossessedCharacter->GetInventoryComponent()->EquipItemSlot(4);
	}
}

void ARZ_PlayerController::OnQuickslot6KeyPressed()
{
	if (PossessedCharacter.IsValid() && PossessedCharacter->GetInventoryComponent())
	{
		PossessedCharacter->GetInventoryComponent()->EquipItemSlot(5);
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
	if (!PossessedCharacter.IsValid()) { return; }
	if (!PossessedCharacter->GetInventoryComponent()) { return; }

	PossessedCharacter->GetInventoryComponent()->AddItem(ItemName);
}
