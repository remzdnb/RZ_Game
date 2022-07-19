/// RemzDNB

#include "S2D_Pawn.h"
#include "S2D_WorldTileManager.h"
//
#include "RZ_PowerManager.h"
#include "RZ_PowerComponent.h"
#include "RZ_InventoryComponent.h"
//
#include "EngineUtils.h"

AS2D_Pawn::AS2D_Pawn()
{
}

void AS2D_Pawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!GetWorld()->IsGameWorld()) { return; }

	for (TActorIterator<AS2D_WorldTileManager> GridManagerItr(GetWorld()); GridManagerItr; ++GridManagerItr)
	{
		GridManager = *GridManagerItr;
	}
}

void AS2D_Pawn::BeginPlay()
{
	Super::BeginPlay();
}

void AS2D_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AS2D_Pawn::OnBuildStart()
{
	Super::OnBuildStart();

	if (!GridManager) { return; }
	if (!PowerComp) { return; }
	if (!PowerComp->GetPowerManager()) { return; }

	GridManager->ClearActiveTiles();

	GridManager->AddSelection(
		GetActorLocation(),
		FIntPoint(PowerComp->GetPoweredAreaSize().X,
		          PowerComp->GetPoweredAreaSize().Y)
	);

	for (const auto& Grid : PowerComp->GetPowerManager()->GetPowerGrids())
	{
		for (const auto& AttachedPowerComponent : Grid.AttachedPowerComponents)
		{
			GridManager->AddSelection(
				AttachedPowerComponent->GetOwner()->GetActorLocation(),
				FIntPoint(AttachedPowerComponent->GetPoweredAreaSize().X,
						  AttachedPowerComponent->GetPoweredAreaSize().Y)
			);
		}
	}
	
	/*
	for (const auto PComp : PowerComp->PowerManager->GetPowerComponents())
	{
		if (PComp->IsActive())
		{
			GridManager->AddSelection(
				FIntPoint(PComp->GetOwner()->GetActorLocation().X, PComp->GetOwner()->GetActorLocation().Y),
				FIntPoint(
					GetActorSettings().NormalizedWorldSize.X + PowerComp->GetPowerComponentSettings().PowerRange,
					GetActorSettings().NormalizedWorldSize.Y + PowerComp->GetPowerComponentSettings().PowerRange
				)
			);
		}
	}*/
}

void AS2D_Pawn::OnBuildStop()
{
	Super::OnBuildStop();
}

void AS2D_Pawn::OnBuildEnd()
{
	Super::OnBuildEnd();

	GridManager->ClearActiveTiles();
}

void AS2D_Pawn::OnSelectedByInventory(bool bNewIsSelected)
{
	Super::OnSelectedByInventory(bNewIsSelected);

	if (!bNewIsSelected)
	{
		GridManager->ClearActiveTiles();
	}
}

void AS2D_Pawn::OnHoverStart()
{
	Super::OnHoverStart();
	
	if (!GridManager) { return; }
	if (!PowerComp) { return; }
	if (!PowerComp->GetPowerManager()) { return; }

	GridManager->ClearActiveTiles();

	for (const auto& Grid : PowerComp->GetPowerManager()->GetPowerGrids())
	{
		if (Grid.GridID == PowerComp->GetPowerGridID())
		{
			for (const auto& AttachedPowerComponent : Grid.AttachedPowerComponents)
			{
				GridManager->AddSelection(
					AttachedPowerComponent->GetOwner()->GetActorLocation(),
					FIntPoint(AttachedPowerComponent->GetPoweredAreaSize().X,
							  AttachedPowerComponent->GetPoweredAreaSize().Y)
				);
			}
		}
	}
	
	//GridManager->ShowPowerGrid(PowerComp->PowerManager->GetComponentsFromGrid(PowerComp->GetPowerGridID()));
	/*GridManager->AddSelection(
		GetActorLocation(),
		FIntPoint(
			GetActorSettings().NormalizedWorldSize.X + PowerComp->GetPowerComponentSettings().PowerRange,
			GetActorSettings().NormalizedWorldSize.Y + PowerComp->GetPowerComponentSettings().PowerRange
		)
	);
	for (const auto& ConnectedPowerComp : PowerComp->GetConnectedPowerComps())
	{
		GridManager->AddSelection(
			GetActorLocation(),
			FIntPoint(ConnectedPowerComp->GetPoweredAreaSize().X,
			          ConnectedPowerComp->GetPoweredAreaSize().Y)
		);
	}*/
}

void AS2D_Pawn::OnHoverEnd()
{
	Super::OnHoverEnd();

	if (!GridManager) { return; }

	GridManager->ClearActiveTiles();
}

void AS2D_Pawn::OnBuildLocationUpdated(const FVector& NewBuildLocation)
{
	Super::OnBuildLocationUpdated(NewBuildLocation);

	GridManager->ClearActiveTiles();

	GridManager->AddSelection(
		GetActorLocation(),
		FIntPoint(PowerComp->GetPoweredAreaSize().X,
				  PowerComp->GetPoweredAreaSize().Y)
	);

	for (const auto& Grid : PowerComp->GetPowerManager()->GetPowerGrids())
	{
		for (const auto& AttachedPowerComponent : Grid.AttachedPowerComponents)
		{
			GridManager->AddSelection(
				AttachedPowerComponent->GetOwner()->GetActorLocation(),
				FIntPoint(AttachedPowerComponent->GetPoweredAreaSize().X,
						  AttachedPowerComponent->GetPoweredAreaSize().Y)
			);
		}
	}
}



	