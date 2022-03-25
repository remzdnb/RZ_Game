/// RemzDNB
///
///	RZ_Projectile.h
///	Base class for physics based projectiles.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_ItemActor.h"
//
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RZ_Projectile.generated.h"

UCLASS()
class RZM_ITEMACTOR_API ARZ_Projectile : public AActor
{
	GENERATED_BODY()

public:

	ARZ_Projectile();

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

protected:

	virtual void BeginPlay() override;

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionSphereComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovementComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ParticleComp;

	//

	class URZ_ItemActorModuleSettings* ItemActorModuleSettings;
	const FRZ_ProjectileWeaponData* WeaponData;

	UFUNCTION() void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void SpawnImpactFX(AActor* HitActor, FVector ImpactPoint, FVector ImpactNormal);
};
