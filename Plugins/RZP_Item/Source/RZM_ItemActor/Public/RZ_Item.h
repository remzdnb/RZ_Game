/// RemzDNB
///
///	Base class for any usable/attachable item.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_ItemActor.h"
//
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "RZ_Item.generated.h"

UCLASS()
class RZM_ITEMACTOR_API ARZ_Item : public AActor
{
	GENERATED_BODY()

public:

	ARZ_Item();

protected:

	virtual void BeginPlay() override;

	/////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UPROPERTY() // replicated ?
	FVector OwnerTargetLocation;

	//
	
	UFUNCTION()
	void SetWantsToUse(bool bNewWantsToUse);

	UFUNCTION()
	void SetItemState(ERZ_ItemState NewItemState);

	//

	FORCEINLINE UFUNCTION() const FName GetDataRowName() const { return DataTableRowName; }
	FORCEINLINE UFUNCTION() const FRZ_ItemInfo* GetItemData() const { return ItemData; }
	FORCEINLINE UFUNCTION() ERZ_ItemState GetItemState() const { return ItemState; }

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootSceneCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* RootStaticMeshCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* RootSkeletalMeshCT;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DataTableRowName;
	
	//
	
	class URZ_ItemActorModuleSettings* ItemActorPluginSettings;
	const FRZ_ItemInfo* ItemData;
	class IRZ_PawnItemInterface* OwnerPawnInterface;
	
	//

	UPROPERTY()
	ERZ_ItemState ItemState;
	
	UPROPERTY() bool bWantsToUse;
	UPROPERTY() bool bHasReleasedTrigger;
	UPROPERTY() float LastUseTime;
};
