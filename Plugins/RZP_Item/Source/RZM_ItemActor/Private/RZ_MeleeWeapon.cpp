#include "RZ_MeleeWeapon.h"
#include "RZM_ItemActor.h"
#include "RZM_ItemActor.h"
#include "RZM_ItemActor.h"
//
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ARZ_MeleeWeapon::ARZ_MeleeWeapon()
{
	RootStaticMeshCT = CreateDefaultSubobject<UStaticMeshComponent>(FName("RootStaticMeshCT"));
	RootStaticMeshCT->SetCollisionProfileName("IgnoreAll");
	RootStaticMeshCT->SetGenerateOverlapEvents(false);
	RootStaticMeshCT->SetCustomDepthStencilValue(1);
	RootStaticMeshCT->SetupAttachment(RootComponent);


	SetActorTickEnabled(true);
}

void ARZ_MeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

	/*const FRZ_MeleeWeaponData* const NewMeleeWeaponData = ->GetMeleeWeaponDataFromRow(DataRowName);
	if (NewMeleeWeaponData)
	{
		MeleeWeaponData = NewMeleeWeaponData;
	}*/
}

void ARZ_MeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*const float CurrentTime = GetWorld()->GetTimeSeconds();

	if ((CurrentTime - LastUsedTime) >= ItemData->UseTime)
	{
		if (ItemState == ERZ_ItemState::Firing)
			ItemState = ERZ_ItemState::Ready;
	}

	if (bWantsToUse && ItemState == ERZ_ItemState::Ready)
	{
		AttackOnce();
		ItemState = ERZ_ItemState::Firing;
		LastUsedTime = CurrentTime;
	}*/
}

void ARZ_MeleeWeapon::AttackOnce()
{
	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(100.0f);
	const FVector SweepStart = GetOwner()->GetActorLocation();
	const FVector SweepEnd = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 200.0f;
	TArray<FHitResult> OutResults;
	TArray<AActor*> HitActors;

	GetWorld()->SweepMultiByChannel(OutResults, SweepStart, SweepEnd, FQuat::Identity, ECC_Visibility, CollisionSphere);
	for (auto& HitResult : OutResults)
	{
		if (HitResult.Actor.IsValid() && HitActors.Contains(HitResult.Actor) == false)
		{
			UGameplayStatics::ApplyPointDamage(HitResult.Actor.Get(), MeleeWeaponData->BaseDamage, FVector::ZeroVector, HitResult, Cast<APlayerController>(GetOwner()), nullptr, nullptr);
			HitActors.Add(HitResult.Actor.Get());
		}
	}

	if (OwnerPawnInterface)
	{
		OwnerPawnInterface->OnItemUsed(DataRowName);
	}
}