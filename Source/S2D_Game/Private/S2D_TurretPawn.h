/// RemzDNB
///
///	S2D_TurretPawn.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_WeaponSystem.h"
#include "AI/RZ_PawnAIController.h"
#include "AI/RZ_PerceptionComponent.h"
#include "S2D_Pawn.h"
#include "S2D_TurretPawn.generated.h"

class URZ_GameSettings;
class URZ_TurretComponent;
class URZ_PerceptionComponent;

UCLASS()
class AS2D_TurretPawn : public AS2D_Pawn
{
	GENERATED_BODY()
	
public:	

	AS2D_TurretPawn();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Pawn interface

	virtual void SetActiveTarget(AActor* NewActiveTarget) override;
	virtual void SetWantToFire(bool bNewWantToFire) override;

	// Item interface

//	virtual void OnInventorySelection(bool bNewIsSelected) override;
	virtual void OnHoverStart() override;
	virtual void OnHoverEnd() override;
	
	// toggle AI on selected ?
	
	// ?

private:

	UFUNCTION()
	void FireOnce();

	URZ_GameSettings* GameSettings;
	ARZ_PawnAIController* AIController;

	FRZ_ProjectileWeaponSettings ProjectileWeaponSettings;

	//

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* TurretMeshCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_PerceptionComponent* PerceptionCT;

	//

	UMaterialInterface* TurretMeshDefaultMaterial;
	
	bool bWantToFire;
	float LastFireTime;
	float FireRate;
};
