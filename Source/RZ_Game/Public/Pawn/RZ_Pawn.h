/// RemzDNB
///
///	RZ_Building.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
#include "RZM_Shared.h"
#include "RZM_PowerSystem.h"
//
#include "GameFramework/Pawn.h"
#include "RZ_Pawn.generated.h"

class URZ_PawnCombatComponent;

UCLASS()
class RZ_GAME_API ARZ_Pawn : public APawn,
                             public IRZ_PawnInterface,
                             public IRZ_ItemInterface
{
	GENERATED_BODY()

public:

	ARZ_Pawn();

	virtual void OnConstruction(const FTransform& InTransform) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Pawn interface

	virtual void Init(ERZ_PawnOwnership NewPawnOwnerShip, uint8 NewTeamID);
	virtual UBehaviorTree* GetBehaviorTree() override { return PawnBehaviorTree; };
	virtual void SetActiveTarget(AActor* NewActiveTarget) override;
	virtual void SetWantToFire(bool bNewWantToFire) override;
	
	// Item interface

	FORCEINLINE virtual const FName& GetTableRowName() override { return DataTableRowName; }

	virtual void SetPlayerTargetLocation(const FVector& NewPlayerTargetLocation) override;
	virtual void SetItemMode(ERZ_ItemMode NewItemMode) override;
	virtual void OnInventorySelection(bool bNewIsSelected) override;
	virtual void SetWantToUse(bool bNewWantsTouse) override;
	
	virtual void OnHoverStart() override;
	virtual void OnHoverEnd() override;
	
	//

	//const FRZ_ItemSettings* GetItemSettings() const;

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootSceneComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMeshComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBoxCT; // Preset : ?
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OTMWidgetComp;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class URZ_BuildingOTM_BaseWidget* BuildingOTMWidget;
	
	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//class UStaticMeshComponent* DemoMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DataTableRowName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector PlayerTargetLocation;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ARZ_Character BP Settings", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* PawnBehaviorTree;

	URZ_GameInstance* GameInstance;
	URZ_GameSettings* GameSettings;
	ARZ_GameState* GameState;
	
	bool bIsDemoMode;

	UMaterialInterface* BaseMeshDefaultMaterial;
	
	/// Combat
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	// Projectile Interface

	//virtual void OnProjectileCollision(float ProjectileDamage, const FVector& HitLocation,
	                                   //AController* InstigatorController) override;

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
