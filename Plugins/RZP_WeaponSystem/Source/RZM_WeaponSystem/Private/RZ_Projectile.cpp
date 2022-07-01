// ItemActor Module
#include "RZ_Projectile.h"
#include "RZM_WeaponSystem.h"
#include "RZM_WeaponSystem.h"
// Engine
#include "RZ_AttributeComponent.h"
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

void ARZ_Projectile::Init(const FRZ_ProjectileWeaponSettings& NewProjectileWeaponSettings)
{
	ProjectileWeaponSettings = NewProjectileWeaponSettings;
}

void ARZ_Projectile::BeginPlay()
{
	Super::BeginPlay();

//	ItemActorModuleSettings = Cast<IRZ_WeaponSystemModuleInterface>(GetGameInstance())->GetWeaponSystemModuleSettings();

	// crash for turrets
	//ProjectileWeaponOwner = Cast<ARZ_ProjectileWeapon>(GetOwner());
	//PawnOwner = Cast<APawn>(ProjectileWeaponOwner->GetOwner());
	//ControllerOwner = Cast<AController>(PawnOwner->GetOwner());
	
	/*WeaponData = ItemActorModuleSettings->GetProjectileWeaponInfoFromRow(Cast<ARZ_Weapon>(GetOwner())->GetDataRowName());
	if (WeaponData)
	{
		ProjectileMovementCT->InitialSpeed = WeaponData->ProjectileSpeed;
		ProjectileMovementCT->MaxSpeed = WeaponData->ProjectileSpeed;
	}*/

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

	URZ_AttributeComponent* AttributeComponent =
		Cast<URZ_AttributeComponent>(OtherActor->GetComponentByClass(URZ_AttributeComponent::StaticClass()));
	if (AttributeComponent)
	{
		AttributeComponent->ApplyDamage(ProjectileWeaponSettings.Damage, SweepResult.Location, ControllerOwner, nullptr);
	}
	
	SpawnImpactFX(OtherActor, SweepResult.ImpactPoint, SweepResult.ImpactNormal);
	
	Destroy();
}

void ARZ_Projectile::SpawnImpactFX(AActor* HitActor, FVector ImpactPoint, FVector ImpactNormal)
{
	if (Cast<ACharacter>(HitActor))
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ProjectileWeaponSettings.CharacterImpactParticle,
			ImpactPoint,
			UKismetMathLibrary::MakeRotFromZ(ImpactNormal)
		);
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ProjectileWeaponSettings.WorldImpactParticle,
			ImpactPoint,
			UKismetMathLibrary::MakeRotFromZ(ImpactNormal)
		);
	}
}

