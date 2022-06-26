/// RemzDNB

// RZ_Game
#include "Core/RZ_SurvivalGameMode.h"
#include "Core/RZ_GameInstance.h"
#include "Core/RZ_GameState.h"
#include "Core/RZ_WorldSettings.h"
#include "Pawn/RZ_PawnStart.h"
#include "AI/RZ_PawnAIController.h"
#include "Pawn/RZ_Character.h"
// Engine
#include "EngineUtils.h"
#include "Core/RZ_GameSettings.h"
#include "Kismet/GameplayStatics.h"

ARZ_SurvivalGameMode::ARZ_SurvivalGameMode()
{
	GameStateClass = ARZ_GameState::StaticClass();
}

void ARZ_SurvivalGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!GetWorld()->IsGameWorld()) { return; }
}

void ARZ_SurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	/*for (TActorIterator<ARZ_Pawn> Building(GetWorld()); Building; ++Building)
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
						PawnInterface->InitCombatInterface(ERZ_PawnOwnership::WaveAI, 0);
						PawnInterface->SetAssignedTarget(PlayerCharacter);
					}
					
					UGameplayStatics::FinishSpawningActor(NewPawn, PawnStarts[Index]->GetStartTransform());
				}
			}
		}
	}

}
