/// RemzDNB
///
///	RZ_StartDelayedAction_BTT.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RZ_StartDelayedAction_BTT.generated.h"

UCLASS()
class RZ_GAME_API URZ_StartDelayedAction_BTT : public UBTTaskNode
{
	GENERATED_BODY()

public:

	URZ_StartDelayedAction_BTT();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	//virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
