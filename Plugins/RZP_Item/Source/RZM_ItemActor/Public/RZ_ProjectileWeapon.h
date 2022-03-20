///// ARZ_ProjectileWeapon - RemzDNB
///// Base class for any Weapon that can spawn projectiles.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Item.h"
#include "RZM_ItemActor.h"
#include "RZM_ItemActor.h"
//
#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "RZ_ProjectileWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFire, float, FireTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReload, float, ReloadTime);

UCLASS()
class RZM_ITEMACTOR_API ARZ_ProjectileWeapon : public ARZ_Item
{
	GENERATED_BODY()

public:

	ARZ_ProjectileWeapon();

	FFire FireEvent;
	FReload ReloadEvent;

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UFUNCTION()
	void Init(FName NewDataRowName);

	/* IRZ_InteractionInterface
	virtual void StartHover(class ARZ_PlayerController* InstigatorPC) { return; }
	virtual void StopHover(class ARZ_PlayerController* InstigatorPC) { return; }
	virtual void StartUse(class ARZ_PlayerController* InstigatorPC) override;
	virtual void StopUse(class ARZ_PlayerController* InstigatorPC) override;
	virtual class APawn* Enter(class APlayerController* InstigatorPC) { return nullptr; }
	virtual class APawn* Exit(class APlayerController* InstigatorPC) { return nullptr; }
	virtual void Open() { return; }
	virtual void Close() { return; }
	*/

	UFUNCTION() 
	void UpdateOwnerTargetLocation(FVector NewTargetLocation);

	FORCEINLINE UFUNCTION() FVector GetFacingLocation() const { return FacingLocation; }


	FORCEINLINE UFUNCTION() const FRZ_ProjectileWeaponData& GetWeaponData() const { return WeaponData; }



	
protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* PStartSceneCT; // Location from where projectiles will be spawned.

	//



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector FacingLocation;

private:
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRZ_ProjectileWeaponData WeaponData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<class UParticleSystemComponent*> MuzzleParticleList; // Added in BPs

	void Debug(float DeltaTime);

///// Fire
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public :

	UPROPERTY() 
	float LastFireTime;

	//

	UFUNCTION() 
	void FireOnce();

	UFUNCTION()
	void SpawnProjectile();

	UFUNCTION(NetMulticast, Unreliable)
	void SpawnFireFXMulticast();

	UFUNCTION()
	bool CanFire();



///// Reload
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

///// Attachments
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	class UStaticMeshComponent* MagMeshCT; // mesh components abuse pas

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BarrelMeshCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ScopeMeshCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* MuzzleSceneCT; // Muzzle FXs will spawn at this location.

	//

	//UFUNCTION()
	//bool IsAttachmentInWhiteList(class ARZ_Attachment* Attachment);
	
};
