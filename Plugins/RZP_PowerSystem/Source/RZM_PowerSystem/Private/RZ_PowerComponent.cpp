// RemzDNB

#include "RZ_PowerComponent.h"
// Engine
#include "EngineUtils.h"
#include "RZ_PowerManager.h"
#include "Kismet/KismetMathLibrary.h"

URZ_PowerComponent::URZ_PowerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URZ_PowerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<ARZ_PowerManager> FoundPowerManager(GetWorld()); FoundPowerManager; ++FoundPowerManager)
	{
		PowerManager = *FoundPowerManager;
	}

	if (PowerManager)
	{
		PowerManager->AddPowerComponent(this);
	}
}

TArray<URZ_PowerComponent*> URZ_PowerComponent::GetComponentsInRange() const
{
	TArray<URZ_PowerComponent*> ResultArray;

	if (!GetOwner()) { return ResultArray; }
	
	for (const auto& PowerComponent : PowerManager->GetPowerComponents())
	{
		if (!PowerComponent->GetOwner()) { break; }

		const float Distance = UKismetMathLibrary::Vector_Distance(
			GetOwner()->GetActorLocation(),
			PowerComponent->GetOwner()->GetActorLocation()
		);

		if (Distance - PowerCompSettings.PowerRange >= 0)
		{
			ResultArray.Add(PowerComponent);
		}
	}

	return ResultArray;
}