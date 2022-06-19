/// RemzDNB
///
///	Base class for any weapon that spawns projectiles.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Weapon.h"
//
#include "Components/TimelineComponent.h"
#include "RZ_ProjectileWeapon.generated.h"

class USplineMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFireDelegate, float, FireTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReloadDelegate, float, ReloadTime);

UCLASS()
class RZM_WEAPONSYSTEM_API ARZ_ProjectileWeapon : public ARZ_Weapon
{
	GENERATED_BODY()

public:

	ARZ_ProjectileWeapon();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//

	FORCEINLINE const FRZ_ProjectileWeaponSettings& GetProjectileWeaponData() const { return ProjectileWeaponSettings; }

	//
	
	FFireDelegate OnFire;
	FReloadDelegate OnReload;

private:

	//
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRZ_ProjectileWeaponSettings ProjectileWeaponSettings;

	UPROPERTY(Replicated, Transient)
	uint32 ClipAmmo;

	UPROPERTY(Replicated, Transient)
	uint32 StockAmmo;
	
	/// Fire
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
private:

	UFUNCTION()
	void FireTick();

	UFUNCTION() 
	void FireOnce();

	UFUNCTION()
	void SpawnProjectile(const FTransform& SpawnTransform);

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
	UStaticMeshComponent* MagStaticMeshCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BarrelStaticMeshCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ScopeStaticMeshCT;

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

