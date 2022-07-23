/// RemzDNB

// RZ_Game
#include "AI/RZ_PawnAIController.h"
#include "Character/RZ_Character.h"
#include "Pawn/RZ_Pawn.h"
#include "Core/RZ_GameInstance.h"
#include "Core/RZ_GameMode.h"
// PerceptionSystem
#include "RZ_PerceptionComponent.h"
// Engine
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ARZ_PawnAIController::ARZ_PawnAIController()
{
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComp");
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComp");

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
}

void ARZ_PawnAIController::BeginPlay()
{
	Super::BeginPlay();
	
	IRZ_AIOwnerInterface* AIOwnerInterface = Cast<IRZ_AIOwnerInterface>(GetPawn());
	if (AIOwnerInterface)
	{
		BehaviorTree = AIOwnerInterface->GetBehaviorTree();
		//ToggleAI(true);
	}

}

void ARZ_PawnAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetPawn()) { return; }
	if (!BlackboardComp) { return; }
	
	// Rotate pawn to ViewTargetLocation.

	if (BlackboardComp->GetValueAsObject(TARGETACTOR_BBKEY))
	{
		const FVector ViewTargetLocation = BlackboardComp->GetValueAsVector(TARGETVIEWLOCATION_BBKEY);
		const FRotator NewPawnRotation = UKismetMathLibrary::FindLookAtRotation(
			GetPawn()->GetActorLocation(),
			ViewTargetLocation
		);
		const FRotator NewSmoothedPawnRotation = UKismetMathLibrary::RInterpTo_Constant(
			GetControlRotation(),
			NewPawnRotation,
			DeltaTime,
			1000.0f
		);
		SmoothedViewTargetLocation = UKismetMathLibrary::VInterpTo_Constant(
			SmoothedViewTargetLocation,
			ViewTargetLocation,
			DeltaTime,
			1000.0f);

		SetFocalPoint(SmoothedViewTargetLocation);
	}
	else
	{
		ACharacter* Char = Cast<ARZ_Character>(GetPawn());
		if (Char)
		{
			SetFocalPoint(GetPawn()->GetActorLocation() + Char->GetCharacterMovement()->Velocity);
		}
	}
	


	//GetPawn()->SetActorRotation(FRotator(0.0f, NewSmoothedPawnRotation.Yaw, 0.0f));
	//SetControlRotation(FRotator(GetControlRotation().Pitch, NewPawnRotation.Yaw, GetControlRotation().Roll));
	//SetFocus(FRotator(GetControlRotation().Pitch, NewPawnRotation.Yaw, GetControlRotation().Roll));
	/*GetPawn()->SetActorRotation(FRotator(
		0.0f,
		InterpTargetRotation.Yaw,
		0.0f
	));
	BlackboardCT->SetValueAsVector("SelfLocation", GetPawn()->GetActorLocation());*/
}

void ARZ_PawnAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//MoveToLocation(FVector::ZeroVector, 10.f);
	//MoveTo

	//return;
	
	IRZ_AIOwnerInterface* PawnInterface = Cast<IRZ_AIOwnerInterface>(GetPawn());
	if (!PawnInterface) { return; }

	BehaviorTree = GameSettings->CharacterBehaviorTree;
	
	if (BehaviorTree->BlackboardAsset)
	{
		BlackboardComp->InitializeBlackboard(*(BehaviorTree->BlackboardAsset));
		BehaviorTreeComp->StartTree(*BehaviorTree);
	}
	
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject("SelfActor", GetPawn());
		BlackboardComp->SetValueAsVector("MoveTargetLocation", FVector::ZeroVector);
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
		if (BlackboardComp)
			BlackboardComp->InitializeBlackboard(*(BehaviorTree->BlackboardAsset));
		if (BehaviorTree)
			BehaviorTreeComp->StartTree(*BehaviorTree);
		/*PossessedCharacter->GetAIPerceptionComponent()->OnPerceptionUpdated.AddUniqueDynamic(
			this,
			&ARZ_PawnAIController::OnActorPerceptionUpdated
		);*/

	}
	else
	{
		if (BehaviorTreeComp)
			BehaviorTreeComp->StopTree();
	}
}

void ARZ_PawnAIController::AddNewDelayedAction(FName ActionName, const FVector& TargetLocation, AActor* TargetActor)
{
	FRZ_AIAction NewAction;
	NewAction.ActionName = ActionName;
	NewAction.TargetLocation = TargetLocation;
	NewAction.TargetActor = TargetActor;
	
	ActionList.Add(NewAction);
	OnActionListUpdated.Broadcast();
}

void ARZ_PawnAIController::ConsumeLatestAction()
{
	if (ActionList.Num() != 0)
	{
		ActionList.RemoveAt(0);
		OnActionListUpdated.Broadcast();
	}
}

void ARZ_PawnAIController::SetMoveToLocation(const FVector& NewMoveToLocation)
{
	BlackboardComp->SetValueAsVector(TARGETMOVELOCATION_BBKEY, NewMoveToLocation);
}
