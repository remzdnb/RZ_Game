/// RemzDNB

#pragma once

#include "RZ_Game.h"
#include "BehaviorTree/BTService.h"
#include "RZ_UpdateTargetViewLocation_BTS.generated.h"

UCLASS()
class RZ_GAME_API URZ_UpdateTargetViewLocation_BTS : public UBTService
{
	GENERATED_BODY()

public:

	URZ_UpdateTargetViewLocation_BTS();

protected:
	
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

private:

	//
};
