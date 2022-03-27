// ItemActor Module
#include "RZ_Projectile.h"
#include "RZ_Item.h"
#include "RZM_ItemActor.h"
// Engine
#include "RZ_ProjectileWeapon.h"
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

	ProjectileWeaponOwner = Cast<ARZ_ProjectileWeapon>(GetOwner());
	PawnOwner = Cast<APawn>(ProjectileWeaponOwner->GetOwner());
	ControllerOwner = Cast<AController>(PawnOwner->GetOwner());
	
	WeaponData = ItemActorModuleSettings->GetProjectileWeaponInfoFromRow(Cast<ARZ_Item>(GetOwner())->GetDataRowName());
	if (WeaponData)
	{
		//ProjectileMovementCT->InitialSpeed = WeaponData->ProjectileSpeed;
		//ProjectileMovementCT->MaxSpeed = WeaponData->ProjectileSpeed;
	}

	//

	if (GetLocalRole() == ROLE_Authority)
	{
		CollisionSphereComp->IgnoreActorWhenMoving(GetOwner(), true);
		CollisionSphereComp->IgnoreActorWhenMoving(GetOwner()->GetOwner(), true);
		CollisionSphereComp->OnComponentBeginOverlap.AddDynamic(this, &ARZ_Projectile::OnOverlap);	
	}
	
	SetLifeSpan(3.0f);
}

void ARZ_Projectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                               const FHitResult& SweepResult)
{
	if (GetLocalRole() < ROLE_Authority)
		return;

	if (OtherActor == GetOwner() || OtherActor == GetOwner()->GetOwner())
		return; // ?
	
	IRZ_ProjectileInterface* OtherActorProjectileInterface = Cast<IRZ_ProjectileInterface>(OtherActor);
	if (OtherActorProjectileInterface)
	{
		OtherActorProjectileInterface->OnProjectileCollision(WeaponData->Damage, SweepResult.Location, ControllerOwner);
	}

	SpawnImpactFX(OtherActor, SweepResult.ImpactPoint, SweepResult.ImpactNormal);
	Destroy();
}

void ARZ_Projectile::SpawnImpactFX(AActor* HitActor, FVector ImpactPoint, FVector ImpactNormal)
{
	if (WeaponData == nullptr)
		return;

	if (Cast<ACharacter>(HitActor))
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			WeaponData->CharacterImpactParticle,
			ImpactPoint,
			UKismetMathLibrary::MakeRotFromZ(ImpactNormal)
		);
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			WeaponData->WorldImpactParticle,
			ImpactPoint,
			UKismetMathLibrary::MakeRotFromZ(ImpactNormal)
		);
	}
}

