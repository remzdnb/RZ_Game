// RemzDNB

#pragma once

#include "CoreMinimal.h"
#include "Core/RZ_PlayerController.h"
#include "S2D_PlayerController.generated.h"

#define DEFAULTCONTROLSETTINGSPRESETNAME "AttachedTD"

UCLASS()
class AS2D_PlayerController : public ARZ_PlayerController
{
	GENERATED_BODY()

public:

	AS2D_PlayerController();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnRep_Pawn() override;
	
	virtual void OnMenuOpened(bool bNewIsVisible) override;

	//

	virtual void OnLookUpKeyPressed() override;
	virtual void OnLookDownKeyPressed() override;
	virtual void OnLookRightKeyPressed() override;
	virtual void OnLookLeftKeyPressed() override;

private:

	FInputModeGameAndUI DefaultInputMode;
};
