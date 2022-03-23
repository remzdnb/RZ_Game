#include "Game/RZ_GameMode.h"
#include "Game/RZ_WorldSettings.h"
#include "Player/RZ_PlayerController.h"
#include "RZ_PawnStart.h"
#include "AI/RZ_CharacterAIController.h"
//
#include "RZ_UIManager.h"
//
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

ARZ_GameMode::ARZ_GameMode()
{
	GameStateClass = nullptr;
	PlayerStateClass = nullptr;
	PlayerControllerClass = ARZ_PlayerController::StaticClass();
	HUDClass = ARZ_UIManager::StaticClass();
	DefaultPawnClass = nullptr;
	SpectatorClass = nullptr;

	PrimaryActorTick.bCanEverTick = true;

	PawnStartIndex = 0;
}

void ARZ_GameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	WorldSettings = Cast<ARZ_WorldSettings>(GetWorld()->GetWorldSettings());

	for (TActorIterator<ARZ_PawnStart> PawnStart(GetWorld()); PawnStart; ++PawnStart)
	{
		PawnStarts.Add(*PawnStart);
	}
}

void ARZ_GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ARZ_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle controllers pawn respawn
	
	if (ReadyControllers.Num() != 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ARZ_GameMode::Tick : 0"));
		
		const ARZ_PawnStart* PawnStart = GetAvailablePawnStart();
		if (PawnStart)
		{
			//UE_LOG(LogTemp, Warning, TEXT("ARZ_GameMode::Tick : 1"));
			APawn* SpawnedPawn = SpawnPawn(PawnStart->GetStartTransform());
			if (SpawnedPawn)
			{
				//UE_LOG(LogTemp, Warning, TEXT("ARZ_GameMode::Tick : 2"));
				ReadyControllers[0]->Possess(SpawnedPawn);
				ReadyControllers[0]->OnRep_Pawn();
				ReadyControllers.RemoveAt(0);
			}
		}
	}
}

void ARZ_GameMode::QueryRespawn(AController* NewController)
{
	if (ReadyControllers.Contains(NewController) == false)
	{
		ReadyControllers.Add(NewController);
	}
}

FTransform ARZ_GameMode::QuerySpawnLocation()
{
	if (PawnStarts.Max() == 0)
		return FTransform::Identity;
	
	FTransform TransformResult = FTransform::Identity;
	
	if (PawnStarts.IsValidIndex(PawnStartIndex))
	{
		TransformResult = PawnStarts[PawnStartIndex]->GetStartTransform();
		PawnStartIndex++;
	}
	else
	{
		TransformResult = PawnStarts[0]->GetStartTransform();
		PawnStartIndex = 0;
	}
	
	return TransformResult;
}

APawn* ARZ_GameMode::SpawnPawn(const FTransform& SpawnTransform)
{
	APawn* NewPawn = GetWorld()->SpawnActorDeferred<APawn>(
		WorldSettings->DefaultPawnClass,
		SpawnTransform,
		this,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);
	if (NewPawn)
	{
		UGameplayStatics::FinishSpawningActor(NewPawn, SpawnTransform);
		return NewPawn;
	}

	return nullptr;
}

ARZ_PawnStart* ARZ_GameMode::GetAvailablePawnStart()
{
	if (PawnStarts.Num() == 0)
		return nullptr;
	
	for (uint8 Index = 0; Index < PawnStarts.Num(); Index++)
	{
		if (PawnStarts.IsValidIndex(Index))
		{
			if (PawnStarts[Index]->GetIsAvailable())
			{
				return PawnStarts[Index].Get();
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("ARZ_GameMode::GetAvailablePawnStart : -1"));

	return nullptr;
}

void ARZ_GameMode::AddAIController()
{
	ARZ_CharacterAIController* NewAIController = GetWorld()->SpawnActorDeferred<ARZ_CharacterAIController>(
		ARZ_CharacterAIController::StaticClass(),
		FTransform::Identity,
		this,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);
	if (NewAIController)
	{
		UGameplayStatics::FinishSpawningActor(NewAIController, FTransform::Identity);
	}
}

void ARZ_GameMode::RemoveAIController()
{
	
}
