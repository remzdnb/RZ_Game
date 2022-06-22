/// RemzDNB
///
///	RZ_PowerManager.h
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_PowerSystem.h"
#include "RZ_PowerComponent.h"
#include "GameFramework/Actor.h"
#include "RZ_PowerManager.generated.h"

UCLASS()
class RZM_POWERSYSTEM_API ARZ_PowerManager : public AActor
{
	GENERATED_BODY()
	
public:	

	ARZ_PowerManager();
	virtual void BeginPlay() override;

	//

	TArray<URZ_PowerComponent*> GetPowerComponents() const { return PowerComponents; };

	TArray<URZ_PowerComponent*> GetComponentsFromGrid(int32 GridID);

	//

	UFUNCTION()
	void AddPowerComponent(URZ_PowerComponent* InPowerComponent);

	UFUNCTION()
	void RemovePowerComponent(URZ_PowerComponent* InPowerComponent);
	
	UFUNCTION()
	void ReevaluteGrids();

	// Grids manipulations.
	
	UFUNCTION()
	void CreateGrid(URZ_PowerComponent* FirstPowerComponent, URZ_PowerComponent* SecondPowerComponent);

	UFUNCTION()
	void AddToGrid(uint8 GridID, URZ_PowerComponent* PowerComponent);

	UFUNCTION()
	void MergeGrids(uint8 FirstGridID, uint8 SecondGridID);
	
private:

	TArray<URZ_PowerComponent*> PowerComponents;
    TArray<FRZ_PowerGridInfo> Grids;

	//

	
};
