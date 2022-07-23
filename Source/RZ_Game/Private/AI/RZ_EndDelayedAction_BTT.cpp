/// RemzDNB

#include "AI/RZ_EndDelayedAction_BTT.h"
#include "AI/RZ_PawnAIController.h"
//
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

URZ_EndDelayedAction_BTT::URZ_EndDelayedAction_BTT()
{
	bNotifyTick = false;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;
	NodeName = "EndDelayedActionTask";
}

EBTNodeResult::Type URZ_EndDelayedAction_BTT::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!OwnerComp.GetAIOwner()) { return EBTNodeResult::Failed; }
	
	ARZ_PawnAIController* OwnerAIController = Cast<ARZ_PawnAIController>(OwnerComp.GetAIOwner());
	if (!OwnerAIController) { return EBTNodeResult::Failed; }
	
	UBlackboardComponent* OwnerBlackboardComp = OwnerAIController->GetBlackboardComponent();
	if (!OwnerBlackboardComp) { return EBTNodeResult::Failed; }
	
	if (OwnerAIController->GetActionList().Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	if (OwnerBlackboardComp->GetValueAsName("CurrentActionName") != "None")
	{
		OwnerAIController->ConsumeLatestAction();
	}

	OwnerBlackboardComp->SetValueAsName("CurrentActionName", "None");

	return EBTNodeResult::Succeeded;
}



