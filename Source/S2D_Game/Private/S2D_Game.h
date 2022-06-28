#pragma once

#include "CoreMinimal.h"
#include "S2D_Game.generated.h"

class AS2D_GameState;
class US2D_GameSettings;
class URZ_BuildingComponent;
class AS2D_WorldTileManager;
class AS2D_WorldTile;

UENUM(BlueprintType)
enum class ES2D_WorldTileType : uint8
{
	Grid,
	Border,
};

USTRUCT(BlueprintType)
struct FS2D_WorldTileData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FIntPoint Position;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	ES2D_WorldTileType Type;
	
	FS2D_WorldTileData()
	{
		Type = ES2D_WorldTileType::Grid;
	}
};