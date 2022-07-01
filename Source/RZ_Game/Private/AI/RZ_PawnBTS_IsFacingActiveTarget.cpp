/// RemzDNB

#include "AI/RZ_PawnBTS_IsFacingActiveTarget.h"
#include "AI/RZ_PawnAIController.h"
#include "RZ_SensingComponent.h"
#include "RZ_Game.h"
//
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

URZ_PawnBTS_IsFacingActiveTarget::URZ_PawnBTS_IsFacingActiveTarget()
{
}

void URZ_PawnBTS_IsFacingActiveTarget::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (!OwnerComp.GetAIOwner()) { return; }
	if (!OwnerComp.GetAIOwner()->GetPawn()) { return; }
}

void URZ_PawnBTS_IsFacingActiveTarget::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void URZ_PawnBTS_IsFacingActiveTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (!OwnerComp.GetAIOwner()) { return; }
	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!OwnerComp.GetAIOwner()->GetPawn()) { return; }
	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	if (!BlackboardComp) { return; }

	const AActor* ActiveTargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("ActiveTargetActor"));
	if (!ActiveTargetActor)
	{
		BlackboardComp->SetValueAsBool("IsFacingActiveTarget", false);
		return;
	}

	float TargetYaw = UKismetMathLibrary::FindLookAtRotation(
		OwnerPawn->GetActorLocation(),
		ActiveTargetActor->GetActorLocation()
	).Yaw;
	
	if (FMath::Abs(OwnerPawn->GetActorRotation().Yaw - TargetYaw) <= 25.0f)
	{
		BlackboardComp->SetValueAsBool("IsFacingActiveTarget", true);
	}
	else
	{
		BlackboardComp->SetValueAsBool("IsFacingActiveTarget", false);
	}
}
