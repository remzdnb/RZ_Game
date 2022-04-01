/// RemzDNB

// RZ_Game
#include "Building/RZ_TurretBuilding.h"
#include "Game/RZ_GameInstance.h"
#include "Game/RZ_GameSettings.h"
#include "AI/RZ_PawnAIController.h"
// WeaponSystem plugin
#include "RZ_Projectile.h"
// Engine
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Turret component shouldnt exist, it should be a weapon instead, implemented here with a InventorySystem.
// Turrets attached to other pawns like vehicles will be costly but w/e.
// Or Turrets could not inherit from building at all, and hold invisible weapons/implements projectile class.

ARZ_TurretBuilding::ARZ_TurretBuilding()
{
	TurretMeshCT = CreateDefaultSubobject<USkeletalMeshComponent>(FName("TurretMeshCT"));
	TurretMeshCT->SetupAttachment(RootComponent);
	TurretMeshCT->SetCollisionProfileName("IgnoreAll");
	TurretMeshCT->SetAbsolute(false, false, false);
	//TurretMeshCT->SetCustomDepthStencilValue(1);

	PerceptionCT = CreateDefaultSubobject<URZ_PerceptionComponent>("PerceptionCT");

	AIControllerClass = ARZ_PawnAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	bWantToFire = false;
	FireRate = 0.07f;
}

void ARZ_TurretBuilding::BeginPlay()
{
	Super::BeginPlay();

	GameSettings = Cast<URZ_GameInstance>(GetGameInstance())->GetGameSettings();
	AIController = Cast<ARZ_PawnAIController>(AIController);
	
	ProjectileWeaponSettings = *Cast<URZ_GameInstance>(GetGameInstance())->GetWeaponSystemModuleSettings()->GetProjectileWeaponInfoFromRow(DataTableRowName);
}

void ARZ_TurretBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	if (bWantToFire && CurrentTime - LastFireTime >= FireRate)
	{
		LastFireTime = CurrentTime;
		FireOnce();
	}
}

void ARZ_TurretBuilding::FireOnce()
{
	// Projectile

	const FVector SpawnLocation = FVector(
		TurretMeshCT->GetSocketLocation("Muzzle_00").X,
		TurretMeshCT->GetSocketLocation("Muzzle_00").Y,
		BASEVIEWHEIGHT
	);
	const FRotator SpawnRotation = GetActorRotation();
	const FTransform SpawnTransform(SpawnRotation, SpawnLocation, FVector(1.0f));
	const FActorSpawnParameters SpawnParameters;

	ARZ_Projectile* const Projectile = GetWorld()->SpawnActorDeferred<ARZ_Projectile>(
		ProjectileWeaponSettings.ProjectileBP,
		SpawnTransform,
		this,
		Cast<APawn>(GetOwner())
	);
	if (Projectile)
	{
		Projectile->Init(ProjectileWeaponSettings);
		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
	}
	
	// FX
	if (ProjectileWeaponSettings.MuzzleParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			ProjectileWeaponSettings.MuzzleParticle,
			TurretMeshCT->GetSocketLocation("Muzzle_00"),
			TurretMeshCT->GetSocketRotation("Muzzle_00"),
			FVector(1.5f)
		);
	}
}

void ARZ_TurretBuilding::SetActiveTarget(AActor* NewActiveTarget)
{
	if (!TurretMeshCT) { return; }

	if (NewActiveTarget)
	{
		Cast<AController>(GetOwner())->SetControlRotation(
			UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), NewActiveTarget->GetActorLocation())
		);
	}
}

void ARZ_TurretBuilding::SetWantToFire(bool bNewWantToFire)
{
	Super::SetWantToFire(bNewWantToFire);

	bWantToFire = bNewWantToFire;
}

void ARZ_TurretBuilding::ToggleDemoMode(bool bNewIsEnabled)
{
	Super::ToggleDemoMode(bNewIsEnabled);
	
	if (AIController)
	{
		AIController->ToggleAI(!bNewIsEnabled);
		// both here and in aicontroller.
	}
	
	//if (TurretComponent)
	//{
	//TurretComponent->SetMaterial(0, GameSettings->DemoPawnMaterial);
	//}
}

