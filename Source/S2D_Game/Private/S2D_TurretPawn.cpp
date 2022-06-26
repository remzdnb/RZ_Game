/// RemzDNB

// RZ_Game
#include "S2D_TurretPawn.h"
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

AS2D_TurretPawn::AS2D_TurretPawn()
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

void AS2D_TurretPawn::BeginPlay()
{
	Super::BeginPlay();

	GameSettings = Cast<URZ_GameInstance>(GetGameInstance())->GetGameSettings();
	AIController = Cast<ARZ_PawnAIController>(AIController);

	ProjectileWeaponSettings = *Cast<URZ_GameInstance>(GetGameInstance())->
	                            GetWeaponSystemModuleSettings()->GetProjectileWeaponInfoFromRow(DataTableRowName);
	
	TurretMeshDefaultMaterial = TurretMeshCT->GetMaterial(0);
}

void AS2D_TurretPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	if (bWantToFire && CurrentTime - LastFireTime >= FireRate)
	{
		LastFireTime = CurrentTime;
		FireOnce();
	}
}

void AS2D_TurretPawn::FireOnce()
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

void AS2D_TurretPawn::SetActiveTarget(AActor* NewActiveTarget)
{
	if (!TurretMeshCT) { return; }

	if (NewActiveTarget)
	{
		Cast<AController>(GetOwner())->SetControlRotation(
			UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), NewActiveTarget->GetActorLocation())
		);
	}
}

void AS2D_TurretPawn::SetWantToFire(bool bNewWantToFire)
{
	Super::SetWantToFire(bNewWantToFire);

	bWantToFire = bNewWantToFire;
}

void AS2D_TurretPawn::OnHoverStart()
{
	Super::OnHoverStart();
	
	TurretMeshCT->SetRenderCustomDepth(true);
}

void AS2D_TurretPawn::OnHoverEnd()
{
	Super::OnHoverEnd();
	
	TurretMeshCT->SetRenderCustomDepth(false);
}
/*
void AS2D_TurretPawn::OnInventorySelection(bool bNewIsSelected)
{
	Super::OnInventorySelection(bNewIsSelected);

	bNewIsSelected
		? TurretMeshCT->SetMaterial(0, GameSettings->ItemSpawnMaterial_Valid)
		: TurretMeshCT->SetMaterial(0, TurretMeshDefaultMaterial);

	// if is selected, update demo
	
	// weird stuff happening
	
	ARZ_PawnAIController* PawnAIController = Cast<ARZ_PawnAIController>(GetOwner());
	if (PawnAIController)
	{
		PawnAIController->ToggleAI(!bNewIsSelected);
	}
}*/