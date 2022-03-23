/// RemzDNB

#include "AI/RZ_CharacterAIController.h"
#include "Character/RZ_Character.h"
#include "Game/RZ_GameMode.h"
//
#include "AI/RZ_AIPatrolPoint.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

ARZ_CharacterAIController::ARZ_CharacterAIController()
{
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComp");
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComp");
	
	TargetPatrolPointKey = "TargetPatrolPoint";
}

void ARZ_CharacterAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GameMode = Cast<ARZ_GameMode>(GetWorld()->GetAuthGameMode());
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARZ_AIPatrolPoint::StaticClass(), PatrolPoints);
}

void ARZ_CharacterAIController::BeginPlay()
{
	Super::BeginPlay();

	GameMode->QueryRespawn(this);
}

void ARZ_CharacterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PossessedCharacter = Cast<ARZ_Character>(InPawn);
	if (PossessedCharacter.IsValid())
	{
		if (PossessedCharacter->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(PossessedCharacter->BehaviorTree->BlackboardAsset));
			BehaviorTreeComp->StartTree(*PossessedCharacter->BehaviorTree);
		}
	}
}
