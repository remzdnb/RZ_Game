#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RZ_PawnBTT_SelectPatrolPoint.generated.h"

UCLASS()
class RZ_GAME_API URZ_PawnBTT_SelectPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
