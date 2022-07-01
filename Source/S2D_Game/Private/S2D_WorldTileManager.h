/// RemzDNB
///
///	S2D_WorldTileManager.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Math/RZ_IntRect.h"
#include "RZM_PowerSystem.h"
#include "S2D_Game.h"
#include "S2D_GameSettings.h"
#include "GameFramework/Actor.h"
#include "S2D_WorldTileManager.generated.h"

class UArrowComponent;

UCLASS()
class AS2D_WorldTileManager : public AActor
{
	GENERATED_BODY()
	
public:	

	AS2D_WorldTileManager();
	
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//ToDO : CursorPositionToGridSelection
	
	//
	
	UFUNCTION()
	void AddSelection(const FVector& Location, FIntPoint Size);

	UFUNCTION()
	void ClearActiveTiles();

	//

	UFUNCTION()
	bool IsActiveTileLocation(const FVector& TargetLocation) const;

	UFUNCTION()
	bool IsValidNewTileLocation(const FVector& TargetLocation) const;
	
	
private:
	
	UFUNCTION()
	void EnableTile(FIntPoint Position);
	
	UFUNCTION()
	void AddRectSelection(FRZ_IntRect IntRect);
	

	// Components

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootSceneComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* DirectionArrowComp;

	//

	ARZ_PowerManager* PowerManagerRef;
	const US2D_GameSettings* S2DGameSettings;

	// Transient

	UPROPERTY()
	TArray<AS2D_WorldTile*> PooledTiles;
	
	UPROPERTY()
	TArray<AS2D_WorldTile*> ActiveTiles;
	
	//

	bool bDebug;
	
	void Debug(float DeltaTime);


/*
	
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
