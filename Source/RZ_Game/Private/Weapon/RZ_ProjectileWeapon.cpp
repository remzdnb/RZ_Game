#include "Weapon/RZ_ProjectileWeapon.h"
#include "Weapon/RZ_Projectile.h"
// Engine 
#include "Components/SkeletalMeshComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "Net/UnrealNetwork.h"

#pragma region +++++ ...

ARZ_ProjectileWeapon::ARZ_ProjectileWeapon()
{
	MagMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("MagMeshComp"));
	MagMeshComp->SetCollisionProfileName("IgnoreAll");
	MagMeshComp->SetupAttachment(RootMeshComp);
	BarrelMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("BarrelMeshComp"));
	BarrelMeshComp->SetCollisionProfileName("IgnoreAll");
	BarrelMeshComp->SetupAttachment(RootMeshComp);
	ScopeMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("ScopeMeshComp"));
	ScopeMeshComp->SetCollisionProfileName("IgnoreAll");
	ScopeMeshComp->SetupAttachment(RootMeshComp);

	//AimSplineMeshCT = CreateDefaultSubobject<USplineMeshComponent>(FName("AimSplineMeshCT"));
	//AimSplineMeshCT->SetupAttachment(RootComponent);

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	WeaponState = ERZ_WeaponState::Disabled;
}

void ARZ_ProjectileWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld()) { return; }
}

void ARZ_ProjectileWeapon::BeginPlay()
{
	Super::BeginPlay();

	ClipAmmo = ProjectileWeaponSettings.MaxClipAmmo;
	StockAmmo = ProjectileWeaponSettings.MaxStockAmmo;

	if (RZGameSettings)
	{
		//const FRZ_ProjectileWeaponSettings* ProjectileWeaponSettings = RZGameSettings->GetProjectileWeaponSettingsFromTableRow(ItemName);
		//if (ProjectileWeaponSettings)
		//{
		//nah
		//}



		//SetupViewSpline();
	}

	/*ReloadCurve = DataManager->GlobalSettings.Linear1SCurve_0to1;
	if (ReloadCurve)
	{
		FOnTimelineFloat ReloadTimelineProgress;
		FOnTimelineEventStatic ReloadTimelineFinishedCallback;
		ReloadTimelineProgress.BindUFunction(this, FName("ReloadTimelineProgress"));
		ReloadTimelineFinishedCallback.BindUFunction(this, FName("ReloadTimelineEnd"));
		ReloadTimeline.AddInterpFloat(ReloadCurve, ReloadTimelineProgress);
		ReloadTimeline.SetTimelineFinishedFunc(ReloadTimelineFinishedCallback);
		ReloadTimeline.SetLooping(false);
		ReloadTimeline.SetPlayRate(1 / WeaponData.ReloadTime);
	}*/
}

void ARZ_ProjectileWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Debug(DeltaTime);
	
	if (WeaponState == ERZ_WeaponState::Disabled)
		return;
	
	UE_LOG(LogTemp, Warning, TEXT("ARZ_ProjectileWeapon::Tick"));
	
	if (GetLocalRole() == ROLE_Authority)
	{
		if (ClipAmmo == 0)
			StartReloading();
		
		//else if (ProjectileWeaponSettings.bIsAutoFire)

		FireTick();
	}
	
	//

	ReloadTimeline.TickTimeline(DeltaTime);

	UpdateViewSpline(DeltaTime);
	
	//
	
	const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerTargetLocation); // target location
	const FRotator LerpedRotation = FMath::Lerp(GetActorRotation(), TargetRotation, 1.0f);
	//SetActorRotation(FRotator(LerpedRotation.Pitch, LerpedRotation.Yaw, 0.0f));
}

/*void ARZ_ProjectileWeapon::UpdateWeaponState(ERZ_WeaponState NewWeaponState)
{
	WeaponState = NewWeaponState;
}*/

#pragma endregion

#pragma region +++++ Fire ...

void ARZ_ProjectileWeapon::FireTick()
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	if (WeaponState == ERZ_WeaponState::Firing && (CurrentTime - LastUseTime) >= ProjectileWeaponSettings.DelayBetweenShots)
	{
		WeaponState = ERZ_WeaponState::Ready;
	}
		
	if (bWantToUse &&
		WeaponState == ERZ_WeaponState::Ready &&
		(CurrentTime - LastUseTime) >= ProjectileWeaponSettings.DelayBetweenShots)
	{
		WeaponState = ERZ_WeaponState::Firing;
		LastUseTime = CurrentTime;
		FireOnce();
	}


}

void ARZ_ProjectileWeapon::FireOnce()
{
	WeaponState = ERZ_WeaponState::Firing;

	const FVector SpawnLocation = RootMeshComp->GetSocketLocation("MuzzleSocket_00");
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, PlayerTargetLocation);

	if (TurretComp)
	{
		SpawnRotation = RootMeshComp->GetComponentRotation();
		SpawnRotation.Yaw += 90.0f;
	}
	
	FTransform SpawnTransform(SpawnRotation, SpawnLocation, FVector(1.0f));
	
	// projectile spacing
	bool bIsRightProjectile = true;
	for (int32 Index = 0; Index < ProjectileWeaponSettings.TraceCountPerBarrel; Index++)
	{
		if (Index == 0)
		{
			SpawnProjectile(SpawnTransform);
		}
		else
		{
			if (bIsRightProjectile)
			{
				const FRotator OffsetedSpawnRotation = FRotator(
					SpawnRotation.Pitch,
					SpawnRotation.Yaw + Index * ProjectileWeaponSettings.TraceSpread,
					0.0f
				);
				const FTransform OffsetedSpawnTransform(OffsetedSpawnRotation, SpawnLocation, FVector(1.0f));
				SpawnProjectile(OffsetedSpawnTransform);
			}
			else
			{
				const FRotator OffsetedSpawnRotation = SpawnRotation + FRotator(0.0f, Index * ProjectileWeaponSettings.TraceSpread * -1, 0.0f);
				const FTransform OffsetedSpawnTransform(OffsetedSpawnRotation, SpawnLocation, FVector(1.0f));
				SpawnProjectile(OffsetedSpawnTransform);
			}

			bIsRightProjectile = !bIsRightProjectile;
		}
	}


	SpawnFireFXMulticast();

	OnWeaponFired.Broadcast(this);
	/*if (OwnerInventoryCT.IsValid())
	{
		OwnerInventoryCT->InventoryUpdatedEvent.Broadcast(true);
		OwnerInventoryCT->GetOwnerInventoryInterface()->OnItemUse(WeaponData.FireTime, ItemData.AnimType);
	}*/

	//FireEvent.Broadcast(0.0f);*/
}



void ARZ_ProjectileWeapon::SpawnProjectile(const FTransform& SpawnTransform)
{
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
	
	//UKismetSystemLibrary::DrawDebugLine(GetWorld(), SpawnLocation, TargetLocation, FColor::Yellow, 2.0f, 2.0f);
	//VCUtilityLibrary::PrintFloatToScreen("Fire Once : ", SpawnLocation.X, FColor::Purple, -1, 2.0f);
}

void ARZ_ProjectileWeapon::SpawnFireFXMulticast_Implementation()
{
	/*if (ProjSettings->MuzzleParticle)
	{
		UGameplayStatics::SpawnEmitterAttached(
			ProjSettings->MuzzleParticle,
			RootMeshComp,
			"MuzzleSocket_00",
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTargetIncludingScale
		);
		
		//UE_LOG(LogTemp, Display, TEXT("ARZ_ProjectileWeapon::SpawnFireFXMulticast - 2"), *this->GetName());
	}*/

	/*if (WeaponData.FireSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponData.FireSound, GetActorLocation());
	}*/
}


void ARZ_ProjectileWeapon::SpawnImpactFX(const FHitResult& HitResult)
{
}

#pragma endregion

#pragma region +++++ Reload ...

void ARZ_ProjectileWeapon::SetWantsToReload(bool bNewWantsToReload)
{
	bWantsToReload = bNewWantsToReload;

	if (GetLocalRole() < ROLE_Authority)
		SetWantsToReloadServer(bNewWantsToReload);
}

void ARZ_ProjectileWeapon::SetWantsToReloadServer_Implementation(bool bNewWantsToReload)
{
	SetWantsToReload(bNewWantsToReload);
}

void ARZ_ProjectileWeapon::StartReloading()
{	
	/*if (OwnerInventoryCT.IsValid() == false)
		return;

	if (WeaponState == ERZ_WeaponState::Ready)
	{
		if (MagAttachment)
			MagAttachment->Destroy();

		//OwnerInventoryCT->RefillEquippedItem();
		if (MagAttachment)
		{
			ReloadTimeline.PlayFromStart();
			WeaponState = ERZ_WeaponState::Reloading;
			//ReloadEvent.Broadcast(WeaponData.ReloadTime);

			//OwnerInventoryCT->InventoryUpdatedEvent.Broadcast(true);
			//OwnerInventoryCT->GetOwnerInventoryInterface()->OnItemReload(WeaponData.ReloadTime, ItemData.AnimType);
		}
	}*/
}

void ARZ_ProjectileWeapon::StopReloading()
{
	if (WeaponState == ERZ_WeaponState::Reloading)
	{
		ReloadTimeline.Stop();
		WeaponState = ERZ_WeaponState::Ready;
	}
}

void ARZ_ProjectileWeapon::ReloadTimelineProgress(float Value)
{
	ReloadTimelineProgressValue = Value;
}

void ARZ_ProjectileWeapon::ReloadTimelineEnd()
{
	if (WeaponState == ERZ_WeaponState::Reloading && ReloadTimeline.GetPlaybackPosition() == ReloadTimeline.GetTimelineLength())
	{
		// clip

		WeaponState = ERZ_WeaponState::Ready;
	}
}

#pragma endregion

#pragma region +++++ Aim spline mesh ...

void ARZ_ProjectileWeapon::SetupViewSpline()
{
	ViewSplineCT = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	ViewSplineCT->RegisterComponentWithWorld(GetWorld());
	ViewSplineCT->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	ViewSplineCT->SetMobility(EComponentMobility::Movable);
	//ViewSplineCT->SetStaticMesh(WeaponSystemModuleSettings->EngineCylinderMesh);
	//ViewSplineCT->SetMaterial(0, WeaponSystemModuleSettings->ViewSplineMaterial);
	ViewSplineCT->SetWorldScale3D(FVector(1.0f, 0.05f, 0.05f));
	ViewSplineCT->AttachToComponent(RootMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ViewSplineCT->SetForwardAxis(ESplineMeshAxis::Z, true);
}

void ARZ_ProjectileWeapon::UpdateViewSpline(float DeltaTime)
{
	//if (!Cast<AActor>(GetOwner()))
	//if (!GetOwner()->IsLocallyControlled() == false)
		//return;
	
	if (ViewSplineCT == nullptr) { return; }

	/*if (bIsSelected == false)
	{
		ViewSplineCT->SetVisibility(false);
		return;
	}*/

	ViewSplineCT->SetVisibility(true);

	/// Snap mesh to projectile plane.

	ViewSplineCT->SetWorldLocation(FVector(
		RootMeshComp->GetSocketLocation("MuzzleSocket_00").X,
		RootMeshComp->GetSocketLocation("MuzzleSocket_00").Y,
		BASEVIEWHEIGHT
	));

	const float Distance = FVector::Dist(ViewSplineCT->GetComponentLocation(), PlayerTargetLocation);

	ViewSplineCT->SetEndPosition(FVector(Distance, 0.0f, 0.0f), true);
	ViewSplineCT->SetWorldRotation(
		UKismetMathLibrary::FindLookAtRotation(ViewSplineCT->GetComponentLocation(), PlayerTargetLocation));

	ViewSplineCT->SetVisibility(true);
}

#pragma endregion

#pragma region +++++ Network / Debug ...

void ARZ_ProjectileWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARZ_ProjectileWeapon, ClipAmmo);
	DOREPLIFETIME(ARZ_ProjectileWeapon, StockAmmo);
}

void ARZ_ProjectileWeapon::Debug(float DeltaTime)
{
	if (!RZGameSettings) { return; }
	if (!RZGameSettings->bDebugWeapons) { return; }
	//if (!WeaponSystemModuleSettings) { return; }
	//if (bIsSelected && !WeaponSystemModuleSettings->bDebugEquippedItems) { return; }
	//if (!bIsSelected && !WeaponSystemModuleSettings->bDebugHolsteredItems) { return; }

	FString RoleString = "None";
	if (GetLocalRole() == ROLE_Authority) { RoleString = "Authority"; }
	else if (GetLocalRole() == ROLE_AutonomousProxy) { RoleString = "AutonomousProxy"; }
	else if (GetLocalRole() == ROLE_SimulatedProxy) { RoleString = "SimulatedProxy"; }
	
	FString StateString;
	if (WeaponState == ERZ_WeaponState::Disabled)
		StateString = "Disabled";
	if (WeaponState == ERZ_WeaponState::Ready)
		StateString = "Ready";
	else if (WeaponState == ERZ_WeaponState::Firing)
		StateString = "Firing";
	else if (WeaponState == ERZ_WeaponState::Reloading)
		StateString = "Reloading";

	FColor Color;
	if (GetLocalRole() == ROLE_Authority)
		Color = FColor::Red;
	else
		Color = FColor::Red;

	const FString StringToPrint = this->GetName() + " /// " +
		" // bWantsToUse : " + (bWantToUse ? "True" : "False") +
		" // State : " + StateString;
		
	
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, Color, FString::Printf(TEXT("%s"), *StringToPrint));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, Color, TEXT(" "));
}

#pragma endregion