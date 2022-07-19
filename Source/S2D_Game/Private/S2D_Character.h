/// RemzDNB
///
///	S2D_Character.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "S2D_Game.h"
#include "Actor/RZ_Character.h"
#include "S2D_Character.generated.h"

UCLASS()
class AS2D_Character : public ARZ_Character
{
	GENERATED_BODY()

public:

	AS2D_Character(const FObjectInitializer& ObjectInitializer);
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};


