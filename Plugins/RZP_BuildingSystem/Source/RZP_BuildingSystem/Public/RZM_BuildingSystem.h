#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "RZM_BuildingSystem.generated.h"

class ARZ_BuildingManager;
class URZ_BuildingComponent;

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FRZM_BuildingSystemModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UINTERFACE(MinimalAPI)
class URZ_BuildableInterface : public UInterface
{
	GENERATED_BODY()
};

class RZM_BUILDINGSYSTEM_API IRZ_BuildableInterface
{
	GENERATED_BODY()

public:

	virtual ARZ_BuildingManager* GetBuildingManager() const = 0; // ?
	virtual URZ_BuildingComponent* GetBuildingComponent() const = 0;
	virtual uint8 GetGridSize() = 0; // ?

	virtual void OnPickedUp() = 0;
	virtual void OnBuildStart() = 0;
	virtual void OnBuildStop() = 0;
	virtual void OnBuildEnd() = 0;
	virtual void OnBuildLocationUpdated(const FVector& NewBuildLocation) = 0;
	virtual void OnValidBuildLocation() = 0;
	virtual void OnInvalidBuildLocation() = 0;

private:

	FIntPoint GridPosition;
};