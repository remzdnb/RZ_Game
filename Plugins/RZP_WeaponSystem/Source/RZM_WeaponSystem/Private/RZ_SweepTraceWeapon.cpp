/// RemzDNB

#include "RZ_SweepTraceWeapon.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ARZ_SweepTraceWeapon::ARZ_SweepTraceWeapon()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = true;
}

void ARZ_SweepTraceWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ARZ_SweepTraceWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ARZ_SweepTraceWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsSweepEnabled)
	{
		FireOnce();
	}
}

/*void ARZ_SweepTraceWeapon::SetWantToUse(bool bNewWantToUse)
{
	Super::SetWantToUse(bNewWantToUse);

	bNewWantToUse ? EnableSweep() : DisableSweep();
}*/

void ARZ_SweepTraceWeapon::EnableSweep()
{
	GetWorld()->GetTimerManager().SetTimer(
		SweepTraceTimer,
		this,
		&ARZ_SweepTraceWeapon::DisableSweep,
		SweepMaxDuration,
		false,
		0.0f
	);

	bIsSweepEnabled = true;
	ItemState = ERZ_WeaponState::Firing;
	SetActorTickEnabled(true);
}

void ARZ_SweepTraceWeapon::DisableSweep()
{
	GetWorld()->GetTimerManager().ClearTimer(SweepTraceTimer);

	bIsSweepEnabled = true;
	ItemState = ERZ_WeaponState::Ready;
	SetActorTickEnabled(false);
}

void ARZ_SweepTraceWeapon::FireOnce()
{
	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = GetActorLocation() + GetActorRotation().Vector() * 10000.0f;
	TArray<FHitResult> HitResults;
	CalcSingleTrace(HitResults, TraceStart, TraceEnd);

	if (WeaponSystemModuleSettings->bDebugEquippedItems)
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, .3f, 0, 3.0f);
	
	for (const auto& HitResult : HitResults)
	{
		IRZ_WeaponDamageInterface* WeaponDamageInterface = Cast<IRZ_WeaponDamageInterface>(HitResult.Actor);
		if (WeaponDamageInterface)
		{
			WeaponDamageInterface->OnProjectileCollision(100.0, HitResult.Location, nullptr);
		}

		if (HitResult.Actor.IsValid())
		{
			return;
		}

		/*UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			ProjectileWeaponSettings.CharacterImpactParticle,
			HitResult.Location,
			FRotator::ZeroRotator,
			FVector(1.0f)
		);*/
	}
	
}