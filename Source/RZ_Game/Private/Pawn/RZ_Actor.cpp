// RemzDNB


#include "Pawn/RZ_Actor.h"

// Sets default values
ARZ_Actor::ARZ_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARZ_Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARZ_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

