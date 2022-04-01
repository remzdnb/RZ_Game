#pragma once

#include "GameFramework/Actor.h"
#include "RZ_WorldGrid.generated.h"

class ARZ_WorldTile;
//
class UArrowComponent;

UENUM(BlueprintType)
enum class ERZ_WorldTileType : uint8
{
	Grid,
	Border,
};

UCLASS()
class RZM_WORLD_API ARZ_WorldGrid : public AActor
{
	GENERATED_BODY()
	
public:
	
	ARZ_WorldGrid();
	
	//virtual void OnConstruction(const FTransform& Transform) override;
	//virtual void BeginPlay() override;

	//

	UFUNCTION() TArray<ARZ_WorldTile*> GetGridTiles() const { return GridTiles; }
	FORCEINLINE UInstancedStaticMeshComponent* GetInstancedGroundMeshesComponent() { return InstancedStaticMeshComp; }

private:

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "ARZ_WorldGrid", meta = (AllowPrivateAccess = "true"))
	void RegenerateGrid();

	UFUNCTION()
	void SpawnTile(TSubclassOf<AActor> TileClass, const FTransform& SpawnTransform, int32 PositionX, int32 PositionY,
			   float SizeX, float SizeY, ERZ_WorldTileType TileType);

	//UFUNCTION()
	//FTransform GetInstancedSocket(int32 InstanceIndex);

	//

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootSceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UInstancedStaticMeshComponent* InstancedStaticMeshComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* DirectionArrowComp;

	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_WorldGrid", meta = (AllowPrivateAccess = "true"))
	float TileSize;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_WorldGrid", meta = (AllowPrivateAccess = "true"))
	FIntPoint GridSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_WorldGrid", meta = (AllowPrivateAccess = "true"))
	bool bSpawnBorderTiles;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_WorldGrid", meta = (AllowPrivateAccess = "true"))
	int32 BorderTileLength;
	
	UPROPERTY()
	TArray<ARZ_WorldTile*> GridTiles;
	
	//
	
	//UFUNCTION()
	//void SpawnTile(TSubclassOf<AActor> TileClass, const FTransform& SpawnTransform, int32 PositionX, int32 PositionY,
	               //float SizeX, float SizeY, ERZ_WorldTileType TileType);

	//UFUNCTION()
	//void OnGamePhaseUpdated(ESS_GamePhase NewGamePhase);
	
	//UFUNCTION()
	//void UpdateTilesOwnership();
/*
	///// Pawns
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UFUNCTION()
	ASS_Pawn* SpawnPawn(const FRZ_IntRect& SpawnRect, const FName& PawnDataRowName, ESS_Team Team,
	                    bool bIsDemoPawn = false);

	UFUNCTION()
	bool RequestPawnMovement(ASS_Pawn* PawnToMove, const FIntPoint& MoveVector);

	UFUNCTION()
	void RegisterPawnToGrid(ASS_Pawn* PawnToRegister, const FRZ_IntRect& PawnRect);

	UFUNCTION()
	void UnregisterPawnFromGrid(const ASS_Pawn* PawnToUnregister);

private:

	UPROPERTY()
	TArray<ASS_Pawn*> PawnArray;
	
	///// Helpers
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	public:

	// Get grid tile from its corresponding border tile.
	ARZ_WorldTile* ConvertBorderToGridTile(const ARZ_WorldTile* BorderTile) const;
	
	// Get tile reference from grid position.
	ARZ_WorldTile* GetTileFromPosition(const FIntPoint& Position) const;
	
	// Get all tile references from rectangle coordinates.
	TArray<ARZ_WorldTile*> GetTilesFromRect(const FRZ_IntRect& Rect) const;

	// Check tiles validity and ownership.
	bool IsValidRect(const FRZ_IntRect& Rect, const ASS_Pawn* PawnToIgnore = nullptr) const;
	
	// Get best spawn position for a given team.
	bool GetSpawnRect(FRZ_IntRect& RectResult, const ARZ_WorldTile* FromTile, int32 GroupSize, ESS_Team Team) const;
	
	// Get all pawns registered inside a given rect.
	TArray<ASS_Pawn*> GetAllPawnsFromRect(const FRZ_IntRect& Rect);

	// Get move vector between two tiles. Returns false if invalid.
	bool GetMoveVector(FIntPoint& MoveVectorResult, const ARZ_WorldTile* StartTile, const ARZ_WorldTile* TargetTile) const;
	
	//

	UFUNCTION() // get closest tile in line better
	ARZ_WorldTile* GetClosestTile(const ARZ_WorldTile* TargetTile, TArray<ARZ_WorldTile*> TilesToCheck) const;

	///// Rendering
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void HighlightLine(int32 PositionX, const FLinearColor& HighlightColor);
	void HighlightColumn(int32 PositionY, const FLinearColor& HighlightColor);
	void HighlightRect(const FRZ_IntRect& Rect, const FLinearColor& HighlightColor, bool bSaveChanges);

	UFUNCTION()
	void DisableAllHighlight(bool bResetToDefault = false);

	///// Debug
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UFUNCTION()
	static void DebugPoint(const FIntPoint& IntPoint);

	UFUNCTION()
	static void DebugRect(const FRZ_IntRect& Rect);

	UFUNCTION()
	static void DebugTile(const ARZ_WorldTile* TileToDebug);*/
};
