/// RemzDNB

// RZ_Game
#include "Game/RZ_SurvivalGameMode.h"
#include "Game/RZ_GameInstance.h"
#include "Game/RZ_WorldSettings.h"
#include "Pawn/RZ_PawnStart.h"
#include "AI/RZ_CharacterAIController.h"
// Engine
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

ARZ_SurvivalGameMode::ARZ_SurvivalGameMode()
{
	
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
	//GetWorld()->GetTimerManager().SetTimer(WaveTimer, this, &ARZ_SurvivalGameMode::SpawnAIWave, AIWAVEDELAY, false);
	//SpawnAIWave();
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
		for (uint8 Index = 0; Index < AIPAWNCOUNT_BASE; Index++)
		{
			if (PawnStarts.IsValidIndex(Index))
			{
				/*SpawnPawn(
					WorldSettings->WaveAICharacterClass,
					PawnStarts[Index]->GetStartTransform(),
					ERZ_PawnOwnership::WaveAI
				);*/

				ARZ_CharacterAIController* NewAIController = GetWorld()->SpawnActorDeferred<ARZ_CharacterAIController>(
					ARZ_CharacterAIController::StaticClass(),
					FTransform::Identity,
					this,
					nullptr,
					ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
				);
				if (NewAIController)
				{
					NewAIController->Init(PlayerCharacter, PawnStarts[Index]->GetStartTransform());
					UGameplayStatics::FinishSpawningActor(NewAIController, FTransform::Identity);
				}
			}
		}
	}

}
