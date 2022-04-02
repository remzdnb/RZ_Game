/// RemzDNB
///
///	Base class for any weapon that spawns projectiles.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_WeaponSystem.h"
#include "RZ_Weapon.h"
//
#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "RZ_ProjectileWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFireDelegate, float, FireTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReloadDelegate, float, ReloadTime);

UCLASS()
class RZM_WEAPONSYSTEM_API ARZ_ProjectileWeapon : public ARZ_Weapon
{
	GENERATED_BODY()

public:

	ARZ_ProjectileWeapon();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//

	FORCEINLINE const FRZ_ProjectileWeaponSettings& GetProjectileWeaponData() const { return ProjectileWeaponSettings; }
	
	//
	
	FFireDelegate OnFire;
	FReloadDelegate OnReload;

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* MuzzleTipSceneComp; // Location from where projectiles will be spawned.

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRZ_ProjectileWeaponSettings ProjectileWeaponSettings;

	UPROPERTY(Replicated, Transient)
	uint32 ClipAmmo;

	UPROPERTY(Replicated, Transient)
	uint32 StockAmmo;
	
	//

	void Debug(float DeltaTime);

	/// Fire
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
private:

	UFUNCTION()
	void FireTick();

	UFUNCTION() 
	void FireOnce();

	UFUNCTION()
	void SpawnProjectile();

	UFUNCTION(NetMulticast, Unreliable)
	void SpawnFireFXMulticast();
	
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

public:

	/*UFUNCTION()
	void ReplaceAttachment(ERZ_AttachmentType Type, class ARZ_Attachment* NewAttachment);

	UFUNCTION()
	bool RemoveAttachment(class ARZ_Attachment* NewAttachment);

	UFUNCTION(Server, Reliable)
	void RemoveAttachmentServer(ERZ_AttachmentType AttachmentType, class ARZ_Attachment* AttachmentRef);*/

	FORCEINLINE class ARZ_Attachment* GetMagAttachment() const { return MagAttachment; }
	FORCEINLINE class ARZ_Attachment* GetBarrelAttachment() const { return BarrelAttachment; }
	FORCEINLINE class ARZ_Attachment* GetScopeAttachment() const { return ScopeAttachment; }

protected:

	UPROPERTY(Replicated) class ARZ_Attachment* MagAttachment;
	UPROPERTY(Replicated) class ARZ_Attachment* BarrelAttachment;
	UPROPERTY(Replicated) class ARZ_Attachment* ScopeAttachment;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MagStaticMeshComp; // mesh components ?

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BarrelStaticMeshComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ScopeStaticMeshComp;

	//

	//UFUNCTION()
	//bool IsAttachmentInWhiteList(class ARZ_Attachment* Attachment);

	/// Aim spline mesh
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	private:

	UPROPERTY()
	class USplineMeshComponent* AimSplineMesh;

	void SetupAimSplineMesh();
	void UpdateAimSplineMesh();
	
};