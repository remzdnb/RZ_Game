/// RemzDNB

#include "AI/RZ_PawnBTT_SelectPatrolPoint.h"
#include "AI/RZ_AIPatrolPoint.h"
#include "AI/RZ_PawnAIController.h"
//
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

EBTNodeResult::Type URZ_PawnBTT_SelectPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARZ_PawnAIController* CharacterAIController = Cast<ARZ_PawnAIController>(OwnerComp.GetAIOwner());
	if (CharacterAIController)
	{
		UBlackboardComponent* BlackboardComp = CharacterAIController->GetBlackboardComponent();

		ARZ_AIPatrolPoint* CurrentPoint = Cast<ARZ_AIPatrolPoint>(
			BlackboardComp->GetValueAsObject("TargetPatrolPoint")
		);

		TArray<AActor*> PatrolPoints = CharacterAIController->GetPatrolPoints();
		PatrolPoints.Remove(CurrentPoint);

		ARZ_AIPatrolPoint* NextPoint = Cast<ARZ_AIPatrolPoint>(
			PatrolPoints[UKismetMathLibrary::RandomIntegerInRange(0, PatrolPoints.Num() - 1)]
		);

		BlackboardComp->SetValueAsObject("TargetPatrolPoint", NextPoint);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
