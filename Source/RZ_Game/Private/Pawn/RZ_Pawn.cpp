/// RemzDNB

#include "Pawn/RZ_Pawn.h"
#include "Core/RZ_GameInstance.h"
#include "Core/RZ_GameState.h"
#include "Pawn/RZ_PawnCombatComponent.h"
//
#include "RZM_InventorySystem.h"
// PowerSystem
#include "RZ_PowerManager.h"
#include "RZ_PowerComponent.h"
//
#include "Building/RZ_BuildingOTM_BaseWidget.h"
//
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/RZ_GameSettings.h"

ARZ_Pawn::ARZ_Pawn()
{
	RootSceneComp = CreateDefaultSubobject<USceneComponent>(FName("RootSceneComp"));
	RootComponent = RootSceneComp;

	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("BaseMeshComp"));
	BaseMeshComp->SetupAttachment(RootComponent);
	BaseMeshComp->SetAbsolute(false);
	BaseMeshComp->SetCollisionProfileName("IgnoreAll");
	BaseMeshComp->SetGenerateOverlapEvents(false);
	BaseMeshComp->SetCustomDepthStencilValue(1);

	CollisionBoxCT = CreateDefaultSubobject<UBoxComponent>(FName("CollisionBoxCT"));
	CollisionBoxCT->SetupAttachment(RootComponent);
	//CollisionBoxCT->SetAbsolute(false, true, false);
	CollisionBoxCT->SetCollisionProfileName("OverlapAll");
	CollisionBoxCT->IgnoreActorWhenMoving(this, true);

	OTMWidgetComp = CreateDefaultSubobject<UWidgetComponent>(FName("OTMWidgetComp"));;
	OTMWidgetComp->SetupAttachment(RootComponent);
	OTMWidgetComp->SetCollisionProfileName("IgnoreAll");
	OTMWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	OTMWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	OTMWidgetComp->SetDrawSize(FVector2D(200.0f, 100.f));

	PawnCombatComp = CreateDefaultSubobject<URZ_PawnCombatComponent>(FName("PawnCombatComp"));
	
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
	
	PawnCombatComp->Init(1000.0f, 1000.0f);

	InitItemSettings(GetWorld(), DataTableRowName);
}

void ARZ_Pawn::BeginPlay()
{
	Super::BeginPlay();

	BuildingOTMWidget = Cast<URZ_BuildingOTM_BaseWidget>(OTMWidgetComp->GetWidget());
	if (BuildingOTMWidget)
		BuildingOTMWidget->Init(this);

	BaseMeshDefaultMaterial = BaseMeshComp->GetMaterial(0);

	GameState->ReportPawnBeginPlay(this);
	
	PawnCombatComp->OnHealthReachedZero.AddUniqueDynamic(this, &ARZ_Pawn::OnDestroyed);
}

void ARZ_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*if (bIsSelected)
	{
		SetActorLocation(PlayerTargetLocation);
		SetActorLocation(PlayerTargetLocation);
		//CollisionBoxCT->SetWorldLocation(SpawnLocation);
		GridMaterialMeshCT->SetWorldLocation(FVector(
			SpawnLocation.X,
			SpawnLocation.Y,
			1.0f
		));
		//BaseMeshCT->SetWorldLocation(LerpedItemLocation);
		BuildSquareMeshComp->SetWorldLocation(PlayerTargetLocation);
	}*/
}

void ARZ_Pawn::Init(ERZ_PawnOwnership NewPawnOwnerShip, uint8 NewTeamID)
{
	SetPawnOwnerShip(NewPawnOwnerShip);
	SetTeamID(NewTeamID);
}

void ARZ_Pawn::SetActiveTarget(AActor* NewActiveTarget)
{
}

void ARZ_Pawn::SetWantToFire(bool bNewWantToFire)
{
}

void ARZ_Pawn::SetPlayerTargetLocation(const FVector& NewPlayerTargetLocation)
{
	PlayerTargetLocation = NewPlayerTargetLocation;
}

void ARZ_Pawn::SetItemMode(ERZ_ItemMode NewItemMode)
{
	IRZ_ItemInterface::SetItemMode(NewItemMode);

	ItemMode = NewItemMode;

	if (ItemMode == ERZ_ItemMode::Hidden)
	{
		SetActorTickEnabled(false);
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
	else if (ItemMode == ERZ_ItemMode::Construction)
	{
		SetActorTickEnabled(true);
		SetActorHiddenInGame(false);
		SetActorEnableCollision(false);
		//BuildSquareMeshComp->SetVisibility(true);
		//PowerSquareMeshComp->SetVisibility(true);
	}
	else if (ItemMode == ERZ_ItemMode::Visible)
	{
		SetActorTickEnabled(true);
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);
		//BuildSquareMeshComp->SetVisibility(false);
		//PowerSquareMeshComp->SetVisibility(false);
	}
}

void ARZ_Pawn::OnInventorySelection(bool bNewIsSelected)
{
	if (bIsSelected == bNewIsSelected) { return; }
	
	if (bNewIsSelected)
	{
		//BaseMeshComp->SetMaterial(0, GameSettings->ItemSpawnMaterial_Valid);
	}
	else
	{
		//BaseMeshComp->SetMaterial(0, BaseMeshDefaultMaterial);
		//BaseMeshComp->SetWorldLocation(GetActorLocation());
	}

	SetActorTickEnabled(bNewIsSelected);
	SetActorHiddenInGame(!bNewIsSelected);
	SetActorEnableCollision(bNewIsSelected);
	SetActorTickEnabled(bNewIsSelected);
	bIsSelected = bNewIsSelected;
}


void ARZ_Pawn::OnHoverStart()
{
	BaseMeshComp->SetRenderCustomDepth(true);

	if (BuildingOTMWidget)
	{
		BuildingOTMWidget->ToggleBuildingOTM_BPI();
	}
}

void ARZ_Pawn::OnHoverEnd()
{
	BaseMeshComp->SetRenderCustomDepth(false);

	if (BuildingOTMWidget)
	{
		BuildingOTMWidget->ToggleBuildingOTM_BPI();
	}
}

void ARZ_Pawn::SetWantToUse(bool bNewWantsTouse)
{
	//if (!bNewWantsTouse || !(ItemMode == ERZ_ItemMode::Construction) || !IsValidBuildLocation()) { return; }
	
	//SetItemMode(ERZ_ItemMode::Visible);
}

/*
void ARZ_Pawn::OnProjectileCollision(float ProjectileDamage, const FVector& HitLocation,
                                         AController* InstigatorController)
{
	if (GetLocalRole() < ROLE_Authority || PawnCombatComp == nullptr)
		return;

	PawnCombatComp->ApplyDamage(ProjectileDamage, HitLocation, InstigatorController, nullptr);
}*/

void ARZ_Pawn::OnDestroyed()
{
	/*if (PowerComp)
	{
		PowerComp->PowerManager->RemovePowerComponent(PowerComp);
	}
	
	Destroy();
	GameState->ReportPawnEndPlay(this);*/
}
