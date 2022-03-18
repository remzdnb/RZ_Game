// RemzDNB


#include "Building/RZ_BaseBuilding.h"

// Sets default values
ARZ_BaseBuilding::ARZ_BaseBuilding()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARZ_BaseBuilding::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARZ_BaseBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARZ_BaseBuilding::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

