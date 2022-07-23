/// RemzDNB
///
/// RZ_WorldTileSystem.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

#pragma once

#include "CoreMinimal.h"
#include "RZ_WorldTileSystem.generated.h"

#define WORLDTILESYSTEM_GRIDSIZE 100.0f;

class ARZ_WorldTileManager;
class ARZ_WorldTile;

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FRZ_WorldTileSystemModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
enum class ERZ_WorldTileType : uint8
{
	Grid,
	Border,
};

USTRUCT(BlueprintType)
struct FRZ_WorldTileData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FIntPoint Position;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	ERZ_WorldTileType Type;
	
	FRZ_WorldTileData()
	{
		Type = ERZ_WorldTileType::Grid;
	}
};

/// Module settings.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class RZ_WORLDTILESYSTEM_API URZ_WorldTileSystemSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	
	URZ_WorldTileSystemSettings() {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid")
	TSubclassOf<ARZ_WorldTile> GridTileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid")
	int32 MaxGridTiles;
	
};

UINTERFACE(MinimalAPI)
class URZ_WorldTileSystemSettingsInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_WORLDTILESYSTEM_API IRZ_WorldTileSystemSettingsInterface
{
	GENERATED_BODY()

	// Implemented in GameInstance.

public:
	
	virtual const URZ_WorldTileSystemSettings* GetTileSystemSettings() const = 0;
};
