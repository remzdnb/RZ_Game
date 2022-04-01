/// RemzDNB

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "RZ_PawnBTS_UpdateActiveTarget.generated.h"

UCLASS()
class RZ_GAME_API URZ_PawnBTS_UpdateActiveTarget : public UBTService
{
	GENERATED_BODY()

public:

	URZ_PawnBTS_UpdateActiveTarget();

protected:
	
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
