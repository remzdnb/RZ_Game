// RemzDNB

#pragma once

#include "RZ_IntRect.h"
#include "RZ_WorldTileSystem.h"
#include "GameFramework/Actor.h"
#include "RZ_WorldTile.generated.h"

class UBoxComponent;

UCLASS()
class ARZ_WorldTile : public AActor
{
	GENERATED_BODY()
	
public:	

	ARZ_WorldTile();
	
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	//

	UFUNCTION()
	void Enable(const FRZ_WorldTileData& NewTileData);

	UFUNCTION()
	void Disable();

	UFUNCTION()
	void UpdateBorders();
	

	//UFUNCTION() class UBoxComponent* GetCollisionBox() const { return CollisionCT; }
	//UFUNCTION() const FRZ_WorldTileData& GetTileData() const { return TileData; }

	//UFUNCTION() void ClearBaseSelection();
	//UFUNCTION() void ClearBorderSelection();
	//UFUNCTION() void ClearAllSelection();

private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootSceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SouthBorderMeshComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* NorthBorderMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WestBorderMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* EastBorderMeshComp;

	//
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRZ_WorldTileData TileData;

	//

	ARZ_WorldTileManager* GridManagerRef;
	
	/*

	UPROPERTY()
	ESS_Team OwningTeam;
	
	///// Pawn
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UFUNCTION(BlueprintNativeEvent)
	void RegisterPawnBPI(ASS_Pawn* PawnToRegister);

	UFUNCTION(BlueprintNativeEvent)
	void UnregisterPawnBPI();

	//

	FORCEINLINE UFUNCTION() TWeakObjectPtr<ASS_Pawn> GetRegisteredPawn() const { return RegisteredPawn; }
	
private:

	TWeakObjectPtr<ASS_Pawn> RegisteredPawn;

	///// Rendering
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UFUNCTION()
	void EnableHighlight(const FLinearColor& Color);
	
	UFUNCTION()
	void EnableDirectionalHighlight(ESS_Direction Direction, const FLinearColor& Color, bool bSaveColor = false);
	
	UFUNCTION()
	void DisableHighlight(bool bResetToDefault = false);

private:

	UPROPERTY()
	UMaterialInstanceDynamic* BaseDMaterial;
	
	FLinearColor DefaultSouthBorderColor;
	FLinearColor DefaultNorthBorderColor;
	FLinearColor DefaultWestBorderColor;
	FLinearColor DefaultEastBorderColor;
	FLinearColor SavedSouthBorderColor;
	FLinearColor SavedNorthBorderColor;
	FLinearColor SavedWestBorderColor;
	FLinearColor SavedEastBorderColor;*/

};
