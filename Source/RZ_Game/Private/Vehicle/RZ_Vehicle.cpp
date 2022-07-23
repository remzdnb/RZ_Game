/// RemzDNB

#include "Vehicle/RZ_Vehicle.h"

ARZ_Vehicle::ARZ_Vehicle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARZ_Vehicle::BeginPlay()
{
	Super::BeginPlay();
}

void ARZ_Vehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
