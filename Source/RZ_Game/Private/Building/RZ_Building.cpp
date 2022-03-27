/// RemzDNB

#include "Building/RZ_Building.h"

#include "Components/BoxComponent.h"
#include "Pawn/RZ_PawnCombatComponent.h"

ARZ_Building::ARZ_Building()
{
	RootSceneComp = CreateDefaultSubobject<USceneComponent>(FName("RootSceneComp"));
	RootComponent = RootSceneComp;

	CollisionBoxComp = CreateDefaultSubobject<UBoxComponent>(FName("CollisionBoxComp"));
	CollisionBoxComp->SetCollisionProfileName("Pawn");
	CollisionBoxComp->SetupAttachment(RootComponent);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMeshComp"));
	SkeletalMeshComp->SetCollisionProfileName("IgnoreAll");
	SkeletalMeshComp->SetupAttachment(RootComponent);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComp"));
	StaticMeshComp->SetCollisionProfileName("IgnoreAll");
	StaticMeshComp->SetupAttachment(RootComponent);
	
	PawnCombatComp = CreateDefaultSubobject<URZ_PawnCombatComponent>(FName("PawnCombatComp"));
	
	PrimaryActorTick.bCanEverTick = false;
}

void ARZ_Building::OnConstruction(const FTransform& InTransform)
{
	Super::OnConstruction(InTransform);

	// Align the collision box to the floor level.
	
	const FVector BoxExtent = CollisionBoxComp->GetScaledBoxExtent();
	CollisionBoxComp->SetRelativeLocation(FVector(0.0f, 0.0f, BoxExtent.Z));
}


void ARZ_Building::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnCombatComp->Init(1000.0f, 1000.0f);
}

void ARZ_Building::BeginPlay()
{
	Super::BeginPlay();

	PawnCombatComp->OnHealthReachedZero.AddUniqueDynamic(this, &ARZ_Building::OnDestroyed);
}

void ARZ_Building::OnProjectileCollision(float ProjectileDamage, const FVector& HitLocation,
                                         AController* InstigatorController)
{
	if (GetLocalRole() < ROLE_Authority || PawnCombatComp == nullptr)
		return;

	PawnCombatComp->ApplyDamage(ProjectileDamage, HitLocation, InstigatorController, nullptr);
}

void ARZ_Building::OnDestroyed()
{
	Destroy();
}
