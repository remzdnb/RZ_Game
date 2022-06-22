#include "RZ_PowerManager.h"
#include "RZ_PowerComponent.h"

ARZ_PowerManager::ARZ_PowerManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARZ_PowerManager::BeginPlay()
{
	Super::BeginPlay();
}

TArray<URZ_PowerComponent*> ARZ_PowerManager::GetComponentsFromGrid(int32 GridID)
{
	if (!Grids.IsValidIndex(GridID)) { return TArray<URZ_PowerComponent*>(); }

	return (Grids[GridID].AttachedPowerComponents);
}

void ARZ_PowerManager::AddPowerComponent(URZ_PowerComponent* InPowerComponent)
{
	if (!PowerComponents.Contains(InPowerComponent))
	{
		PowerComponents.Add(InPowerComponent);
		ReevaluteGrids();
	}
}

void ARZ_PowerManager::RemovePowerComponent(URZ_PowerComponent* InPowerComponent)
{
	if (PowerComponents.Contains(InPowerComponent))
	{
		PowerComponents.Remove(InPowerComponent);
		ReevaluteGrids();
	}
}

void ARZ_PowerManager::ReevaluteGrids()
{
	Grids.Empty();
	
	for (const auto& PowerComponent : PowerComponents)
	{
		if (PowerComponent->GetComponentsInRange().Num() == 0)
		{
			CreateGrid(PowerComponent, nullptr);
			break;	
		}
		
		for (const auto& PowerComponentInRange : PowerComponent->GetComponentsInRange())
		{
			if (PowerComponent->PowerGridID == 0 && PowerComponentInRange->PowerGridID == 0)
			{
				CreateGrid(PowerComponent, PowerComponentInRange);
				break;
			}

			if (PowerComponent->PowerGridID != 0 && PowerComponentInRange->PowerGridID != 0)
			{
				MergeGrids(PowerComponent->PowerGridID, PowerComponentInRange->PowerGridID);
				break;
			}

			if (PowerComponent->PowerGridID == 0)
			{
				AddToGrid(PowerComponentInRange->PowerGridID, PowerComponent);
				break;
			}

			if (PowerComponentInRange->PowerGridID == 0)
			{
				AddToGrid(PowerComponent->PowerGridID, PowerComponentInRange);
				break;
			}
		}
	}
}

void ARZ_PowerManager::CreateGrid(URZ_PowerComponent* FirstPowerComponent, URZ_PowerComponent* SecondPowerComponent)
{
	if (!FirstPowerComponent) { return; }

	FRZ_PowerGridInfo TempPowerGridInfo;
	const uint8 NewGridID = Grids.Max() + 1;
	
	TempPowerGridInfo.GridID = FirstPowerComponent->PowerGridID = NewGridID;
	TempPowerGridInfo.AttachedPowerComponents.Add(FirstPowerComponent);
	
	if (SecondPowerComponent)
	{
		SecondPowerComponent->PowerGridID = NewGridID;
		TempPowerGridInfo.AttachedPowerComponents.Add(SecondPowerComponent);
	}
	
	Grids.Add(TempPowerGridInfo);
}


void ARZ_PowerManager::AddToGrid(uint8 GridID, URZ_PowerComponent* PowerComponent)
{
	if (!PowerComponent ||
		!Grids.IsValidIndex(GridID) ||
		Grids[GridID].AttachedPowerComponents.Contains(PowerComponent)
	) { return; }

	PowerComponent->PowerGridID = GridID;
	Grids[GridID].AttachedPowerComponents.Add(PowerComponent);
}

void ARZ_PowerManager::MergeGrids(uint8 FirstGridID, uint8 SecondGridID)
{
	if (!Grids.IsValidIndex(FirstGridID) || !Grids.IsValidIndex(SecondGridID)) { return; }

	for (const auto& PowerComponent : Grids[SecondGridID].AttachedPowerComponents)
	{
		PowerComponent->PowerGridID = FirstGridID;
	}

	Grids.RemoveAt(SecondGridID);
}


