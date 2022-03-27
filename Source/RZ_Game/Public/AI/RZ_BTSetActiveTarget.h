/// RemzDNB
///
///	RZ_BTSetActiveTarget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RZ_BTSetActiveTarget.generated.h"

UCLASS()
class RZ_GAME_API URZ_BTSetActiveTarget : public UBTTaskNode
{
	GENERATED_BODY()


	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
