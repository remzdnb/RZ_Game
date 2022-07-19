// RemzDNB

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RZ_Vehicle.generated.h"

UCLASS()
class RZ_GAME_API ARZ_Vehicle : public APawn
{
	GENERATED_BODY()

public:

	ARZ_Vehicle();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
