/// RemzDNB
///
///	RZ_Building.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
#include "RZM_Shared.h"
//
#include "GameFramework/Pawn.h"
#include "RZ_Building.generated.h"

class URZ_PawnCombatComponent;

UCLASS()
class RZ_GAME_API ARZ_Building : public APawn,
                                 public IRZ_PawnInterface,
                                 public IRZ_ItemInterface
                                
                                 //public IRZ_ProjectileInterface
{
	GENERATED_BODY()

public:

	ARZ_Building();

	virtual void OnConstruction(const FTransform& InTransform) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	// Pawn interface

	virtual void Init(ERZ_PawnOwnership NewPawnOwnerShip, uint8 NewTeamID);
	virtual class UBehaviorTree* GetBehaviorTree();
	virtual void SetActiveTarget(AActor* NewActiveTarget) override;
	virtual void SetWantToFire(bool bNewWantToFire) override;
	
	// Item interface

	virtual void SetControllerTargetLocation(const FVector& NewPlayerTargetLocation) override;
	
	virtual const FName& GetTableRowName() override;
	virtual void OnSelectionUpdated(bool bNewIsSelected) override;
	virtual void EnableBuildMode(bool bNewIsEnabled) override;
	virtual void UpdateBuildModeLocation(const FVector& SpawnLocation, const FVector& LerpedItemLocation) override;
	virtual void SetBuildMeshVisibility(bool bNewIsVisible) override;
	virtual bool IsValidBuildLocation() override;
	virtual void SetWantToUse(bool bNewWantsTouse) override;
	virtual void OnHoverStart() override;
	virtual void OnHoverEnd() override;
	
	//

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootSceneCT; // Root

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* GridMaterialMeshCT; // Ground square demo material is applied to this mesh.

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BaseMeshCT; //

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBoxCT; // Preset : ?
	
	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//class UStaticMeshComponent* DemoMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DataTableRowName;

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ARZ_Character BP Settings", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* PawnBehaviorTree;

	ARZ_GameState* GameState;
	
	bool bIsDemoMode;

	UMaterialInterface* BaseMeshDefaultMaterial;

	URZ_GameSettings* GameSettings;
	
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
