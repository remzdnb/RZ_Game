// RemzDNB

#pragma once

#include "Components/ActorComponent.h"
#include "RZ_TurretComponent.generated.h"

class URZ_SensingComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetLockedDelegate, bool, bIsTargetLocked);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RZ_GAME_API URZ_TurretComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:

	URZ_TurretComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc) override;

	//

	FTargetLockedDelegate OnTargetLocked;

	UFUNCTION()
	void Init(USceneComponent* ComponentToRotate);

	UFUNCTION()
	void ToggleTurretAI(bool bNewIsEnabled);

private:

	UPROPERTY()
	URZ_SensingComponent* PerceptionComp;

	UPROPERTY()
	USceneComponent* CompToRotate;

	//

	FTimerHandle PerceptionUpdateTimer;

	UPROPERTY()
	float TimerDelay;

	UPROPERTY()
	bool bIsAIEnabled;

	//

	UFUNCTION()
	void UpdateSensedActors();
};
