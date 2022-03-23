/// RemzDNB
///
///
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RZ_CharacterAIController.generated.h"

class ARZ_GameMode;
class ARZ_Character;
//

UCLASS()
class RZ_GAME_API ARZ_CharacterAIController : public AAIController
{
	GENERATED_BODY()

public:

	ARZ_CharacterAIController();
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	//

	FORCEINLINE TArray<AActor*> GetPatrolPoints() const { return PatrolPoints; }

private:

	ARZ_GameMode* GameMode;

	TArray<AActor*> PatrolPoints;
	
	TWeakObjectPtr<ARZ_Character> PossessedCharacter;
	class UBehaviorTreeComponent* BehaviorTreeComp;
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY()
	FName TargetPatrolPointKey;
};
