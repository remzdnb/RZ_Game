/// RemzDNB
///
///	RZ_BuildingComponent.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Components/ActorComponent.h"
#include "RZM_Shared.h"
#include "RZM_BuildingSystem.h"
#include "RZ_BuildingComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RZM_BUILDINGSYSTEM_API URZ_BuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	URZ_BuildingComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* TickFunction) override;

	UFUNCTION()
	void StartBuilding(AActor* NewBuildableActor);

	UFUNCTION()
	void StopBuilding();
	
	UFUNCTION()
	void EndBuilding();
	
	UFUNCTION()
	void RotateBuildActor(bool bRotateRight) const;
	
	//

	FVector PlayerTargetLocation;

	FORCEINLINE bool GetIsBuilding() const { return bIsBuilding; }

private:

	UFUNCTION()
	void UpdateBuildableActorLocation(float DeltaTime);

	UFUNCTION()
	void UpdateBuildableActorCollision();

	UFUNCTION()
	virtual bool IsValidBuildLocation();

	//

	TWeakObjectPtr<AActor> BuildableActor;

	FVector LastBuildActorLocation;
	bool bIsBuilding;
	FVector BuildSize;

	//

	void Debug(float DeltaTime) const;

	bool bDebug;
};
