/// RemzDNB
///
///	S2D_BuildComponent.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "S2D_BuildComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class US2D_BuildComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	US2D_BuildComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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

private:

	UFUNCTION()
	void UpdateBuildingPawnLocation(float DeltaTime) const;

	UFUNCTION()
	virtual bool IsValidBuildLocation();

	//

	TWeakObjectPtr<AActor> BuildableActor;

	//

	void Debug(float DeltaTime) const;
};
