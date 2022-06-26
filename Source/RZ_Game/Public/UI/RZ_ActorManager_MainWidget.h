// RemzDNB

#pragma once

#include "Blueprint/UserWidget.h"
#include "RZM_PowerSystem.h"
#include "RZ_Game.h"
#include "RZ_ProgressBarWidget.h"
#include "RZ_ActorManager_MainWidget.generated.h"

UCLASS()
class RZ_GAME_API URZ_ActorManager_MainWidget : public UUserWidget // PawnManager ?
{
	GENERATED_BODY()
	
public:

	virtual void NativeOnInitialized() override;

	//

	UFUNCTION()
	void Update();
	
	//UFUNCTION(BlueprintNativeEvent)
	//void ToggleBuildingOTM_BPI();
	
private:

	ARZ_PowerManager* PowerManager;
	URZ_GameSettings* GameSettings;

	//
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* SwitcherButtonsContainer;
	
	//

	UFUNCTION()
	void OnSwitcherButtonPressed(uint8 ButtonID);

};
