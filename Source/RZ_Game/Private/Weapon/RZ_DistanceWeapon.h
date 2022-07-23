/// RemzDNB
///
///	Base class for any weapon that spawns projectiles.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Weapon.h"
//
#include "Components/TimelineComponent.h"
#include "RZ_DistanceWeapon.generated.h"

class USplineMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFireDelegate, float, FireTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReloadDelegate, float, ReloadTime);

UCLASS()
class RZ_GAME_API ARZ_DistanceWeapon : public ARZ_Weapon
{
	GENERATED_BODY()

public:

	ARZ_DistanceWeapon();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//

	//FORCEINLINE const FRZ_DistanceWeaponSettings& GetProjectileWeaponData() const { return ProjectileWeaponSettings; }

	//
	
	FFireDelegate OnFire;
	FReloadDelegate OnReload;

private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RZ", meta = (AllowPrivateAccess = "true"))
	FRZ_DistanceWeaponSettings DistanceWeaponSettings;
	
	/// Fire
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
private:

	UFUNCTION()
	void FireTick();

	UFUNCTION() 
	void FireOnce();

	UFUNCTION()
	void ApplyDamage(AActor* TargetActor);
	
	//

	UFUNCTION(NetMulticast, Unreliable)
	void SpawnFireFXMulticast();

	UFUNCTION()
	void SpawnImpactFX(const FHitResult& HitResult);
	
	/// Reload
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UFUNCTION()
	void SetWantsToReload(bool bNewWantsToReload);

private:

	UPROPERTY() bool bWantsToReload;
	UPROPERTY() class UCurveFloat* ReloadCurve;
	UPROPERTY() FTimeline ReloadTimeline;
	UPROPERTY() float ReloadTimelineProgressValue;

	UFUNCTION(Server, Reliable)
	void SetWantsToReloadServer(bool bNewWantsToReload);

	UFUNCTION()
	void StartReloading();

	UFUNCTION()
	void StopReloading();

	UFUNCTION()
	void ReloadTimelineProgress(float Value);

	UFUNCTION()
	void ReloadTimelineEnd();

	/// Attachments
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MagMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BarrelMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ScopeMeshComp;

	/// Aim spline mesh
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:

	void SetupViewSpline();
	void UpdateViewSpline(float DeltaTime);

	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USplineMeshComponent* ViewSplineCT;
	
	/// Debug
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:
	
	void Debug(float DeltaTime);
	
};

