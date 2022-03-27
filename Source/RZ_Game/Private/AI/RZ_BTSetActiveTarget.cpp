/// RemzDNB

#include "AI/RZ_BTSetActiveTarget.h"
#include "AI/RZ_CharacterAIController.h"
//
#include "GameplayTagAssetInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type URZ_BTSetActiveTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARZ_CharacterAIController* CharacterAIController = Cast<ARZ_CharacterAIController>(OwnerComp.GetAIOwner());
	if (CharacterAIController)
	{
		UBlackboardComponent* BlackboardComp = CharacterAIController->GetBlackboardComponent();

		BlackboardComp->SetValueAsObject("ActiveTarget", nullptr);

		uint8 Index = 0;
		for (const auto& DetectedActor : CharacterAIController->GetDetectedActors())
		{
			const IGameplayTagAssetInterface* GameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(DetectedActor);
			if (GameplayTagAssetInterface)
			{
				if (GameplayTagAssetInterface->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("PawnOwnership.Player"))))
				{
					BlackboardComp->SetValueAsObject("ActiveTarget", CharacterAIController->GetDetectedActors()[0]);
					return EBTNodeResult::Succeeded;
				}
			}

			Index++;
		}
		
		/*if (CharacterAIController->GetDetectedActors().IsValidIndex(0))
		{
			BlackboardComp->SetValueAsObject("ActiveTarget", CharacterAIController->GetDetectedActors()[0]);
		}
		else
		{
			BlackboardComp->SetValueAsObject("ActiveTarget", nullptr);
		}

		return EBTNodeResult::Succeeded;*/
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Failed;
}