/// RemzDNB
///
///	ARZ_Weapon.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_BaseWidgets.h"
#include "RZ_AttributeSystem.h"
#include "RZ_InventorySystem.h"
#include "RZ_Game.h"
//
#include "RZ_AddHealth_GE.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "RZ_Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponFiredDelegate, class ARZ_Weapon*, Weapon);

UCLASS()
class RZ_GAME_API ARZ_Weapon : public AActor,
                               public IRZ_WorldInteractionInterface,
                               public IRZ_InventoryItemInterface,
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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootSceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* RootMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class URZ_TurretComponent* TurretComp;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RZ", meta = (AllowPrivateAccess = "true"))
	FRZ_WeaponSettings WeaponSettings;

	//
	
	URZ_GameSettings* RZGameSettings;

	UPROPERTY()
	
	//
	
	ERZ_WeaponState WeaponState;
		
	float LastUseTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsTurretWeapon;
	

	/// ActorInterface implementation.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
protected:

	UPROPERTY()
	bool bOwnerWantToUse;

	UPROPERTY()
	bool bHasReleasedTrigger;
	
	/// InventorySystem implementation.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	
	// ItemInterface

	FORCEINLINE virtual const FName& GetItemName() const override { return ItemName; }
	FORCEINLINE virtual const FRZ_ItemSettings& GetItemSettings() const override { return ItemSettings; }
	virtual void OnAttachedToInventory() override;
	virtual void OnEquippedByInventory(bool bNewIsSelected) override;
	FORCEINLINE virtual void OnWantToBeUsedByInventory(bool bNewWantTouse) override { bOwnerWantToUse = bNewWantTouse; }

	// InventoryOwnerInterface

	virtual URZ_InventoryComponent* GetInventoryComponent() const override { return InventoryComp; }
	virtual USceneComponent* GetAttachComponent() const override { return RootSceneComp; }

	//

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RZ", meta = (AllowPrivateAccess = "true"))
	FName ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRZ_ItemSettings ItemSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_InventoryComponent* InventoryComp;
};

