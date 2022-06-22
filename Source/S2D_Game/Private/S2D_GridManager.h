/// RemzDNB
///
///	S2D_GridManager.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_IntRect.h"
#include "RZM_PowerSystem.h"
#include "S2D_Game.h"
#include "S2D_GameSettings.h"
#include "GameFramework/Actor.h"
#include "S2D_GridManager.generated.h"

class UArrowComponent;

UCLASS()
class AS2D_GridManager : public AActor
{
	GENERATED_BODY()
	
public:	

	AS2D_GridManager();
	
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	//ToDO : CursorPositionToGridSelection

	UFUNCTION()
	void ShowPowerGrid(TArray<URZ_PowerComponent*> PowerComponents);

	UFUNCTION()
	void HidePowerGrid();
	
private:

	UFUNCTION()
	void InitTiles();

	UFUNCTION()
	void EnableTile(FIntPoint Position);

	UFUNCTION()
	void DisableAllTiles();

	UFUNCTION()
	void AddRectSelection(FRZ_IntRect IntRect);

	UFUNCTION()
	void UpdateSelectionBorders();
	

	// Components

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootSceneComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* DirectionArrowComp;

	//
	
	const US2D_GameSettings* S2DGameSettings;

	// Transient

	UPROPERTY()
	TArray<AS2D_GridTile*> DisabledTiles;
	
	UPROPERTY()
	TArray<AS2D_GridTile*> EnabledTiles;










	
	
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