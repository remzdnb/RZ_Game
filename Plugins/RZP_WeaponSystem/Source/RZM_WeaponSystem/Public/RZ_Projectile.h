/// RemzDNB
///
///	RZ_Projectile.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_WeaponSystem.h"
//
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RZ_Projectile.generated.h"

UCLASS()
class RZM_WEAPONSYSTEM_API ARZ_Projectile : public AActor
{
	GENERATED_BODY()

public:

	ARZ_Projectile();

	void Init(const FRZ_ProjectileWeaponSettings& NewProjectileWeaponSettings);

	virtual void BeginPlay() override;

private:
	
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	               const FHitResult& SweepResult);
	
	UFUNCTION()
	void SpawnImpactFX(AActor* HitActor, FVector ImpactPoint, FVector ImpactNormal);

	//

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionSphereComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ParticleComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovementComp;

	//

	class ARZ_ProjectileWeapon* ProjectileWeaponOwner;
	APawn* PawnOwner;
	AController* ControllerOwner;
	
	class URZ_WeaponSystemModuleSettings* ItemActorModuleSettings;
	FRZ_ProjectileWeaponSettings ProjectileWeaponSettings;


};
