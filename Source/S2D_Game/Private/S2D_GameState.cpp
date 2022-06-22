#include "S2D_GameState.h"
#include "S2D_GridManager.h"

AS2D_GameState::AS2D_GameState()
{
}

void AS2D_GameState::BeginPlay()
{
	Super::BeginPlay();

	// Spawn GridManager.

	if (GetLocalRole() == ROLE_Authority)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AS2D_GridManager>(
			AS2D_GridManager::StaticClass(),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParameters
		);
	}
}
