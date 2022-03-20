// RZItemPlugin
#include "RZ_Projectile.h"
//#include "RZ_ItemPluginDataManager.h"
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
	CollisionSphereCT = CreateDefaultSubobject<USphereComponent>(FName("CollisionSphereCT"));
	CollisionSphereCT->InitSphereRadius(1.0f);
	CollisionSphereCT->SetCollisionProfileName("Projectile");
	CollisionSphereCT->SetGenerateOverlapEvents(true);
	CollisionSphereCT->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	RootComponent = CollisionSphereCT;

	MeshCT = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshCT"));
	MeshCT->SetCollisionProfileName("IgnoreAll");
	MeshCT->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	MeshCT->SetupAttachment(RootComponent);

	ParticleCT = CreateDefaultSubobject<UParticleSystemComponent>(FName("ParticleCT"));
	ParticleCT->SetupAttachment(RootComponent);

	ProjectileMovementCT = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovementCT"));
	ProjectileMovementCT->ProjectileGravityScale = 0.0f;
	ProjectileMovementCT->bRotationFollowsVelocity = true;

	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	//InitialLifeSpan = 7.0f;


}

void ARZ_Projectile::Init(const ARZ_ItemPluginDataManager* const NewDataManager, const FName WeaponDataRowName)
{
	DataManager = NewDataManager;

	SetLifeSpan(3.0f);
	/*WeaponData = NewDataManager->GetWeaponDataFromRow(WeaponDataRowName);
	if (WeaponData)
	{
		ProjectileMovementCT->InitialSpeed = WeaponData->ProjectileSpeed;
		ProjectileMovementCT->MaxSpeed = WeaponData->ProjectileSpeed;
	}*/


}

void ARZ_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
	//CollisionSphereCT->OnComponentBeginOverlap.AddDynamic(this, &ARZ_Projectile::OnOverlap);

	/*if (DataManager->ItemPluginSettings.bIsDebugEnabled_Projectile)
	{
		UE_LOG(LogTemp, Error, TEXT("ARZ_Projectile::BeginPlay : "), *this->GetName());
	}*/
}

void ARZ_Projectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetOwner()->GetOwner())
	{
		//UGameplayStatics::ApplyPointDamage(OtherActor, WeaponData->Damage, FVector::ZeroVector, SweepResult, Cast<APlayerController>(GetOwner()), nullptr, nullptr);
		//SpawnImpactFX(OtherActor, SweepResult.ImpactPoint, SweepResult.ImpactNormal);
		Destroy();
	}

	UE_LOG(LogTemp, Error, TEXT("ARZ_Projectile::OnOverlap. %s"), *OtherActor->GetName());
}

void ARZ_Projectile::SpawnImpactFX(AActor* HitActor, FVector ImpactPoint, FVector ImpactNormal)
{
	if (Cast<ACharacter>(HitActor))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData->CharacterImpactParticle, ImpactPoint, UKismetMathLibrary::MakeRotFromZ(ImpactNormal));
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponData->WorldImpactParticle, ImpactPoint, UKismetMathLibrary::MakeRotFromZ(ImpactNormal));
	}
}

