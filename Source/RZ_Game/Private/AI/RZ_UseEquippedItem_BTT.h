/// RemzDNB
///
///	RZ_UseEquippedItem_BTT.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RZ_UseEquippedItem_BTT.generated.h"

UCLASS()
class RZ_GAME_API URZ_UseEquippedItem_BTT : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	uint32 bWantToUse : 1;
};
