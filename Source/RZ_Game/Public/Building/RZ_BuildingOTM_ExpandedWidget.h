/// RemzDNB
///
/// RZ_BuildingOTM_ExpandedWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
#include "RZM_Shared.h"
#include "RZM_PowerSystem.h"
//
#include "Blueprint/UserWidget.h"
#include "RZ_BuildingOTM_ExpandedWidget.generated.h"

class UTextBlock;

UCLASS()
class RZ_GAME_API URZ_BuildingOTM_ExpandedWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	//

	UFUNCTION()
	void Init(ARZ_Pawn* NewBuildingRef);

	UFUNCTION()
	void Update();

private:
	
	ARZ_Pawn* BuildingRef;

	//

	UPROPERTY(VisibleAnywhere, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* BuildingNameText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget, AllowPrivateAccess = "true"))
	URZ_PowerWidget* PowerWidget;
};
