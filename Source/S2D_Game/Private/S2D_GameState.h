// RemzDNB

#pragma once

#include "S2D_Game.h"
#include "Core/RZ_GameState.h"
#include "S2D_GameState.generated.h"

UCLASS()
class AS2D_GameState : public ARZ_GameState
{
	GENERATED_BODY()
	
public:

	AS2D_GameState();

	virtual void BeginPlay() override;

	//
	
	const US2D_GameSettings* GetS2DGameSettings() const { return S2DGameSettings; }

private :

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AS2D_GameState", meta = (AllowPrivateAccess = true))
	US2D_GameSettings* S2DGameSettings;
};
