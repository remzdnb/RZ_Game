// RemzDNB

#pragma once

#include "CoreMinimal.h"
#include "Player/RZ_PlayerController.h"
#include "TD_PlayerController.generated.h"

UCLASS()
class ATD_PlayerController : public ARZ_PlayerController
{
	GENERATED_BODY()

public:

	ATD_PlayerController();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void OnRep_Pawn() override;
};
