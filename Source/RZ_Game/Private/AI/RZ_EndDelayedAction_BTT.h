/// RemzDNB
///
///	RZ_EndDelayedAction_BTT.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RZ_EndDelayedAction_BTT.generated.h"

UCLASS()
class RZ_GAME_API URZ_EndDelayedAction_BTT : public UBTTaskNode
{
	GENERATED_BODY()

public:

	URZ_EndDelayedAction_BTT();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
