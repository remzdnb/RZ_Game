/// RemzDNB
///
///	RZ_Pawn.h
///	
///	Implements InventoryComponent
///	Implements ItemInterface
///	Implements PowerComponent
///	Implements PowerUserInterface
///	Implements PawnCombatComponent
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once


#include "RZ_AttributeSystem.h"
#include "RZ_InventorySystem.h"
#include "RZ_BuildingSystem.h"
#include "RZ_PowerSystem.h"
#include "RZ_PowerComponentInterface.h"
#include "RZ_Game.h"
#include "GameFramework/Pawn.h"
#include "RZ_Pawn.generated.h"

class URZ_AttributeComponent;
class UWidgetComponent;

UCLASS()
class RZ_GAME_API ARZ_Pawn : public APawn,
                             public IRZ_ActorInterface,
                             public IRZ_PawnInterface,
                             public IRZ_ItemInterface,
                             public IRZ_InventoryOwnerInterface,
                             public IRZ_BuildableActorInterface,
                             public IRZ_PowerComponentInterface
{
	GENERATED_BODY()

public:

	ARZ_Pawn();

	virtual void OnConstruction(const FTransform& InTransform) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	///
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual UBehaviorTree* GetBehaviorTree() override { return PawnBehaviorTree; };
	
protected:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootSceneComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* OTMWidgetComp;
	
	//
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* PawnBehaviorTree;

	//

	URZ_GameInstance* GameInstance;
	URZ_GameSettings* GameSettings;
	ARZ_GameState* GameState;
	URZ_PawnOTMWidget* OTMWidget;
	
	/// ActorInterface implementation.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	FORCEINLINE virtual const FVector& GetPlayerTargetLocation() const override { return PlayerTargetLocation; }
	FORCEINLINE virtual void SetPlayerTargetLocation(const FVector& NewPlayerTargetLocation) override
	{
		PlayerTargetLocation = NewPlayerTargetLocation;
	};
	virtual void SetWantToUse(bool bNewWantsTouse) override;
	virtual void OnHoverStart() override;
	virtual void OnHoverEnd() override;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector PlayerTargetLocation;
	
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
	
	/// BuildingSystem implementation.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	
	FORCEINLINE virtual URZ_BuildingComponent* GetBuildingComponent() const override { return BuildingComp; }

	virtual void OnPickedUp() override { return; };
	virtual void OnDemoBuildStart() override;
	virtual void OnDemoBuildStop() override;
	virtual void OnBuildStart() override;
	virtual void OnBuildStop() override;
	virtual void OnBuildEnd() override;
	virtual void OnValidBuildLocation() override;
	virtual void OnInvalidBuildLocation() override;
	virtual void OnBuildLocationUpdated(const FVector& NewBuildLocation) override { return; };

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_BuildingComponent* BuildingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BuildMeshComp;
	
	UPROPERTY()
	UMaterialInterface* BaseMeshDefaultMaterial;

	/// PowerSystem implementation.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	URZ_PowerComponent* PowerComp;
	
public:
	
	FORCEINLINE virtual URZ_PowerComponent* GetPowerComponent() override { return PowerComp; }

	//
	
	virtual void OnPowerStatusUpdated(bool bNewIsPowered) override;
	
	/// AttributeSystem implementation.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_AttributeComponent* AttributeComp;

private:
	
	UPROPERTY()
	FTimerHandle OnHitTimerHandle;

	//
	
	UFUNCTION()
	void OnDestroyed();
	
	/// Misc
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	
	virtual void SetActiveTarget(AActor* NewActiveTarget) override;
	virtual void SetWantToFire(bool bNewWantToFire) override; // set wants to fire ?
	
	//virtual void OnProjectileCollision(float ProjectileDamage, const FVector& HitLocation,
	                                   //AController* InstigatorController) override;
	
	//UFUNCTION(NetMulticast, Reliable)
	//void OnDeath_Multicast();

	//UFUNCTION()
	//void SetOnHitMaterial(bool bNewIsEnabled);
};
