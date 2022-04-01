/// RemzDNB

// RZ_Game
#include "Game/RZ_GameMode.h"
#include "Game/RZ_GameInstance.h"
#include "Game/RZ_GameState.h"
#include "Game/RZ_GameSettings.h"
#include "Game/RZ_WorldSettings.h"
#include "Player/RZ_PlayerController.h"
#include "Pawn/RZ_PawnStart.h"
#include "Character/RZ_Character.h"
#include "AI/RZ_PawnAIController.h"
// UI plugin
#include "RZ_UIManager.h"
// Engine
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

	GameState = GetGameState<ARZ_GameState>();
	GameSettings = Cast<URZ_GameInstance>(GetGameInstance())->GetGameSettings();
	WorldSettings = Cast<ARZ_WorldSettings>(GetWorld()->GetWorldSettings());

	for (TActorIterator<ARZ_PawnStart> PawnStart(GetWorld()); PawnStart; ++PawnStart)
	{
		if (PawnStart->GetOwnership() == ERZ_PawnOwnership::Player)
			PlayerPawnStarts.Add(*PawnStart);
		else if (PawnStart->GetOwnership() == ERZ_PawnOwnership::WaveAI)
			WaveAIPawnStarts.Add(*PawnStart);
		else
			FreeAIPawnStarts.Add(*PawnStart);
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
			APawn* SpawnedPawn = SpawnPawn(GameSettings->DefaultCharacterClass, PawnStart->GetStartTransform(), ERZ_PawnOwnership::Player);
			if (SpawnedPawn)
			{
				//UE_LOG(LogTemp, Warning, TEXT("ARZ_GameMode::Tick : 2"));
				SpawnedPawn->SetOwner(ReadyControllers[0].Get());
				if (Cast<ARZ_Character>(SpawnedPawn))
				{
					Cast<ARZ_Character>(SpawnedPawn)->Init(ERZ_PawnOwnership::Player,0);
				}
				ReadyControllers[0]->Possess(SpawnedPawn);
				ReadyControllers[0]->OnRep_Pawn();
				ReadyControllers.RemoveAt(0);
				PlayerCharacters.Add(SpawnedPawn);
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
	if (PlayerPawnStarts.Max() == 0)
		return FTransform::Identity;
	
	FTransform TransformResult = FTransform::Identity;
	
	if (PlayerPawnStarts.IsValidIndex(PawnStartIndex))
	{
		TransformResult = PlayerPawnStarts[PawnStartIndex]->GetStartTransform();
		PawnStartIndex++;
	}
	else
	{
		TransformResult = PlayerPawnStarts[0]->GetStartTransform();
		PawnStartIndex = 0;
	}
	
	return TransformResult;
}

APawn* ARZ_GameMode::SpawnPawn(TSubclassOf<APawn> PawnClass, const FTransform& SpawnTransform, ERZ_PawnOwnership Ownership)
{
	APawn* NewPawn = GetWorld()->SpawnActorDeferred<APawn>(
		PawnClass,
		SpawnTransform,
		nullptr,
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

TArray<ARZ_PawnStart*> ARZ_GameMode::GetValidPawnStarts(ERZ_PawnOwnership Ownership) const
{
	TArray<ARZ_PawnStart*> ResultArray;

	// use field array
	
	for (TActorIterator<ARZ_PawnStart> PawnStart(GetWorld()); PawnStart; ++PawnStart)
	{
		if (PawnStart->GetOwnership() == Ownership && PawnStart->GetIsAvailable() && PawnStart->GetIsEnabled())
		{
			ResultArray.Add(*PawnStart);
		}
	}
	
	return ResultArray;
}

ARZ_PawnStart* ARZ_GameMode::GetAvailablePawnStart()
{
	if (PlayerPawnStarts.Num() == 0)
		return nullptr;
	
	for (uint8 Index = 0; Index < PlayerPawnStarts.Num(); Index++)
	{
		if (PlayerPawnStarts.IsValidIndex(Index))
		{
			if (PlayerPawnStarts[Index]->GetIsAvailable() && PlayerPawnStarts[Index]->GetIsEnabled())
			{
				return PlayerPawnStarts[Index].Get();
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("ARZ_GameMode::GetAvailablePawnStart : -1"));

	return nullptr;
}

void ARZ_GameMode::AddAIController()
{
	ARZ_PawnAIController* NewAIController = GetWorld()->SpawnActorDeferred<ARZ_PawnAIController>(
		ARZ_PawnAIController::StaticClass(),
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
