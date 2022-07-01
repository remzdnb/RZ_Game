/// RemzDNB
///
///	Must be implemented in a pawn.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_Shared.h"
#include "Components/ActorComponent.h"
#include "RZ_SensingComponent.generated.h"

class URZ_GameSettings;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RZM_SHARED_API URZ_SensingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	URZ_SensingComponent();
	
	virtual void BeginPlay() override;

	//

	UFUNCTION()
	void UpdateSensedActors();

	//

	FORCEINLINE TArray<TWeakObjectPtr<AActor>> GetSensedActors() const { return SensedActors; }

private:



	UFUNCTION()
	TArray<AActor*> GetActorsInRange() const;
	
	UFUNCTION()
	void Debug();
	
	//

	//ARZ_GameState* GameState;
	//TWeakObjectPtr<URZ_GameSettings> GameSettings;
	TArray<TWeakObjectPtr<AActor>> SensedActors;

	//
	
	float VisionDistance;
	float VisionAngle;
};
