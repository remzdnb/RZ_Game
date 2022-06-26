/// RemzDNB

#include "S2D_Pawn.h"
#include "S2D_GridManager.h"
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

	for (TActorIterator<AS2D_GridManager> GridManagerItr(GetWorld()); GridManagerItr; ++GridManagerItr)
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
	if (!PowerComp->PowerManager) { return; }

	GridManager->ClearActiveTiles();

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
	}
}

void AS2D_Pawn::OnBuildStop()
{
	Super::OnBuildStop();
}

void AS2D_Pawn::OnBuildEnd()
{
	Super::OnBuildEnd();
}

void AS2D_Pawn::OnHoverStart()
{
	Super::OnHoverStart();
	
	if (!GridManager) { return; }
	if (!PowerComp) { return; }
	if (!PowerComp->PowerManager) { return; }
	
	//GridManager->ShowPowerGrid(PowerComp->PowerManager->GetComponentsFromGrid(PowerComp->GetPowerGridID()));
	GridManager->AddSelection(
		FIntPoint(GetActorLocation().X, GetActorLocation().Y),
		FIntPoint(
			GetActorSettings().NormalizedWorldSize.X + PowerComp->GetPowerComponentSettings().PowerRange,
			GetActorSettings().NormalizedWorldSize.Y + PowerComp->GetPowerComponentSettings().PowerRange
		)
	);
	for (const auto& ConnectedPowerComp : PowerComp->GetConnectedPowerComps())
	{
		GridManager->AddSelection(
			FIntPoint(ConnectedPowerComp->GetOwner()->GetActorLocation().X,
			          ConnectedPowerComp->GetOwner()->GetActorLocation().Y),
			FIntPoint(ConnectedPowerComp->GetPoweredAreaSize().X,
			          ConnectedPowerComp->GetPoweredAreaSize().Y)
		);
	}
}

void AS2D_Pawn::OnHoverEnd()
{
	Super::OnHoverEnd();

	if (!GridManager) { return; }

	GridManager->ClearActiveTiles();
}



	