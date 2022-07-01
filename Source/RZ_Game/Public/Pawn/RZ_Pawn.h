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

#include "GameFramework/Pawn.h"
#include "RZM_Shared.h"
#include "RZM_InventorySystem.h"
#include "RZM_BuildingSystem.h"
#include "RZM_PowerSystem.h"
#include "RZ_PowerComponentInterface.h"
#include "RZ_Game.h"
#include "RZ_Pawn.generated.h"

class URZ_AttributeComponent;
class UWidgetComponent;

UCLASS()
class RZ_GAME_API ARZ_Pawn : public APawn,
                             public IRZ_ActorInterface,
                             public IRZ_BuildableInterface,
                             public IRZ_PowerComponentInterface,
                             public IRZ_InventoryActorInterface,
                             public IRZ_PawnInterface
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
	FName DataTableRowName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* PawnBehaviorTree;

	//

	URZ_GameInstance* GameInstance;
	URZ_GameSettings* GameSettings;
	ARZ_GameState* GameState;
	URZ_PawnOTMWidget* OTMWidget;
	
	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector PlayerTargetLocation;
	
	/// ActorInterface implementation.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	FORCEINLINE virtual const FName& GetTableRowName() override { return DataTableRowName; }
	FORCEINLINE virtual void SetPlayerTargetLocation(const FVector& NewPlayerTargetLocation) override
	{
		PlayerTargetLocation = NewPlayerTargetLocation;
	};
	virtual void SetWantToUse(bool bNewWantsTouse) override;
	virtual void OnHoverStart() override;
	virtual void OnHoverEnd() override;

	virtual void SetActorMode(ERZ_ActorMode NewActorMode) override;
	
protected:
	
	/// BuildingSystem implementation.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	
	FORCEINLINE virtual ARZ_BuildingManager* GetBuildingManager() const override { return BuildingManager.Get(); }
	FORCEINLINE virtual URZ_BuildingComponent* GetBuildingComponent() const override { return BuildingComp; }
	FORCEINLINE virtual uint8 GetGridSize() override { return ActorSettings.NormalizedWorldSize.X; };

	virtual void OnPickedUp() override { return; };
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

	//

	TWeakObjectPtr<ARZ_BuildingManager> BuildingManager;
	
	UPROPERTY()
	UMaterialInterface* BaseMeshDefaultMaterial;

	/// PowerSystem implementation.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	
	FORCEINLINE virtual URZ_PowerComponent* GetPowerComponent() override { return PowerComp; }
	virtual void OnPowerStatusUpdated(bool bNewIsPowered) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	URZ_PowerComponent* PowerComp;

	/// InventorySystem implementation.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	// InventoryActorInterface

	virtual void OnAttachedToInventory(URZ_InventoryComponent* InventoryCompRef) override;
	virtual void OnInventorySelection(bool bNewIsSelected) override;
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_InventoryComponent* InventoryComp;

	/// CombatSystem implementation.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	
	virtual void SetActiveTarget(AActor* NewActiveTarget) override;
	virtual void SetWantToFire(bool bNewWantToFire) override; // set wants to fire ?
	
	FORCEINLINE URZ_AttributeComponent* GetAttributeComponent() const { return AttributeComp; } // useless
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_CombatComponent* CombatComp;
	
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

	//virtual void OnProjectileCollision(float ProjectileDamage, const FVector& HitLocation,
	                                   //AController* InstigatorController) override;
	
	//UFUNCTION(NetMulticast, Reliable)
	//void OnDeath_Multicast();

	//UFUNCTION()
	//void SetOnHitMaterial(bool bNewIsEnabled);
};
