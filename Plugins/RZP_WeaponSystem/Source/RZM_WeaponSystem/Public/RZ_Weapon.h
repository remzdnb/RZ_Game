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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemUsedDelegate, class ARZ_Weapon*, UsedItem);

UCLASS()
class RZM_WEAPONSYSTEM_API ARZ_Weapon : public AActor,
                                        public IRZ_ItemInterface
{
	GENERATED_BODY()

public:

	ARZ_Weapon();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	//
	
	virtual void SetWantToUse(bool bNewWantsToUse) override;

	UFUNCTION()
	void SetItemState(ERZ_ItemState NewItemState);

	//
	
	FORCEINLINE ERZ_ItemState GetItemState() const { return ItemState; }

	//

	FItemUsedDelegate OnItemUsed;

	UPROPERTY() // replicated ?
	FVector OwnerTargetLocation;

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootSceneComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BaseStaticMeshComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* BaseSkeletalMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DataTableRowName;
	
	//
	
	class URZ_WeaponSystemModuleSettings* ItemActorPluginSettings;
	class IRZ_PawnItemInterface* OwnerPawnInterface;
	
	//

	UPROPERTY()
	ERZ_ItemState ItemState;
	
	UPROPERTY() bool bWantsToUse;
	UPROPERTY() bool bHasReleasedTrigger;
	UPROPERTY() float LastUseTime;
};
