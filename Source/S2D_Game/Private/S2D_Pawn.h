/// RemzDNB
///
///	S2D_Pawn.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Pawn/RZ_Pawn.h"
#include "S2D_Game.h"
#include "S2D_Pawn.generated.h"

UCLASS()
class AS2D_Pawn : public ARZ_Pawn
{
	GENERATED_BODY()

public:

	AS2D_Pawn();
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// ItemInterface
	
	virtual void OnHoverStart() override;
	virtual void OnHoverEnd() override;

	//

	FORCEINLINE virtual URZ_BuildingComponent* GetBuildingComponent() const override { return BuildingComp; }
	FORCEINLINE virtual uint8 GetGridSize() override { return ActorSettings.NormalizedWorldSize.X; };

	virtual void OnPickedUp() override { return; };
	virtual void OnBuildStart() override;
	virtual void OnBuildStop() override;
	virtual void OnBuildEnd() override;

	virtual void OnInventorySelection(bool bNewIsSelected) override;

	virtual void OnBuildLocationUpdated(const FVector& NewBuildLocation) override;

	/// GridSystem
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:
	
	AS2D_WorldTileManager* GridManager;
	UMaterialInterface* BaseMeshDefaultMaterial;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	uint8 GridSize;
};
