/// RemzDNB
///
///	RZ_BuildingComponent.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_BaseFramework.h"
#include "RZ_BuildingSystem.h"
//
#include "Components/ActorComponent.h"
#include "RZ_BuildingComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RZ_BUILDINGSYSTEM_API URZ_BuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	URZ_BuildingComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* TickFunction) override;

	UFUNCTION()
	void StartDemoBuild();

	UFUNCTION()
	void StopDemoBuild();
	
	UFUNCTION()
	void StartBuilding();

	UFUNCTION()
	void StopBuilding();
	
	UFUNCTION()
	void EndBuilding();
	
	UFUNCTION()
	void RotateBuildActor(bool bRotateRight) const;
	
	//

	FVector PlayerTargetLocation;

	FORCEINLINE bool GetIsBuilding() const { return bIsBuilding; }


	FORCEINLINE const FRZ_BuildableActorSettings& GetBuildableActorSettings() const { return BuildableActorSettings; }

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRZ_BuildableActorSettings BuildableActorSettings;
	
	IRZ_BuildableActorInterface* BuildableActorInterface;

	FVector LastBuildActorLocation;

	UPROPERTY()
	bool bIsDemoBuilding;
	
	UPROPERTY()
	bool bIsBuilding;

	UPROPERTY()
	float BuildingStartTime;
	
	UPROPERTY()
	FTimerHandle BuildTimer;

	//

	UFUNCTION()
	void UpdateBuildableActorLocation(float DeltaTime);

	UFUNCTION()
	void UpdateBuildableActorCollision();

	UFUNCTION()
	virtual bool IsValidBuildLocation();
	
};
