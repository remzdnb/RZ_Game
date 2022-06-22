/// RemzDNB
///
///	S2D_Pawn.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Pawn/RZ_Pawn.h"
#include "S2D_Game.h"
#include "S2D_BuildableInterface.h"
#include "S2D_Pawn.generated.h"

UCLASS()
class AS2D_Pawn : public ARZ_Pawn,
                  public IS2D_BuildableInterface
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
	
	// BuildableInterface

	virtual void OnBuildStart() override;
	virtual void OnBuildStop() override;
	virtual void OnBuildEnd() override;
	virtual uint8 GetGridSize() override { return GridSize; };

private:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BuildSquareMeshComp;

	/// GridSystem
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

private:
	
	AS2D_GridManager* GridManager;
	UMaterialInterface* BaseMeshDefaultMaterial;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	uint8 GridSize;

	/// PowerSystem
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	FORCEINLINE URZ_PowerComponent* GetPowerComponent() const { return PowerComp; }

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	URZ_PowerComponent* PowerComp;
};
