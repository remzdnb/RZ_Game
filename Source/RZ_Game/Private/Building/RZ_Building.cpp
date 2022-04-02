/// RemzDNB

#include "Building/RZ_Building.h"
#include "Game/RZ_GameInstance.h"
#include "Game/RZ_GameState.h"
#include "Pawn/RZ_PawnCombatComponent.h"
//
#include "RZM_InventorySystem.h"
//
#include "Components/BoxComponent.h"
#include "Game/RZ_GameSettings.h"

ARZ_Building::ARZ_Building()
{
	RootSceneCT = CreateDefaultSubobject<USceneComponent>(FName("RootSceneCT"));
	RootComponent = RootSceneCT;

	GridMaterialMeshCT = CreateDefaultSubobject<UStaticMeshComponent>(FName("DemoMeshCT"));
	GridMaterialMeshCT->SetupAttachment(RootComponent);
	GridMaterialMeshCT->SetCollisionProfileName("IgnoreAll");
	GridMaterialMeshCT->SetAbsolute(false, true, false);
	
	BaseMeshCT = CreateDefaultSubobject<UStaticMeshComponent>(FName("BaseMeshCT"));
	BaseMeshCT->SetupAttachment(RootComponent);
	BaseMeshCT->SetAbsolute(false, true, false);
	BaseMeshCT->SetCollisionProfileName("IgnoreAll");
	BaseMeshCT->SetGenerateOverlapEvents(false);
	BaseMeshCT->SetCustomDepthStencilValue(1);

	CollisionBoxCT = CreateDefaultSubobject<UBoxComponent>(FName("CollisionBoxCT"));
	CollisionBoxCT->SetupAttachment(RootComponent);
	CollisionBoxCT->SetAbsolute(false, true, false);
	CollisionBoxCT->SetCollisionProfileName("ProjectilePreset");
	CollisionBoxCT->IgnoreActorWhenMoving(this, true);
	
	PawnCombatComp = CreateDefaultSubobject<URZ_PawnCombatComponent>(FName("PawnCombatComp"));

	bUseControllerRotationPitch = true; //?
	bUseControllerRotationYaw = true; //?
	
	PrimaryActorTick.bCanEverTick = false;
}

void ARZ_Building::OnConstruction(const FTransform& InTransform)
{
	Super::OnConstruction(InTransform);

	// Align the collision box to the floor level. incroyable
	
	//const FVector BoxExtent = CollisionBoxCT->GetScaledBoxExtent();
	//CollisionBoxCT->SetRelativeLocation(FVector(0.0f, 0.0f, BoxExtent.Z));
}


void ARZ_Building::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!GetWorld()->IsGameWorld()) { return; }

	GameState = Cast<ARZ_GameState>(GetWorld()->GetGameState());
	GameSettings = Cast<URZ_GameInstance>(GetGameInstance())->GetGameSettings();
	
	PawnCombatComp->Init(1000.0f, 1000.0f);

	InitItemSettings(GetWorld(), DataTableRowName);
}

void ARZ_Building::BeginPlay()
{
	Super::BeginPlay();

	BaseMeshDefaultMaterial = BaseMeshCT->GetMaterial(0);

	GameState->ReportPawnBeginPlay(this);
	
	PawnCombatComp->OnHealthReachedZero.AddUniqueDynamic(this, &ARZ_Building::OnDestroyed);
}

void ARZ_Building::Init(ERZ_PawnOwnership NewPawnOwnerShip, uint8 NewTeamID)
{
	SetPawnOwnerShip(NewPawnOwnerShip);
	SetTeamID(NewTeamID);
}

UBehaviorTree* ARZ_Building::GetBehaviorTree()
{
	return PawnBehaviorTree;
}

void ARZ_Building::SetActiveTarget(AActor* NewActiveTarget)
{
}

void ARZ_Building::SetWantToFire(bool bNewWantToFire)
{
}

void ARZ_Building::SetControllerTargetLocation(const FVector& NewPlayerTargetLocation)
{
}

const FName& ARZ_Building::GetTableRowName()
{
	return DataTableRowName;
}

void ARZ_Building::OnHoverStart()
{
	BaseMeshCT->SetRenderCustomDepth(true);
}

void ARZ_Building::OnHoverEnd()
{
	BaseMeshCT->SetRenderCustomDepth(false);
}

void ARZ_Building::OnSelectionUpdated(bool bNewIsSelected)
{
}

void ARZ_Building::EnableBuildMode(bool bNewIsEnabled)
{
	SetIsBuildMode(bNewIsEnabled);
		
	if (bNewIsEnabled)
	{
		BaseMeshCT->SetMaterial(0, GameSettings->ItemSpawnMaterial_Valid);
		GridMaterialMeshCT->SetVisibility(true);
	}
	else
	{
		BaseMeshCT->SetMaterial(0, BaseMeshDefaultMaterial);
		BaseMeshCT->SetWorldLocation(GetActorLocation());
		GridMaterialMeshCT->SetVisibility(false);
	}
}

void ARZ_Building::UpdateBuildModeLocation(const FVector& SpawnLocation, const FVector& LerpedItemLocation)
{
	// Runs from InventoryComponent tick.

	SetActorLocation(LerpedItemLocation);
	//CollisionBoxCT->SetWorldLocation(SpawnLocation);
	/*GridMaterialMeshCT->SetWorldLocation(FVector(
		SpawnLocation.X,
		SpawnLocation.Y,
		1.0f
	));*/
	//BaseMeshCT->SetWorldLocation(LerpedItemLocation);
	GridMaterialMeshCT->SetWorldLocation(SpawnLocation);

	if (IsValidBuildLocation())
	{
		if (BaseMeshCT->GetMaterial(0) != GameSettings->ItemSpawnMaterial_Valid)
			BaseMeshCT->SetMaterial(0, GameSettings->ItemSpawnMaterial_Valid);
	}
	else
	{
		if (BaseMeshCT->GetMaterial(0) != GameSettings->ItemSpawnMaterial_Invalid)
			BaseMeshCT->SetMaterial(0, GameSettings->ItemSpawnMaterial_Invalid);
	}
}

void ARZ_Building::SetBuildMeshVisibility(bool bNewIsVisible)
{
	GridMaterialMeshCT->SetVisibility(bNewIsVisible);
}

bool ARZ_Building::IsValidBuildLocation()
{
	TArray<AActor*> ActorArray;
	CollisionBoxCT->GetOverlappingActors(ActorArray);

	if (ActorArray.Num() == 0) { return true; }

	return false;
}

void ARZ_Building::SetWantToUse(bool bNewWantsTouse)
{
}

/*
void ARZ_Building::OnProjectileCollision(float ProjectileDamage, const FVector& HitLocation,
                                         AController* InstigatorController)
{
	if (GetLocalRole() < ROLE_Authority || PawnCombatComp == nullptr)
		return;

	PawnCombatComp->ApplyDamage(ProjectileDamage, HitLocation, InstigatorController, nullptr);
}*/

void ARZ_Building::OnDestroyed()
{
	Destroy();
	GameState->ReportPawnEndPlay(this);
}
