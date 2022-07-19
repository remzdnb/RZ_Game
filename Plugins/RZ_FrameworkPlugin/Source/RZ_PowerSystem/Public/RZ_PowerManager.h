/// RemzDNB
///
///	RZ_PowerManager.h
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GameFramework/Actor.h"
#include "RZ_PowerSystem.h"
#include "RZ_PowerComponent.h"
#include "RZ_PowerManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewGridSelected, int32, NewGridID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewComponentSelected, URZ_PowerComponent*, NewSelectedComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPowerManagerUpdated);

UCLASS()
class RZ_POWERSYSTEM_API ARZ_PowerManager : public AActor
{
	GENERATED_BODY()
	
public:	

	ARZ_PowerManager();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	///
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//TSubclassOf<URZ_ButtonWidget> PowerComponentTextWidgetClass;
	
	//

	FNewGridSelected OnNewGridSelected;
	FNewComponentSelected OnNewComponentSelected;
	FPowerManagerUpdated OnPowerManagerUpdated;

	//

	FORCEINLINE int32 GetSelectedGridID() const { return SelectedGridID; }
	FORCEINLINE URZ_PowerComponent* GetSelectedPowerComponent() const { return SelectedComponent; }
	FORCEINLINE const TArray<FRZ_PowerGridInfo>& GetPowerGrids() const { return PowerGrids; }
	FORCEINLINE const TArray<URZ_PowerComponent*>& GetPowerComponents() const { return PowerComponents; };
	FORCEINLINE TArray<URZ_PowerComponent*> GetComponentsFromGrid(int32 GridID);
	
	//
	
	UFUNCTION()
	void EvaluateGrids();

	UFUNCTION()
	void UpdateGridPowerDistribution(FRZ_PowerGridInfo& GridInfo);
	
	//
	
	UFUNCTION()
	int32 CreateGrid(URZ_PowerComponent* PowerComponent);

	UFUNCTION()
	void MergeGrids(int32 FirstGridID, int32 SecondGridID);

	UFUNCTION()
	void AddComponentToGrid(int32 GridID, URZ_PowerComponent* PowerComponent);

	UFUNCTION()
	void RemoveComponentFromGrid(int32 GridID, URZ_PowerComponent* PowerComponent);
	
	//

	UFUNCTION()
	void AddPowerComponentRef(URZ_PowerComponent* InPowerComponent);

	UFUNCTION()
	void RemovePowerComponentRef(URZ_PowerComponent* InPowerComponent);

	//

	UFUNCTION()
	void SelectGridID(int32 NewSelectedGridID);

	UFUNCTION()
	void SelectPowerComponent(URZ_PowerComponent* NewSelectedComponent);
	
private:
	
	TArray<URZ_PowerComponent*> PowerComponents;
	TArray<FRZ_PowerGridInfo> PowerGrids;


	int32 SelectedGridID;
	URZ_PowerComponent* SelectedComponent;

	//
	
	bool bDebug;

	UFUNCTION()
	void Debug(float DeltaTime);
};
