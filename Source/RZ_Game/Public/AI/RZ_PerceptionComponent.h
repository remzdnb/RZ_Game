/// RemzDNB
///
///	Must be implemented in a pawn.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
#include "Components/ActorComponent.h"
#include "RZ_PerceptionComponent.generated.h"

class URZ_GameSettings;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RZ_GAME_API URZ_PerceptionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	URZ_PerceptionComponent();
	
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

	ARZ_GameState* GameState;
	TWeakObjectPtr<URZ_GameSettings> GameSettings;
	TArray<TWeakObjectPtr<AActor>> SensedActors;

	//
	
	float VisionDistance;
	float VisionAngle;
};
