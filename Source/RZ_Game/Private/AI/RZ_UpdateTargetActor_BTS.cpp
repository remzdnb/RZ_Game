/// RemzDNB

#include "AI/RZ_UpdateTargetActor_BTS.h"
#include "AI/RZ_PawnAIController.h"
#include "RZ_PerceptionComponent.h"
//
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

URZ_UpdateTargetActor_BTS::URZ_UpdateTargetActor_BTS()
{
}

void URZ_UpdateTargetActor_BTS::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void URZ_UpdateTargetActor_BTS::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void URZ_UpdateTargetActor_BTS::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (!OwnerComp.GetAIOwner()) { return; }
	if (!OwnerComp.GetAIOwner()->GetPawn()) { return; }
	
	URZ_PerceptionComponent* PerceptionComponent = Cast<URZ_PerceptionComponent>(
		OwnerComp.GetAIOwner()->GetPawn()->GetComponentByClass(URZ_PerceptionComponent::StaticClass())
	);
	if (!PerceptionComponent) { return; }
	
	PerceptionComponent->UpdateSensedActors();
	
	TArray<AActor*> EnemyActors;
	for (const auto& SensedActor : PerceptionComponent->GetSensedActors())
	{
		EnemyActors.Add(SensedActor.Get());
	}
	
	AActor* ActiveTarget = RZ_CommonLibrary::GetClosestActorFromLocation(EnemyActors, OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation());

	//

	IRZ_AIOwnerInterface* AIOwnerInterface = Cast<IRZ_AIOwnerInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (AIOwnerInterface)
	{
		AIOwnerInterface->SetTargetActor(ActiveTarget);
	}

	//

	UBlackboardComponent* BlackboardComp = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	if (!BlackboardComp) { return; }
	
	BlackboardComp->SetValueAsObject(TARGETACTOR_BBKEY, ActiveTarget);

	if (ActiveTarget)
	{
		float TargetYaw = UKismetMathLibrary::FindLookAtRotation(
			OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation(),
			ActiveTarget->GetActorLocation()
		).Yaw;

		if (FMath::Abs(OwnerComp.GetAIOwner()->GetPawn()->GetActorRotation().Yaw - TargetYaw) <= 25.0f)
		{
			BlackboardComp->SetValueAsBool(HASATTACKANGLE_BBKEY, true);
		}
		else
		{
			BlackboardComp->SetValueAsBool(HASATTACKANGLE_BBKEY, false);
		}
	}

	/*const AActor* ActiveTargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("ActiveTargetActor"));
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
	}*/
}
