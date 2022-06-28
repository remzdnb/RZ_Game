/// RemzDNB

// RZ_Game
#include "AI/RZ_PawnAIController.h"
#include "AI/RZ_PerceptionComponent.h"
#include "AI/RZ_AIPatrolPoint.h"
#include "Pawn/RZ_Character.h"
#include "Pawn/RZ_Pawn.h"
#include "Core/RZ_GameInstance.h"
#include "Core/RZ_GameMode.h"
#include "Core/RZ_GameSettings.h"
// Engine
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ARZ_PawnAIController::ARZ_PawnAIController()
{
	BehaviorTreeCT = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComp");
	BlackboardCT = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComp");

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ARZ_PawnAIController::Init(APawn* NewTargetPawn, const FTransform& SpawnLocation)
{
	TargetPawn = NewTargetPawn;
	PawnSpawnLocation = SpawnLocation;
}

void ARZ_PawnAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GameMode = Cast<ARZ_GameMode>(GetWorld()->GetAuthGameMode());
	GameSettings = Cast<URZ_GameInstance>(GetGameInstance())->GetGameSettings();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARZ_AIPatrolPoint::StaticClass(), PatrolPoints);
}

void ARZ_PawnAIController::BeginPlay()
{
	Super::BeginPlay();

	if (!GetPawn()) { return; }
	
	BehaviorTree = GameSettings->TurretBehaviorTree;
	
	//ToggleAI(true);
}

void ARZ_PawnAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetPawn()) { return; }
	if (!BlackboardCT) { return; }

	//
/*
	IRZ_ItemInterface* ItemInterface = Cast<IRZ_ItemInterface>(GetPawn()); // dont do that on tick ffs
	if (ItemInterface)
	{
		if (ItemInterface->GetIsSelected())
			return;
	}*/
	
	// Rotate pawn to AI active target location.
	
	const AActor* ActiveTarget = Cast<AActor>(BlackboardCT->GetValueAsObject("ActiveTargetActor"));
	
	FRotator BaseTargetRotation;

	if (ActiveTarget)
	{
		BaseTargetRotation = UKismetMathLibrary::FindLookAtRotation(
			GetPawn()->GetActorLocation(),
			ActiveTarget->GetActorLocation()
		);
	}
	else
	{
		BaseTargetRotation = InitialRotation;
	}
	
	const FRotator InterpTargetRotation = UKismetMathLibrary::RInterpTo_Constant(
		GetControlRotation(),
		BaseTargetRotation,
		DeltaTime,
		100.0f
	);

	GetPawn()->SetActorRotation(FRotator(
		0.0f,
		InterpTargetRotation.Yaw,
		0.0f
	));


	BlackboardCT->SetValueAsVector("SelfLocation", GetPawn()->GetActorLocation());

	IRZ_PawnInterface* PawnInterface = Cast<IRZ_PawnInterface>(GetPawn()); // cast on tick :/
	if (!PawnInterface) { return; }
	BlackboardCT->SetValueAsObject("AssignedTargetActor", PawnInterface->GetAssignedTarget());
}

void ARZ_PawnAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	IRZ_PawnInterface* PawnInterface = Cast<IRZ_PawnInterface>(GetPawn());
	if (!PawnInterface) { return; }

	BehaviorTree = PawnInterface->GetBehaviorTree();
	
	if (BehaviorTree->BlackboardAsset)
	{
		//BlackboardCT->InitializeBlackboard(*(BehaviorTree->BlackboardAsset));
		//BehaviorTreeCT->StartTree(*BehaviorTree);
	}
	
	if (BlackboardCT)
	{
		BlackboardCT->SetValueAsObject("SelfActor", GetPawn());

	}

	InitialRotation = GetPawn()->GetActorRotation();

// smooth control here

}

void ARZ_PawnAIController::ToggleAI(bool bNewIsEnabled)
{
	// Manage case where it is called before pawn is possessed.
	
	if (!GetPawn()) { return; }
	if (!BehaviorTree) { return; }

	PawnPerceptionComponent = Cast<URZ_PerceptionComponent>(
		GetPawn()->GetComponentByClass(URZ_PerceptionComponent::StaticClass())
	);
	if (!PawnPerceptionComponent.IsValid()) { return;}
	
	if (bNewIsEnabled)
	{
		/*if (BlackboardCT)
			BlackboardCT->InitializeBlackboard(*(BehaviorTree->BlackboardAsset));
		if (BehaviorTree)
			BehaviorTreeCT->StartTree(*BehaviorTree);*/
		/*PossessedCharacter->GetAIPerceptionComponent()->OnPerceptionUpdated.AddUniqueDynamic(
			this,
			&ARZ_PawnAIController::OnActorPerceptionUpdated
		);*/

	}
	else
	{
		//if (BehaviorTreeCT)
			//BehaviorTreeCT->StopTree();
	}
}

void ARZ_PawnAIController::UpdateActiveTarget()
{
	if (!GetPawn()) { return; }
	
	if (!PawnPerceptionComponent.IsValid())
	{
		PawnPerceptionComponent = Cast<URZ_PerceptionComponent>(
			GetPawn()->GetComponentByClass(URZ_PerceptionComponent::StaticClass())
		);
	}

	if (!PawnPerceptionComponent.IsValid()) { return; }

	PawnPerceptionComponent->UpdateSensedActors(); // noice
	
	if (!BlackboardCT) { return; }

	const IRZ_PawnInterface* OwnerPawnCombatInterface = Cast<IRZ_PawnInterface>(GetPawn());
	if (!OwnerPawnCombatInterface) { return; }

	// check team
	
	TArray<AActor*> EnemyActors;
	for (const auto& SensedActor : PawnPerceptionComponent->GetSensedActors())
	{
		const IRZ_PawnInterface* SensedActorCombatInterface = Cast<IRZ_PawnInterface>(SensedActor);
		if (!SensedActorCombatInterface) { break; }

		if (OwnerPawnCombatInterface->GetPawnOwnership() != SensedActorCombatInterface->GetPawnOwnership())
		{
			EnemyActors.Add(SensedActor.Get());
		}
	}

	// check distance
	
	AActor* ActiveTarget = GetClosestActorFromLocation(EnemyActors, GetPawn()->GetActorLocation());
	BlackboardCT->SetValueAsObject("ActiveTargetActor", ActiveTarget);
	if (ActiveTarget)
	{
		BlackboardCT->SetValueAsVector("ActiveTargetLocation", ActiveTarget->GetActorLocation());
	}

	IRZ_PawnInterface* OwnerPawnInterface = Cast<IRZ_PawnInterface>(GetPawn());
	if (OwnerPawnInterface)
	{
		OwnerPawnInterface->SetActiveTarget(ActiveTarget);
	}
}

void ARZ_PawnAIController::SetFinalTargetActor(APawn* NewTargetPawn)
{
	BlackboardCT->SetValueAsObject("FinalTargetActor", NewTargetPawn);
}
