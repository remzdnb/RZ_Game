/// RemzDNB
///
/// S2D_GameSettings.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "S2D_Game.h"
#include "Engine/DataAsset.h"
#include "S2D_GameSettings.generated.h"

UCLASS()
class US2D_GameSettings : public UDataAsset
{
	GENERATED_BODY()

public:

	US2D_GameSettings();
	
	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid")
	TSubclassOf<AS2D_WorldTile> GridTileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid")
	int32 MaxGridTiles;
	
	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
	UMaterialInterface* ItemSpawnMaterial_Valid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
	UMaterialInterface* ItemSpawnMaterial_Invalid;

};
