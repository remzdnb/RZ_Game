/// RemzDNB
///
///	Base PlayerController class.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_Shared.h"
#include "RZM_WeaponSystem.h"
//
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "RZ_Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponFiredDelegate, class ARZ_Weapon*, Weapon);

UCLASS()
class RZM_WEAPONSYSTEM_API ARZ_Weapon : public AActor,
                                        public IRZ_ActorInterface
{
	GENERATED_BODY()

public:

	ARZ_Weapon();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	// Item interface

	virtual void SetPlayerTargetLocation(const FVector& NewPlayerTargetLocation) override;
	//virtual void OnInventorySelection(bool bNewIsSelected) override;
	virtual const FName& GetTableRowName() override;
	virtual void SetWantToUse(bool bNewWantToUse, ERZ_UseType UseType) override;

	//

	UFUNCTION()
	void SetItemState(ERZ_WeaponState NewItemState);

	//
	
	FORCEINLINE ERZ_WeaponState GetItemState() const { return ItemState; }

	//

	FWeaponFiredDelegate OnWeaponFired;

	UPROPERTY()
	FVector PlayerTargetLocation;

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootSceneCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* RootSkeletalMeshCT;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DataTableRowName;
	
	//
	
	const URZ_WeaponSystemModuleSettings* WeaponSystemModuleSettings;

	bool bWantToUse;
	bool bHasReleasedTrigger;
	float LastUseTime;
	
	//

	UPROPERTY()
	ERZ_WeaponState ItemState;

	// Utility

	UFUNCTION()
	void CalcSingleTrace(TArray<FHitResult> HitResults, const FVector& TraceStart, const FVector& TraceEnd);
};
