// RemzDNB


#include "Pawn/RZ_Vehicle.h"

// Sets default values
ARZ_Vehicle::ARZ_Vehicle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARZ_Vehicle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARZ_Vehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARZ_Vehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

