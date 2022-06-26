/// RemzDNB
///
///	RZ_PowerManager.h
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GameFramework/Actor.h"
#include "RZM_Shared.h"
#include "RZM_PowerSystem.h"
#include "RZ_ButtonWidget.h"
#include "RZ_PowerComponent.h"
#include "RZ_PowerManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPowerManagerUpdated);

UCLASS()
class RZM_POWERSYSTEM_API ARZ_PowerManager : public AActor
{
	GENERATED_BODY()
	
public:	

	ARZ_PowerManager();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	///
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<URZ_ButtonWidget> PowerComponentTextWidgetClass;
	
	//

	FPowerManagerUpdated OnPowerManagerUpdated;

	//

	const TArray<FRZ_PowerGridInfo>& GetPowerGrids() const { return PowerGrids; }
	TArray<URZ_PowerComponent*> GetPowerComponents() const { return PowerComponents; };
	TArray<URZ_PowerComponent*> GetComponentsFromGrid(int32 GridID);

	//

	UFUNCTION()
	void AddPowerComponent(URZ_PowerComponent* InPowerComponent);

	UFUNCTION()
	void RemovePowerComponent(URZ_PowerComponent* InPowerComponent);
	
	UFUNCTION()
	void ReevaluteGrids();

	// Grids.
	
	UFUNCTION()
	uint8 CreateGrid(URZ_PowerComponent* PowerComponent);

	UFUNCTION()
	void MergeGrids(uint8 FirstGridID, uint8 SecondGridID); // uint8 ?

	UFUNCTION()
	void AddComponentToGrid(uint8 GridID, URZ_PowerComponent* PowerComponent);

	UFUNCTION()
	void RemoveComponentFromGrid(uint8 GridID, URZ_PowerComponent* PowerComponent);
	
	//

	UFUNCTION()
	void UpdateSavedGrids();
	
private:
	
	//

	TArray<URZ_PowerComponent*> PowerComponents;
    TArray<FRZ_PowerGridInfo> PowerGrids;

	//
	
	bool bDebug;

	UFUNCTION()
	void Debug(float DeltaTime);
};
