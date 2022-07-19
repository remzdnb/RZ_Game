/// RemzDNB
///
///	ARZ_Weapon.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_BaseFramework.h"
#include "RZ_AttributeSystem.h"
#include "RZ_InventorySystem.h"
#include "RZ_Game.h"
//
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "RZ_Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponFiredDelegate, class ARZ_Weapon*, Weapon);

UCLASS()
class RZ_GAME_API ARZ_Weapon : public AActor,
                               public IRZ_ActorInterface,
                               public IRZ_ItemInterface,
                               public IRZ_InventoryOwnerInterface
{
	GENERATED_BODY()

public:

	ARZ_Weapon();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	//

	FWeaponFiredDelegate OnWeaponFired;

protected:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootSceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* RootMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class URZ_TurretComponent* TurretComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_AttributeComponent* CombatComp;

	//

	URZ_GameSettings* RZGameSettings;

	//
	
	ERZ_WeaponState WeaponState;

	/// ActorInterface implementation.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	FORCEINLINE virtual const FVector& GetPlayerTargetLocation() const override { return PlayerTargetLocation; }
	FORCEINLINE virtual void SetPlayerTargetLocation(const FVector& NewPlayerTargetLocation) override
	{
		PlayerTargetLocation = NewPlayerTargetLocation;
	};
	FORCEINLINE virtual void SetWantToUse(bool bNewWantsTouse) override { bWantToUse = bNewWantsTouse; };

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector PlayerTargetLocation;

	UPROPERTY()
	bool bWantToUse;

	UPROPERTY()
	bool bHasReleasedTrigger;
	
	/// InventorySystem implementation.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	
	// ItemInterface

	FORCEINLINE virtual const FName& GetItemName() const override { return ItemName; }
	FORCEINLINE virtual const FRZ_ItemSettings& GetItemSettings() const override { return ItemSettings; }
	virtual void OnAttachedToInventory(URZ_InventoryComponent* InventoryCompRef) override;
	virtual void OnSelectedByInventory(bool bNewIsSelected) override;

	// InventoryOwnerInterface

	virtual URZ_InventoryComponent* GetInventoryComponent() const override { return InventoryComp; }
	virtual USceneComponent* GetAttachComponent() const override { return RootSceneComp; }

	//

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRZ_ItemSettings ItemSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_InventoryComponent* InventoryComp;
	
	/// ??
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsTurretWeapon;
	
	float LastUseTime;

	UFUNCTION()
	void CalcSingleTrace(TArray<FHitResult> HitResults, const FVector& TraceStart, const FVector& TraceEnd);
};

