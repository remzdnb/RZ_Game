/// RemzDNB
///
///	RZ_PawnBTT_SetWantToFire.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RZ_PawnBTT_SetWantToFire.generated.h"

UCLASS()
class RZ_GAME_API URZ_PawnBTT_SetWantToFire : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	uint32 bWantToFire : 1;
};
