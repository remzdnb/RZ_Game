/// RemzDNB

#include "AI/RZ_StartDelayedAction_BTT.h"
#include "AI/RZ_PawnAIController.h"
//
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

URZ_StartDelayedAction_BTT::URZ_StartDelayedAction_BTT()
{
	bNotifyTick = false;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;
	NodeName = "StartDelayedActionTask";
}

EBTNodeResult::Type URZ_StartDelayedAction_BTT::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!OwnerComp.GetAIOwner()) { return EBTNodeResult::Failed; }
	
	ARZ_PawnAIController* OwnerAIController = Cast<ARZ_PawnAIController>(OwnerComp.GetAIOwner());
	if (!OwnerAIController) { return EBTNodeResult::Failed; }
	
	UBlackboardComponent* OwnerBlackboardComp = OwnerAIController->GetBlackboardComponent();
	if (!OwnerBlackboardComp) { return EBTNodeResult::Failed; }
	
	if (OwnerAIController->GetActionList().Num() == 0)
	{
		return EBTNodeResult::Succeeded;
	}
	
	OwnerBlackboardComp->SetValueAsName("CurrentActionName", OwnerAIController->GetActionList()[0].ActionName);
	OwnerBlackboardComp->SetValueAsVector("MoveTargetLocation", OwnerAIController->GetActionList()[0].TargetLocation);

	//if (OwnerAIController->GetActionList()[0].Type == ERZ_AIActionType::Move)

	// delay .. until action completed

	return EBTNodeResult::Succeeded;
}
/*
void URZ_StartDelayedAction_BTT::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	OwnerAIController = Cast<ARZ_PawnAIController>(OwnerComp.GetAIOwner());
	if (!OwnerAIController) { return; }
	
	OwnerBlackboardComp = OwnerAIController->GetBlackboardComponent();
	if (!OwnerBlackboardComp) { return; }

	UE_LOG(LogTemp, Error, TEXT("URZ_StartDelayedAction_BTT::TickTask BASE"));
	
	if (!OwnerAIController) { return; }
	if (!OwnerBlackboardComp) { return; }

	if (OwnerBlackboardComp->GetValueAsName("CurrentActionName") == "Move")
	{
		if (FVector::Dist(OwnerAIController->GetCharacter()->GetActorLocation(),
		                  OwnerAIController->GetActionList()[0].TargetLocation) <= 100.0f)
		{
			AbortTask(OwnerComp, NodeMemory);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("URZ_StartDelayedAction_BTT::TickTask MOVE"));
			OwnerAIController->MoveToLocation(OwnerAIController->GetActionList()[0].TargetLocation);
		}
	}
	else
	{
		AbortTask(OwnerComp, NodeMemory);
	}
}

EBTNodeResult::Type URZ_StartDelayedAction_BTT::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);
	
	OwnerAIController = Cast<ARZ_PawnAIController>(OwnerComp.GetAIOwner());
	if (!OwnerAIController) { return EBTNodeResult::Failed; }
	
	OwnerBlackboardComp = OwnerAIController->GetBlackboardComponent();
	if (!OwnerBlackboardComp) { return EBTNodeResult::Failed; }

	if (OwnerBlackboardComp->GetValueAsName("CurrentActionName") != "None")
	{
		OwnerAIController->ConsumeLatestAction();
	}
	
	OwnerBlackboardComp->SetValueAsName("CurrentActionName", "None");
	
	return EBTNodeResult::Succeeded;
}
*/


