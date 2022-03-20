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

public:

	void Init(const class ARZ_ItemPluginDataManager* const NewDataManager, const FName WeaponDataRowName);

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionSphereCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovementCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ParticleCT;

	//

	const class ARZ_ItemPluginDataManager* DataManager;
	const FRZ_ProjectileWeaponData* WeaponData;

	UFUNCTION() void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void SpawnImpactFX(AActor* HitActor, FVector ImpactPoint, FVector ImpactNormal);
};
