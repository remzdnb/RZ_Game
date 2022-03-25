// ItemActor Module
#include "RZ_Projectile.h"
#include "RZ_Item.h"
// Engine
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ARZ_Projectile::ARZ_Projectile()
{
	CollisionSphereComp = CreateDefaultSubobject<USphereComponent>(FName("CollisionSphereCT"));
	CollisionSphereComp->InitSphereRadius(1.0f);
	CollisionSphereComp->SetCollisionProfileName("ProjectilePreset");
	CollisionSphereComp->SetGenerateOverlapEvents(true);
	CollisionSphereComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	RootComponent = CollisionSphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshCT"));
	MeshComp->SetCollisionProfileName("IgnoreAll");
	MeshComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	MeshComp->SetupAttachment(RootComponent);

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(FName("ParticleCT"));
	ParticleComp->SetupAttachment(RootComponent);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovementCT"));
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;

	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void ARZ_Projectile::BeginPlay()
{
	Super::BeginPlay();

	ItemActorModuleSettings = Cast<IRZ_ItemActorModuleInterface>(GetGameInstance())->GetItemActorModuleSettings();
	WeaponData = ItemActorModuleSettings->GetProjectileWeaponInfoFromRow(Cast<ARZ_Item>(GetOwner())->GetDataRowName());
	if (WeaponData)
	{
		//ProjectileMovementCT->InitialSpeed = WeaponData->ProjectileSpeed;
		//ProjectileMovementCT->MaxSpeed = WeaponData->ProjectileSpeed;
	}

	//

	CollisionSphereComp->OnComponentBeginOverlap.AddDynamic(this, &ARZ_Projectile::OnOverlap);

	SetLifeSpan(3.0f);
}

void ARZ_Projectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetOwner()->GetOwner())
	{
		//UGameplayStatics::ApplyPointDamage(OtherActor, WeaponData->Damage, FVector::ZeroVector, SweepResult, Cast<APlayerController>(GetOwner()), nullptr, nullptr);
		SpawnImpactFX(OtherActor, SweepResult.ImpactPoint, SweepResult.ImpactNormal);
		Destroy();
	}
}

void ARZ_Projectile::SpawnImpactFX(AActor* HitActor, FVector ImpactPoint, FVector ImpactNormal)
{
	if (WeaponData == nullptr)
		return;
	
	if (Cast<ACharacter>(HitActor))
	{
		if (WeaponData->CharacterImpactParticle)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData->CharacterImpactParticle, ImpactPoint, UKismetMathLibrary::MakeRotFromZ(ImpactNormal));
	}
	else
	{
		if (WeaponData->WorldImpactParticle)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData->WorldImpactParticle, ImpactPoint, UKismetMathLibrary::MakeRotFromZ(ImpactNormal));
	}
}

