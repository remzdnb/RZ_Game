// RemzDNB


#include "Vehicle/RZ_BaseVehicle.h"

// Sets default values
ARZ_BaseVehicle::ARZ_BaseVehicle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARZ_BaseVehicle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARZ_BaseVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARZ_BaseVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

