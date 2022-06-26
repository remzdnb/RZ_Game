#pragma once

#include "CoreMinimal.h"
#include "S2D_Game.generated.h"

class AS2D_GameState;
class US2D_GameSettings;
class URZ_BuildingComponent;
class AS2D_GridManager;
class AS2D_GridTile;

UENUM(BlueprintType)
enum class ES2D_GridTileType : uint8
{
	Grid,
	Border,
};

USTRUCT(BlueprintType)
struct FS2D_GridTileData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FIntPoint Position;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	ES2D_GridTileType Type;
	
	FS2D_GridTileData()
	{
		Type = ES2D_GridTileType::Grid;
	}
};