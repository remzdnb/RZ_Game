/// RemzDNB

#include "Building/RZ_Building.h"
#include "Game/RZ_GameInstance.h"
#include "Game/RZ_GameState.h"
#include "Pawn/RZ_PawnCombatComponent.h"
//
#include "RZM_InventorySystem.h"
//
#include "Components/BoxComponent.h"

ARZ_Building::ARZ_Building()
{
	RootSceneCT = CreateDefaultSubobject<USceneComponent>(FName("RootSceneCT"));
	RootComponent = RootSceneCT;

	DemoMeshCT = CreateDefaultSubobject<UStaticMeshComponent>(FName("DemoMeshCT"));
	DemoMeshCT->SetupAttachment(RootComponent);
	DemoMeshCT->SetCollisionProfileName("IgnoreAll");
	DemoMeshCT->SetAbsolute(true, true, true);
	
	BaseMeshCT = CreateDefaultSubobject<UStaticMeshComponent>(FName("BaseMeshCT"));
	BaseMeshCT->SetupAttachment(RootComponent);
	BaseMeshCT->SetCollisionProfileName("IgnoreAll");
	BaseMeshCT->SetAbsolute(true, true, true);

	CollisionBoxCT = CreateDefaultSubobject<UBoxComponent>(FName("CollisionBoxCT"));
	CollisionBoxCT->SetupAttachment(RootComponent);
	CollisionBoxCT->SetCollisionProfileName("IgnoreAll");
	CollisionBoxCT->SetAbsolute(true, true, true);
	
	PawnCombatComp = CreateDefaultSubobject<URZ_PawnCombatComponent>(FName("PawnCombatComp"));

	bUseControllerRotationPitch = true; //?
	bUseControllerRotationYaw = true; //?
	
	PrimaryActorTick.bCanEverTick = false;
}

void ARZ_Building::OnConstruction(const FTransform& InTransform)
{
	Super::OnConstruction(InTransform);

	// Align the collision box to the floor level.
	
	const FVector BoxExtent = CollisionBoxCT->GetScaledBoxExtent();
	CollisionBoxCT->SetRelativeLocation(FVector(0.0f, 0.0f, BoxExtent.Z));
}


void ARZ_Building::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!GetWorld()->IsGameWorld()) { return; }

	GameState = Cast<ARZ_GameState>(GetWorld()->GetGameState());
	
	PawnCombatComp->Init(1000.0f, 1000.0f);

	InventorySystemSettings = Cast<IRZ_InventorySystemModuleInterface>(GetWorld()->GetGameInstance())
		->GetInventorySystemModuleSettings();
	
	
	///InventorySystemSettings = Cast<URZ_GameInstance>(GetGameInstance())->GetInventorySystemModuleSettings();
	if (InventorySystemSettings)
	{
		ItemSettings = *InventorySystemSettings->GetInventoryItemSettingsFromDataTable(DataTableRowName);
	}
}

void ARZ_Building::BeginPlay()
{
	Super::BeginPlay();

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

void ARZ_Building::OnEquipped()
{
}

void ARZ_Building::OnHolstered()
{
}

void ARZ_Building::SetWantsToUse(bool bNewWantsTouse)
{
}

const FRZ_InventoryItemSettings& ARZ_Building::GetItemSettings()
{
	return ItemSettings;
}

void ARZ_Building::ToggleDemoMode(bool bNewIsDemoMode)
{
	bIsDemoMode = bNewIsDemoMode;

	if (bIsDemoMode)
	{
		//DemoMeshComp->SetHiddenInGame(false);
	}
	else
	{
		//DemoMeshComp->SetHiddenInGame(true);
	}
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
