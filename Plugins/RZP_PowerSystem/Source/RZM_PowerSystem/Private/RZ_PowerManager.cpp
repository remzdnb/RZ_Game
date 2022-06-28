#include "RZ_PowerManager.h"
#include "RZ_PowerComponent.h"

ARZ_PowerManager::ARZ_PowerManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SelectedGridID = -1;
	bDebug = true;
}

void ARZ_PowerManager::BeginPlay()
{
	Super::BeginPlay();
}

void ARZ_PowerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Debug(DeltaTime);
}

void ARZ_PowerManager::ReevaluteGrids()
{
	PowerGrids.Empty();

	for (const auto& PowerComponent : PowerComponents)
	{
		PowerComponent->SetPowerGridID(-1);
	}
	
	for (const auto& PowerComponent : PowerComponents)
	{
		if (PowerComponent->bIsDisabled)
		{
			// Skip
		}
		else if (PowerComponent->GetPowerGridID() == -1 && PowerComponent->GetConnectedPowerComps().Num() == 0)
		{
			CreateGrid(PowerComponent);
		}
		else
		{
			for (const auto& PowerComponentInRange : PowerComponent->GetConnectedPowerComps())
			{
				if (PowerComponentInRange->bIsDisabled)
				{
					// Skip
				}
				else if (PowerComponent->GetPowerGridID() == -1 && PowerComponentInRange->GetPowerGridID() == -1)
				{
					const int32 NewGridID = CreateGrid(PowerComponent);
					AddComponentToGrid(NewGridID, PowerComponentInRange);
				}
				else if (PowerComponent->GetPowerGridID() == -1 && !(PowerComponentInRange->GetPowerGridID() == -1))
				{
					AddComponentToGrid(PowerComponentInRange->GetPowerGridID(), PowerComponent);
				}
				else if (PowerComponentInRange->GetPowerGridID() == -1 && !(PowerComponent->GetPowerGridID() == -1))
				{
					AddComponentToGrid(PowerComponent->GetPowerGridID(), PowerComponentInRange);
				}
				else if (PowerComponent->GetPowerGridID() != -1 && PowerComponentInRange->GetPowerGridID() != -1)
				{
					//MergeGrids(PowerComponent->GetPowerGridID(), PowerComponentInRange->GetPowerGridID());
				}
			}
		}
	}
	
	UpdateSavedGrids();
	OnPowerManagerUpdated.Broadcast();
}

int32 ARZ_PowerManager::CreateGrid(URZ_PowerComponent* PowerComponent)
{
	if (!PowerComponent) { return 0; }

	const int32 NewGridID = PowerGrids.Num();

	FRZ_PowerGridInfo TempPowerGridInfo;
	TempPowerGridInfo.GridID = NewGridID;
	TempPowerGridInfo.AttachedPowerComponents.Add(PowerComponent);
	TempPowerGridInfo.UpdateTotalGridPower();
	PowerGrids.Add(TempPowerGridInfo);

	PowerComponent->SetPowerGridID(NewGridID);

	if (bDebug)
	{
		UE_LOG(LogTemp, Display, TEXT("ARZ_PowerManager::CreateGrid // GridCreated - GridID == %i"), NewGridID);
	}

	return NewGridID;
}


void ARZ_PowerManager::AddComponentToGrid(int32 GridID, URZ_PowerComponent* PowerComponent)
{
	if (!PowerComponent ||
		!PowerGrids.IsValidIndex(GridID) ||
		PowerGrids[GridID].AttachedPowerComponents.Contains(PowerComponent)
	) { return; }

	PowerComponent->SetPowerGridID(GridID);
	PowerGrids[GridID].AttachedPowerComponents.Add(PowerComponent);
	PowerGrids[GridID].UpdateTotalGridPower();

	if (bDebug) { UE_LOG(LogTemp, Display, TEXT("ARZ_PowerManager::AddToGrid : GridID == %i"), GridID); }
}

void ARZ_PowerManager::RemoveComponentFromGrid(int32 GridID, URZ_PowerComponent* PowerComponent)
{
	if (!PowerComponent ||
		!PowerGrids.IsValidIndex(GridID) ||
		!PowerGrids[GridID].AttachedPowerComponents.Contains(PowerComponent)
	) { return; }

	PowerComponent->SetPowerGridID(0);
	PowerGrids[GridID].AttachedPowerComponents.Remove(PowerComponent);
	PowerGrids[GridID].UpdateTotalGridPower();

	if (PowerGrids[GridID].AttachedPowerComponents.Num() == 0)
	{
		PowerGrids.RemoveAt(GridID);
	}

	if (bDebug) { UE_LOG(LogTemp, Display, TEXT("ARZ_PowerManager::RemoveComponentFromGrid : GridID == %i"), GridID); }
}

void ARZ_PowerManager::MergeGrids(int32 FirstGridID, int32 SecondGridID)
{
	if (!PowerGrids.IsValidIndex(FirstGridID) || !PowerGrids.IsValidIndex(SecondGridID)) { return; }

	for (const auto& PowerComponent : PowerGrids[SecondGridID].AttachedPowerComponents)
	{
		PowerComponent->SetPowerGridID(FirstGridID);
	}

	PowerGrids.RemoveAt(SecondGridID);
	PowerGrids[FirstGridID].UpdateTotalGridPower();

	if (bDebug) { UE_LOG(LogTemp, Display, TEXT("ARZ_PowerManager::MergeGrids : FirstGridID == %i // SecondGridID == %i"), FirstGridID, SecondGridID); }
}

void ARZ_PowerManager::AddPowerComponentRef(URZ_PowerComponent* InPowerComponent)
{
	if (!PowerComponents.Contains(InPowerComponent))
	{
		PowerComponents.Add(InPowerComponent);
	}
}

void ARZ_PowerManager::RemovePowerComponentRef(URZ_PowerComponent* InPowerComponent)
{
	if (PowerComponents.Contains(InPowerComponent))
	{
		PowerComponents.Remove(InPowerComponent);
	}
}

void ARZ_PowerManager::UpdateSavedGrids()
{
	float TempPower = 0.0f;
	
	for (auto& Grid : PowerGrids)
	{
		TempPower = 0.0f;
		for (const auto& AttachedComponent : Grid.AttachedPowerComponents)
		{
			TempPower += AttachedComponent->GetPowerComponentSettings().MaxProducedPower;
		}
		Grid.ProducedPower = TempPower;

		TempPower = 0.0f;
		for (const auto& AttachedComponent : Grid.AttachedPowerComponents)
		{
			TempPower += AttachedComponent->GetPowerComponentSettings().MaxConsumedPower;
		}
		Grid.ConsumedPower = TempPower;
	}
}

void ARZ_PowerManager::SelectGridID(int32 NewSelectedGridID)
{
	if (NewSelectedGridID == SelectedGridID) { return; }

	SelectedGridID = NewSelectedGridID;

	OnNewGridSelected.Broadcast(NewSelectedGridID);
	OnPowerManagerUpdated.Broadcast();
}

void ARZ_PowerManager::SelectPowerComponent(URZ_PowerComponent* NewSelectedComponent)
{
	if (NewSelectedComponent == SelectedComponent) { return; }

	SelectedComponent = NewSelectedComponent;

	OnNewComponentSelected.Broadcast(NewSelectedComponent);
	OnPowerManagerUpdated.Broadcast();
}

TArray<URZ_PowerComponent*> ARZ_PowerManager::GetComponentsFromGrid(int32 GridID)
{
	if (!PowerGrids.IsValidIndex(GridID)) { return TArray<URZ_PowerComponent*>(); }

	return (PowerGrids[GridID].AttachedPowerComponents);
}

void ARZ_PowerManager::Debug(float DeltaTime)
{
	if (!bDebug) { return; }
	
	const FString SeparatorString = " ------------------------------------------------------------- ";
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, SeparatorString);;
	
	for (const auto& Grid : PowerGrids)
	{
		FString GridString = "Active Grid - ID == " + FString::FromInt(Grid.GridID);
		for (const auto& AttachedPowerComponent : Grid.AttachedPowerComponents)
		{
			GridString += " // Attached == " + AttachedPowerComponent->GetOwner()->GetName();
		}

		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, GridString);;
	}
	
	const FString FinalString = " ------------------- ARZ_PowerManager::Debug ------------------- ";
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, FinalString);;
}


