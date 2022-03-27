/// RemzDNB
///
///	RZ_Building.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_ItemActor.h"
//
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RZ_Building.generated.h"

class URZ_PawnCombatComponent;

UCLASS()
class RZ_GAME_API ARZ_Building : public APawn,
                                 public IRZ_ProjectileInterface
{
	GENERATED_BODY()

public:

	ARZ_Building();

	virtual void OnConstruction(const FTransform& InTransform) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootSceneComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBoxComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* SkeletalMeshComp;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DataTableRowName;

	/// Combat
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	// Projectile Interface

	virtual void OnProjectileCollision(float ProjectileDamage, const FVector& HitLocation,
	                                   AController* InstigatorController) override;

	//

	FORCEINLINE URZ_PawnCombatComponent* GetPawnCombatComponent() const { return PawnCombatComp; }

private:

	UFUNCTION()
	void OnDestroyed();

	//UFUNCTION(NetMulticast, Reliable)
	//void OnDeath_Multicast();

	//UFUNCTION()
	//void SetOnHitMaterial(bool bNewIsEnabled);

	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_PawnCombatComponent* PawnCombatComp;

	UPROPERTY()
	FTimerHandle OnHitTimerHandle;
};
