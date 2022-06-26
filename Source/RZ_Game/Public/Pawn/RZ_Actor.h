// RemzDNB

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RZ_Actor.generated.h"

UCLASS()
class RZ_GAME_API ARZ_Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARZ_Actor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
