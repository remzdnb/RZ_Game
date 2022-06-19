/// RemzDNB

// RZ_Game
#include "Building/RZ_TurretBuilding.h"
#include "Core/RZ_GameInstance.h"
#include "Core/RZ_GameSettings.h"
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
	TurretMeshCT->SetAbsolute(false, false, false);
	TurretMeshCT->SetCollisionProfileName("IgnoreAll");
	TurretMeshCT->SetGenerateOverlapEvents(false);
	TurretMeshCT->SetCustomDepthStencilValue(1);

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

	ProjectileWeaponSettings = *Cast<URZ_GameInstance>(GetGameInstance())->
	                            GetWeaponSystemModuleSettings()->GetProjectileWeaponInfoFromRow(DataTableRowName);
	
	TurretMeshDefaultMaterial = TurretMeshCT->GetMaterial(0);
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
	// Attach a projectile weapon

	for (uint8 MuzzleID = 0; MuzzleID <= ProjectileWeaponSettings.BarrelCount; MuzzleID++)
	{
		const FName MuzzleSocketName = *("MuzzleSocket_0" + FString::FromInt(MuzzleID));
		
		const FVector SpawnLocation = FVector(
			TurretMeshCT->GetSocketLocation(MuzzleSocketName).X,
			TurretMeshCT->GetSocketLocation(MuzzleSocketName).Y,
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
			                                         TurretMeshCT->GetSocketLocation(MuzzleSocketName),
			                                         TurretMeshCT->GetSocketRotation(MuzzleSocketName),
			                                         FVector(1.5f)
			);
		}
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

void ARZ_TurretBuilding::OnHoverStart()
{
	Super::OnHoverStart();
	
	TurretMeshCT->SetRenderCustomDepth(true);
}

void ARZ_TurretBuilding::OnHoverEnd()
{
	Super::OnHoverEnd();
	
	TurretMeshCT->SetRenderCustomDepth(false);
}

void ARZ_TurretBuilding::OnSelectionUpdated(bool bNewIsSelected)
{
	Super::OnSelectionUpdated(bNewIsSelected);

	TurretMeshCT->SetVisibility(bNewIsSelected);

	// weird stuff happening
	
	ARZ_PawnAIController* PawnAIController = Cast<ARZ_PawnAIController>(GetOwner());
	if (PawnAIController)
	{
		PawnAIController->ToggleAI(!bNewIsSelected);
	}
}

void ARZ_TurretBuilding::EnableBuildMode(bool bNewIsEnabled)
{
	Super::EnableBuildMode(bNewIsEnabled);

	if (bNewIsEnabled)
	{
		TurretMeshCT->SetMaterial(0, GameSettings->ItemSpawnMaterial_Valid);
		GridMaterialMeshCT->SetVisibility(true);
	}
	else
	{
		TurretMeshCT->SetMaterial(0, TurretMeshDefaultMaterial);
		GridMaterialMeshCT->SetVisibility(false);
	}
}

void ARZ_TurretBuilding::UpdateBuildModeLocation(const FVector& SpawnLocation, const FVector& LerpedItemLocation)
{
	Super::UpdateBuildModeLocation(SpawnLocation, LerpedItemLocation);

	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, "ARZ_TurretBuilding::UpdateBuildModeLocation");
	
	if (IsValidBuildLocation())
	{
		if (TurretMeshCT->GetMaterial(0) != GameSettings->ItemSpawnMaterial_Valid)
			TurretMeshCT->SetMaterial(0, GameSettings->ItemSpawnMaterial_Valid);
	}
	else
	{
		if (TurretMeshCT->GetMaterial(0) != GameSettings->ItemSpawnMaterial_Invalid)
			TurretMeshCT->SetMaterial(0, GameSettings->ItemSpawnMaterial_Invalid);
	}
}
