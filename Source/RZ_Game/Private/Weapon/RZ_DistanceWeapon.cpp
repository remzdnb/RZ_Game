#include "Weapon/RZ_DistanceWeapon.h"
#include "Weapon/RZ_Projectile.h"
// Engine 
#include "RZ_AbilitySystemComponent.h"
#include "RZ_AttributeSet.h"
#include "RZ_GameplayAbility.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "Net/UnrealNetwork.h"

#pragma region +++++ ...

ARZ_DistanceWeapon::ARZ_DistanceWeapon()
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

void ARZ_DistanceWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld()) { return; }
}

void ARZ_DistanceWeapon::BeginPlay()
{
	Super::BeginPlay();
	
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

void ARZ_DistanceWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Debug(DeltaTime);
	
	if (WeaponState == ERZ_WeaponState::Disabled)
		return;
	
	//UE_LOG(LogTemp, Warning, TEXT("ARZ_DistanceWeapon::Tick"));
	
	if (GetLocalRole() == ROLE_Authority)
	{
		//if (ClipAmmo == 0)
			//StartReloading();
		
		//else if (ProjectileWeaponSettings.bIsAutoFire)

		FireTick();
	}
	
	//

	ReloadTimeline.TickTimeline(DeltaTime);

	UpdateViewSpline(DeltaTime);
	
	//
	
	//const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerTargetLocation); // target location
	//const FRotator LerpedRotation = FMath::Lerp(GetActorRotation(), TargetRotation, 1.0f);
	//SetActorRotation(FRotator(LerpedRotation.Pitch, LerpedRotation.Yaw, 0.0f));
}

/*void ARZ_DistanceWeapon::UpdateWeaponState(ERZ_WeaponState NewWeaponState)
{
	WeaponState = NewWeaponState;
}*/

#pragma endregion

#pragma region +++++ Fire ...

void ARZ_DistanceWeapon::FireTick()
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	if (WeaponState == ERZ_WeaponState::Firing && (CurrentTime - LastUseTime) >= DistanceWeaponSettings.DelayBetweenShots)
	{
		WeaponState = ERZ_WeaponState::Ready;
	}
		
	if (bOwnerWantToUse &&
		WeaponState == ERZ_WeaponState::Ready &&
		(CurrentTime - LastUseTime) >= DistanceWeaponSettings.DelayBetweenShots)
	{
		WeaponState = ERZ_WeaponState::Firing;
		LastUseTime = CurrentTime;
		FireOnce();
	}


}

void ARZ_DistanceWeapon::FireOnce()
{
	WeaponState = ERZ_WeaponState::Firing;
	
	const FVector TraceStart = RootMeshComp->GetSocketLocation("MuzzleSocket_00");
	FVector TargetActorLocation;

	const AActor* TargetActor = nullptr;
	const IRZ_AIOwnerInterface* AIOwnerInterface = Cast<IRZ_AIOwnerInterface>(GetOwner());
	if (AIOwnerInterface && AIOwnerInterface->GetTargetActor())
	{
		TargetActor = AIOwnerInterface->GetTargetActor();
		TargetActorLocation = TargetActor->GetActorLocation();
	}
	const FVector TraceEnd = TraceStart + UKismetMathLibrary::FindLookAtRotation(TraceStart, TargetActorLocation).Vector() * 2000.0f;

	TArray<FHitResult> HitResults;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetOwner());

	// well we need to check for teams here, so maybe with gameplay tags / abilities ?

	// ECC_GTC10 == OverlapPawnBlockWorld.
	const bool bIsBlockingHit = GetWorld()->LineTraceMultiByChannel(
		HitResults,
		TraceStart,
		TraceEnd,
		ECC_GameTraceChannel10,
		TraceParams
	);
	bool bIsDamagingHit = false;
	if (!bIsBlockingHit)
	{
		for (const auto& HitResult : HitResults)
		{
			if (HitResult.Actor == TargetActor)
			{
				ApplyDamage(HitResult.Actor.Get());
				SpawnImpactFX(HitResult);
				bIsDamagingHit = true;
			}
		}
	}
	else
	{
		
	}
		
	if (RZGameSettings->bDebugWeapons)
	{
		if (bIsDamagingHit)
			UKismetSystemLibrary::DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Yellow, .1f, 2.0f);
		else
			UKismetSystemLibrary::DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, .1f, 2.0f);
	}
	
	SpawnFireFXMulticast();

	OnWeaponFired.Broadcast(this);

	//

	/*FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, PlayerTargetLocation);

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
*/
}


void ARZ_DistanceWeapon::ApplyDamage(AActor* TargetActor)
{
	URZ_AbilitySystemComponent* ASComp = Cast<URZ_AbilitySystemComponent>(
		TargetActor->GetComponentByClass(URZ_AbilitySystemComponent::StaticClass())
	);
	if (ASComp)
	{
		FGameplayEffectContextHandle EffectContext = ASComp->MakeEffectContext();
		EffectContext.AddSourceObject(this); 

		/*const FGameplayEffectSpecHandle SpecHandle = ASComp->MakeOutgoingSpec(
			URZ_AddHealth_GE::StaticClass(),
			1,
			EffectContext
		);*/

		//SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), DistanceWeaponSettings.Damage);


		UGameplayEffect* GameplayEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("NewGE")));
		//AddHealthGE->HealthToAdd = -50.0f;
		FGameplayEffectModifierMagnitude GEModifier;
		FGameplayModifierInfo ModifierInfo;
		ModifierInfo.Attribute = URZ_AttributeSet::GetHealthAttribute();
		ModifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(-25.0f);
		ModifierInfo.ModifierOp = EGameplayModOp::Additive;
		GameplayEffect->Modifiers.Add(ModifierInfo);
		GameplayEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
		ASComp->ApplyGameplayEffectToTarget(GameplayEffect, ASComp, 1.0f, EffectContext);

		/*if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle EffectHandle = ASComp->ApplyGameplayEffectSpecToTarget(
				*SpecHandle.Data.Get(),
				ASComp
			);

			/*FActiveGameplayEffectHandle EffectHandle = ASComp->ApplyGameplayEffectSpecToTarget(
				*SpecHandle.Data.Get(),
				ASComp
			);
		}*/
	}
}

void ARZ_DistanceWeapon::SpawnFireFXMulticast_Implementation()
{
	if (WeaponSettings.MuzzleParticle)
	{
		UGameplayStatics::SpawnEmitterAttached(
			WeaponSettings.MuzzleParticle,
			RootMeshComp,
			"MuzzleSocket_00",
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTargetIncludingScale
		);
		
		//UE_LOG(LogTemp, Display, TEXT("ARZ_DistanceWeapon::SpawnFireFXMulticast - 2"), *this->GetName());
	}

	/*if (WeaponData.FireSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponData.FireSound, GetActorLocation());
	}*/
}


void ARZ_DistanceWeapon::SpawnImpactFX(const FHitResult& HitResult)
{
	if (WeaponSettings.WorldImpactParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			WeaponSettings.WorldImpactParticle,
			HitResult.Location,
			FRotator::ZeroRotator
		);
	}
}

#pragma endregion

#pragma region +++++ Reload ...

void ARZ_DistanceWeapon::SetWantsToReload(bool bNewWantsToReload)
{
	bWantsToReload = bNewWantsToReload;

	if (GetLocalRole() < ROLE_Authority)
		SetWantsToReloadServer(bNewWantsToReload);
}

void ARZ_DistanceWeapon::SetWantsToReloadServer_Implementation(bool bNewWantsToReload)
{
	SetWantsToReload(bNewWantsToReload);
}

void ARZ_DistanceWeapon::StartReloading()
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

void ARZ_DistanceWeapon::StopReloading()
{
	if (WeaponState == ERZ_WeaponState::Reloading)
	{
		ReloadTimeline.Stop();
		WeaponState = ERZ_WeaponState::Ready;
	}
}

void ARZ_DistanceWeapon::ReloadTimelineProgress(float Value)
{
	ReloadTimelineProgressValue = Value;
}

void ARZ_DistanceWeapon::ReloadTimelineEnd()
{
	if (WeaponState == ERZ_WeaponState::Reloading && ReloadTimeline.GetPlaybackPosition() == ReloadTimeline.GetTimelineLength())
	{
		// clip

		WeaponState = ERZ_WeaponState::Ready;
	}
}

#pragma endregion

#pragma region +++++ Aim spline mesh ...

void ARZ_DistanceWeapon::SetupViewSpline()
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

void ARZ_DistanceWeapon::UpdateViewSpline(float DeltaTime)
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

	/*ViewSplineCT->SetWorldLocation(FVector(
		RootMeshComp->GetSocketLocation("MuzzleSocket_00").X,
		RootMeshComp->GetSocketLocation("MuzzleSocket_00").Y,
		BASEVIEWHEIGHT
	));*/

	//const float Distance = FVector::Dist(ViewSplineCT->GetComponentLocation(), PlayerTargetLocation);

	//ViewSplineCT->SetEndPosition(FVector(Distance, 0.0f, 0.0f), true);
	//ViewSplineCT->SetWorldRotation(
		//UKismetMathLibrary::FindLookAtRotation(ViewSplineCT->GetComponentLocation(), PlayerTargetLocation));

	ViewSplineCT->SetVisibility(true);
}

#pragma endregion

#pragma region +++++ Debug ...

void ARZ_DistanceWeapon::Debug(float DeltaTime)
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
		" // bWantsToUse : " + (bOwnerWantToUse ? "True" : "False") +
		" // State : " + StateString;
		
	
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, Color, FString::Printf(TEXT("%s"), *StringToPrint));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, Color, TEXT(" "));
}

#pragma endregion