#include "S2D_GameState.h"
#include "S2D_WorldTileManager.h"

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
		GetWorld()->SpawnActor<AS2D_WorldTileManager>(
			AS2D_WorldTileManager::StaticClass(),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParameters
		);
	}
}
