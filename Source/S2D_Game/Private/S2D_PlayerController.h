/// RemzDNB
///
///	S2D_PlayerController.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "S2D_Character.h"
#include "S2D_Game.h"
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

	//
	
	virtual void OnMenuOpened(bool bNewIsVisible) override;

	// override settargetlocation
	
	virtual void OnLookUpKeyPressed() override;
	virtual void OnLookDownKeyPressed() override;
	virtual void OnLookRightKeyPressed() override;
	virtual void OnLookLeftKeyPressed() override;
	virtual void OnLeftMouseButtonPressed() override;
	virtual void OnLeftMouseButtonReleased() override;
	virtual void OnRKeyPressed() override;

private:

	//

	TWeakObjectPtr<AS2D_Character> S2DCharacter;
	
	FInputModeGameAndUI DefaultInputMode;
};
