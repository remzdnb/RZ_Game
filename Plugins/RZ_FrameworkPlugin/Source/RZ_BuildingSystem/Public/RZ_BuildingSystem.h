#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RZ_BuildingSystem.generated.h"

class URZ_BuildingComponent;

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FRZ_BuildingSystemModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

/// BuildingSystem.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class RZ_BUILDINGSYSTEM_API URZ_BuildingSystemSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	
	URZ_BuildingSystemSettings();

	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterialInterface* BuildingMaterial_Valid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMaterialInterface* BuildingMaterial_Invalid;
};

UINTERFACE(MinimalAPI)
class URZ_BuildingSystemInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_BUILDINGSYSTEM_API IRZ_BuildingSystemInterface
{
	GENERATED_BODY()

	// Used to load editor data from a single DataAsset reference. Must be implemented in projects GameInstance.

public:
	
	virtual const URZ_BuildingSystemSettings* GetBuildingSystemSettings() const = 0;
};

/// BuildableActor.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USTRUCT()
struct RZ_BUILDINGSYSTEM_API FRZ_BuildableActorSettings : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 NormalizedBuildSize;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BuildTime;
	
	FRZ_BuildableActorSettings()
	{
		NormalizedBuildSize = 1;
		BuildTime = 5.0f;	
	}
};

UINTERFACE(MinimalAPI)
class URZ_BuildableActorInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_BUILDINGSYSTEM_API IRZ_BuildableActorInterface
{
	GENERATED_BODY()

public:
	
	virtual URZ_BuildingComponent* GetBuildingComponent() const = 0;

	virtual void OnPickedUp() = 0;
	virtual void OnDemoBuildStart() = 0;
	virtual void OnDemoBuildStop() = 0;
	virtual void OnBuildStart() = 0;
	virtual void OnBuildStop() = 0;
	virtual void OnBuildEnd() = 0;
	virtual void OnBuildLocationUpdated(const FVector& NewBuildLocation) = 0;
	virtual void OnValidBuildLocation() = 0;
	virtual void OnInvalidBuildLocation() = 0;
};