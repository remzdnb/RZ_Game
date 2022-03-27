/// RemzDNB

// RZ_Game
#include "AI/RZ_CharacterAIController.h"
#include "AI/RZ_AIPatrolPoint.h"
#include "Character/RZ_Character.h"
#include "Game/RZ_GameInstance.h"
#include "Game/RZ_GameMode.h"
#include "Game/RZ_GameSettings.h"
// Engine
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"

ARZ_CharacterAIController::ARZ_CharacterAIController()
{
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComp");
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComp");
}

void ARZ_CharacterAIController::Init(APawn* NewTargetPawn, const FTransform& SpawnLocation)
{
	TargetPawn = NewTargetPawn;
	PawnSpawnLocation = SpawnLocation;
}

void ARZ_CharacterAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GameMode = Cast<ARZ_GameMode>(GetWorld()->GetAuthGameMode());
	GameSettings = Cast<URZ_GameInstance>(GetGameInstance())->GetGameSettings();
	BehaviorTree = GameSettings->CharacterBehaviorTree;
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARZ_AIPatrolPoint::StaticClass(), PatrolPoints);
}

void ARZ_CharacterAIController::BeginPlay()
{
	Super::BeginPlay();

	//Cast<ARZ_Character>(BlackboardComp->GetValueAsObject("FinalTarget"))->GetName();
	
	ARZ_Character* NewCharacter = GetWorld()->SpawnActorDeferred<ARZ_Character>(
		GameSettings->DefaultCharacterClass,
		PawnSpawnLocation,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);
	if (NewCharacter)
	{
		NewCharacter->Init(ERZ_PawnOwnership::WaveAI);
		UGameplayStatics::FinishSpawningActor(NewCharacter, PawnSpawnLocation);
		Possess(NewCharacter);
	}
	
	//GameMode->QueryRespawn(this);
}

void ARZ_CharacterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PossessedCharacter = Cast<ARZ_Character>(InPawn);
	if (PossessedCharacter.IsValid())
	{
		PossessedCharacter->GetAIPerceptionComponent()->OnPerceptionUpdated.AddUniqueDynamic(
			this,
			&ARZ_CharacterAIController::OnActorPerceptionUpdated
		);
		
		if (BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(BehaviorTree->BlackboardAsset));
			BehaviorTreeComp->StartTree(*BehaviorTree);
			BlackboardComp->SetValueAsObject("FinalTarget", TargetPawn);
		}
	}
}

void ARZ_CharacterAIController::SetFinalTargetPawn(APawn* NewTargetPawn)
{
	BlackboardComp->SetValueAsObject("FinalTargetPoint", NewTargetPawn);
}

void ARZ_CharacterAIController::OnActorPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	BlackboardComp->SetValueAsObject("ActiveTarget", nullptr);

	uint8 Index = 0;
	for (const auto& DetectedActor : UpdatedActors)
	{
		const IGameplayTagAssetInterface* GameplayTagAssetInterface = Cast<IGameplayTagAssetInterface>(DetectedActor);
		if (GameplayTagAssetInterface)
		{
			if (GameplayTagAssetInterface->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("PawnOwnership.Player"))))
			{
				BlackboardComp->SetValueAsObject("ActiveTarget", DetectedActor);
			}
		}

		Index++;
	}
}
