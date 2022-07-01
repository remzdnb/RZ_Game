/// RemzDNB

#include "Pawn/RZ_Pawn.h"
#include "RZ_AttributeComponent.h"
#include "Core/RZ_GameInstance.h"
#include "Core/RZ_GameState.h"
#include "UI/RZ_PawnOTMWidget.h"
// InventorySystem
#include "RZ_InventoryComponent.h"
// BuildingSystem
#include "RZ_BuildingManager.h"
#include "RZ_BuildingComponent.h"
// PowerSystem
#include "RZ_PowerManager.h"
#include "RZ_PowerComponent.h"
//
#include "EngineUtils.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Pawn/RZ_Character.h"

ARZ_Pawn::ARZ_Pawn()
{
	RootSceneComp = CreateDefaultSubobject<USceneComponent>(FName("RootSceneComp"));
	RootComponent = RootSceneComp;

	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("BaseMeshComp"));
	BaseMeshComp->SetupAttachment(RootComponent);
	BaseMeshComp->SetAbsolute(false);
	BaseMeshComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	BaseMeshComp->SetCollisionProfileName("Pawn");
	BaseMeshComp->SetGenerateOverlapEvents(true);
	BaseMeshComp->SetCustomDepthStencilValue(1);
	
	BuildMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("BuildMeshComp"));
	BuildMeshComp->SetupAttachment(RootComponent);
	BuildMeshComp->SetCollisionProfileName("IgnoreAll");
	BuildMeshComp->SetUsingAbsoluteRotation(true);
	//BuildSquareMeshComp->SetAbsolute(false, true, false);

	OTMWidgetComp = CreateDefaultSubobject<UWidgetComponent>(FName("OTMWidgetComp"));;
	OTMWidgetComp->SetupAttachment(RootComponent);
	OTMWidgetComp->SetCollisionProfileName("IgnoreAll");
	OTMWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	OTMWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	OTMWidgetComp->SetDrawSize(FVector2D(150.0f, 40.f));

	CombatComp = CreateDefaultSubobject<URZ_CombatComponent>("CombatComp");
	InventoryComp = CreateDefaultSubobject<URZ_InventoryComponent>("InventoryComp"); // for turrets ?
	BuildingComp = CreateDefaultSubobject<URZ_BuildingComponent>(FName("BuildingComp"));
	PowerComp = CreateDefaultSubobject<URZ_PowerComponent>(FName("PowerComp"));
	
	bUseControllerRotationPitch = true; //?
	bUseControllerRotationYaw = true; //?
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ARZ_Pawn::OnConstruction(const FTransform& InTransform)
{
	Super::OnConstruction(InTransform);
	
	// Align the collision box to the floor level. incroyable
	//const FVector BoxExtent = CollisionBoxCT->GetScaledBoxExtent();
	//CollisionBoxCT->SetRelativeLocation(FVector(0.0f, 0.0f, BoxExtent.Z));
}

void ARZ_Pawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!GetWorld()->IsGameWorld()) { return; }

	GameInstance = Cast<URZ_GameInstance>(GetGameInstance());
	GameSettings = Cast<URZ_GameInstance>(GetGameInstance())->GetGameSettings();
	GameState = Cast<ARZ_GameState>(GetWorld()->GetGameState());

	InitActorSettings(GetWorld(), DataTableRowName);
	//PawnCombatComp->Init(1000.0f, 1000.0f);
	BaseMeshDefaultMaterial = BaseMeshComp->GetMaterial(0);
}

void ARZ_Pawn::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ARZ_BuildingManager> NewBuildingManager(GetWorld()); NewBuildingManager; ++NewBuildingManager)
	{
		BuildingManager = *NewBuildingManager;
		break;
	}

	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	
	GameState->ReportPawnBeginPlay(this);
	
	//PawnCombatComp->OnHealthReachedZero.AddUniqueDynamic(this, &ARZ_Pawn::OnDestroyed);

	OTMWidget = Cast<URZ_PawnOTMWidget>(OTMWidgetComp->GetWidget());
	if (OTMWidget)
		OTMWidget->Init(this);
}

void ARZ_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BuildingComp)
	{
		BuildingComp->PlayerTargetLocation = PlayerTargetLocation;
	}
}

void ARZ_Pawn::OnAttachedToInventory(URZ_InventoryComponent* InventoryCompRef)
{
	IRZ_InventoryActorInterface::OnAttachedToInventory(InventoryCompRef);
	
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	SetActorMode(ERZ_ActorMode::Hidden_Disabled); // nah, do it from inventorycomp

	if (PowerComp->GetPowerManager() &&
		PowerComp->GetPowerGridID() != 0)
	{
		PowerComp->GetPowerManager()->RemoveComponentFromGrid(PowerComp->GetPowerGridID(), PowerComp);
	}

	PowerComp->SetIsDisabled(true);
	for (const auto& Power : PowerComp->GetConnectedPowerComps())
	{
		Power->UpdateConnectedActors(); // OnPowerComponentMoved() in Manager.
	}
	PowerComp->GetPowerManager()->EvaluateGrids();
}

void ARZ_Pawn::OnInventorySelection(bool bNewIsSelected)
{
	if (bNewIsSelected)
	{
		SetActorHiddenInGame(true);
		SetActorTickEnabled(true);
		BuildingComp->StartBuilding(this);
	}
	else
	{
		SetActorHiddenInGame(false);
		SetActorTickEnabled(false);
		BuildingComp->StopBuilding();
	}
}

void ARZ_Pawn::OnHoverStart()
{
	BaseMeshComp->SetRenderCustomDepth(true);

	if (OTMWidget)
	{
		OTMWidget->ToggleBuildingOTM_BPI();
	}
}

void ARZ_Pawn::OnHoverEnd()
{
	BaseMeshComp->SetRenderCustomDepth(false);

	if (OTMWidget)
	{
		OTMWidget->ToggleBuildingOTM_BPI();
	}
}

void ARZ_Pawn::SetActorMode(ERZ_ActorMode NewActorMode)
{
	IRZ_ActorInterface::SetActorMode(NewActorMode);

	/*if (InventoryComp)
	{
		for (const auto& AttachedSlot : InventoryComp->GetAttachedSlots())
		{
			IRZ_ActorInterface* ActorInterface = Cast<IRZ_ActorInterface>(AttachedSlot.AttachedActor);
			if (ActorInterface)
			{
				ActorInterface->SetActorMode(NewActorMode);
			}
		}
	}*/
}

void ARZ_Pawn::OnBuildStart()
{
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(false);

	//BaseMeshComp->SetMaterial(0, BuildingManager->BuildingMaterial_Valid);
	BuildMeshComp->SetVisibility(true);
	OTMWidgetComp->SetVisibility(false);

	SetActorMode(ERZ_ActorMode::Visible_Disabled);
	
	//RZ_UtilityLibrary::PrintStringToScreen("ARZ_Pawn::OnBuildStart", "", FColor::Green, -1, 3.0f);
}

void ARZ_Pawn::OnBuildStop()
{
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	//BaseMeshComp->SetMaterial(0, BaseMeshDefaultMaterial);
	BuildMeshComp->SetVisibility(false);

	//RZ_UtilityLibrary::PrintStringToScreen("ARZ_Pawn::OnBuildStop", "", FColor::Green, -1, 3.0f);
}

void ARZ_Pawn::OnBuildEnd()
{
	if (!BuildingComp->GetIsBuilding()) { return; }
		
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	BaseMeshComp->SetMaterial(0, BaseMeshDefaultMaterial);
	BuildMeshComp->SetVisibility(false);
	OTMWidgetComp->SetVisibility(true);

	SetActorMode(ERZ_ActorMode::Visible_Enabled);

	UE_LOG(LogTemp, Display, TEXT("ARZ_Pawn::OnBuildEnd 0"));

	if (GetOwnerInventory())
	{
		UE_LOG(LogTemp, Display, TEXT("ARZ_Pawn::OnBuildEnd 1"));
		GetOwnerInventory()->DropSelectedSlot();
	}
	
	/*ARZ_Character* CharacterOwner = Cast<ARZ_Character>(GetOwner());
	if (CharacterOwner)
	{
		UE_LOG(LogTemp, Display, TEXT("ARZ_Pawn::OnBuildEnd 1"));
		URZ_InventoryComponent* InvComp = CharacterOwner->GetInventoryComponent();
		if (InvComp)
		{
			UE_LOG(LogTemp, Display, TEXT("ARZ_Pawn::OnBuildEnd 2"));
			InvComp->DropSelectedSlot();
		}
	}*/
	
	//const FDetachmentTransformRules TransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);
	//DetachFromActor(TransformRules);
	
	PowerComp->SetIsDisabled(false);
	PowerComp->UpdateConnectedActors();
	for (const auto& Power : PowerComp->GetConnectedPowerComps())
	{
		Power->UpdateConnectedActors(); // OnPowerComponentMoved() in Manager.
	}
	PowerComp->GetPowerManager()->EvaluateGrids();
	
	//RZ_UtilityLibrary::PrintStringToScreen("ARZ_Pawn::OnBuildEnd", "", FColor::Green, -1, 3.0f);
}

void ARZ_Pawn::OnValidBuildLocation()
{
	if (BuildingManager.IsValid())
	{
		BaseMeshComp->SetMaterial(0, BuildingManager->BuildingMaterial_Valid);
		BuildMeshComp->SetVisibility(true);
	}
}

void ARZ_Pawn::OnInvalidBuildLocation()
{
	if (BuildingManager.IsValid())
	{
		BaseMeshComp->SetMaterial(0, BuildingManager->BuildingMaterial_Invalid);
		BuildMeshComp->SetVisibility(false);
	}
}

void ARZ_Pawn::SetWantToUse(bool bNewWantsTouse)
{
	if (bNewWantsTouse == true)
	{
		if (BuildingComp)
		{
			BuildingComp->EndBuilding();
		}
	}
}

void ARZ_Pawn::SetWantToFire(bool bNewWantToFire)
{
	
}

void ARZ_Pawn::SetActiveTarget(AActor* NewActiveTarget)
{
}

void ARZ_Pawn::OnDestroyed()
{
	/*if (PowerComp)
	{
		PowerComp->PowerManager->RemovePowerComponent(PowerComp);
	}
	
	Destroy();
	GameState->ReportPawnEndPlay(this);*/
}


void ARZ_Pawn::OnPowerStatusUpdated(bool bNewIsPowered)
{
	const ERZ_ActorMode NewActorMode = bNewIsPowered ? ERZ_ActorMode::Visible_Enabled : ERZ_ActorMode::Visible_Disabled;
	
	for (const auto& AttachedSlot : InventoryComp->GetAttachedSlots())
	{
		Cast<IRZ_ActorInterface>(AttachedSlot.AttachedActor)->SetActorMode(NewActorMode);
	}
}
