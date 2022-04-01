/// RemzDNB

// RZ_Game
#include "Game/RZ_SurvivalGameMode.h"
#include "Game/RZ_GameInstance.h"
#include "Game/RZ_GameState.h"
#include "Game/RZ_WorldSettings.h"
#include "Pawn/RZ_PawnStart.h"
#include "AI/RZ_PawnAIController.h"
#include "Character/RZ_Character.h"
// Engine
#include "EngineUtils.h"
#include "Game/RZ_GameSettings.h"
#include "Kismet/GameplayStatics.h"

ARZ_SurvivalGameMode::ARZ_SurvivalGameMode()
{
	GameStateClass = ARZ_GameState::StaticClass();
}

void ARZ_SurvivalGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ARZ_SurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	/*for (TActorIterator<ARZ_Building> Building(GetWorld()); Building; ++Building)
	{
		MainBuilding = *Building;
	}*/

	StartGame();
}

void ARZ_SurvivalGameMode::StartGame()
{
	SpawnAIWave();
	GetWorld()->GetTimerManager().SetTimer(WaveTimer, this, &ARZ_SurvivalGameMode::SpawnAIWave, GameSettings->AIWaveDelay, true, 0.0f);
}

void ARZ_SurvivalGameMode::StopGame()
{
	WaveTimer.Invalidate();
}

void ARZ_SurvivalGameMode::SpawnAIWave()
{
	// Spawn building wave

	// Spawn player waves

	for (const auto& PlayerCharacter : PlayerCharacters)
	{
		TArray<ARZ_PawnStart*> PawnStarts = GetValidPawnStarts(ERZ_PawnOwnership::WaveAI);
		for (uint32 Index = 0; Index < GameSettings->AISpawnCount; Index++)
		{
			if (PawnStarts.IsValidIndex(Index))
			{
				APawn* NewPawn = GetWorld()->SpawnActorDeferred<APawn>(
					GameSettings->DefaultCharacterClass,
					PawnStarts[Index]->GetStartTransform(),
					nullptr,
					nullptr,
					ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
				);
				if (NewPawn)
				{
					IRZ_PawnInterface* PawnInterface = Cast<IRZ_PawnInterface>(NewPawn);
					if (PawnInterface)
					{
						PawnInterface->Init(ERZ_PawnOwnership::WaveAI, 0);
						PawnInterface->SetAssignedTarget(PlayerCharacter);
					}
					
					UGameplayStatics::FinishSpawningActor(NewPawn, PawnStarts[Index]->GetStartTransform());
				}
			}
		}
	}

}
