// RemzDNB

#pragma once

#include "S3D_Game.h"
#include "Core/RZ_PlayerController.h"
#include "S3D_PlayerController.generated.h"

UCLASS()
class AS3D_PlayerController : public ARZ_PlayerController
{
	GENERATED_BODY()

public:

	AS3D_PlayerController();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnRep_Pawn() override;

	virtual void OnMenuOpened(bool bNewIsVisible) override;
	
private:

	const US3D_GameSettings* S3DGameSettings;
	
	bool bIsFirstPerson;

	/// Input
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

private:

	virtual void LookUpAxis(float AxisValue) override;
	virtual void LookRightAxis(float AxisValue) override;
	virtual void OnToggleViewKeyPressed() override;
};
