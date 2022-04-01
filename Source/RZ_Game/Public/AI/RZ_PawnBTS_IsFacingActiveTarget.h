/// RemzDNB

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RZ_PawnBTS_IsFacingActiveTarget.generated.h"

UCLASS()
class RZ_GAME_API URZ_PawnBTS_IsFacingActiveTarget : public UBTService
{
	GENERATED_BODY()

public:

	URZ_PawnBTS_IsFacingActiveTarget();

protected:
	
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

private:

	//
};
