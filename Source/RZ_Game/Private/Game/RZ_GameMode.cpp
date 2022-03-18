#include "Game/RZ_GameMode.h"
#include "Game/RZ_WorldSettings.h"
#include "Player/RZ_PlayerController.h"
#include "Character/RZ_CharacterStart.h"
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

	PawnStartIndex = 0;
}

void ARZ_GameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	WorldSettings = Cast<ARZ_WorldSettings>(GetWorld()->GetWorldSettings());

	for (TActorIterator<ARZ_CharacterStart> PawnStart(GetWorld()); PawnStart; ++PawnStart)
	{
		PawnStarts.Add(*PawnStart);
	}
}

void ARZ_GameMode::BeginPlay()
{
	Super::BeginPlay();
}

FTransform ARZ_GameMode::QuerySpawnLocation()
{
	if (PawnStarts.Max() == 0)
		return FTransform::Identity;

	FTransform TransformResult;
	
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

APawn* ARZ_GameMode::SpawnPawn(TSubclassOf<APawn> PawnClass)
{
	const FTransform SpawnTransform = QuerySpawnLocation();
	
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
