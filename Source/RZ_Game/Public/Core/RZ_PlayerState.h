#pragma once

#include "RZ_Game.h"
#include "GameFramework/PlayerState.h"
#include "RZ_PlayerState.generated.h"

UCLASS()
class ARZ_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	ARZ_PlayerState();

	virtual void BeginPlay() override;
};
