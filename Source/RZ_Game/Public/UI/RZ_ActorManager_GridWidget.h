// RemzDNB

#pragma once

#include "RZ_Game.h"
#include "RZ_PowerSystem.h"
#include "Blueprint/UserWidget.h"
#include "RZ_ActorManager_GridWidget.generated.h"

class UTextBlock;

UCLASS()
class RZ_GAME_API URZ_ActorManager_GridWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	void Update(ARZ_PowerManager* PowerManagerRef, uint8 GridID);

private:

	UPROPERTY(meta = (BindWidget))
	class URZ_ProgressBarWidget* PowerProgressBar;
	
	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UPanelWidget* ActorContainer;

	//

	ARZ_PowerManager* PowerManager;
	URZ_GameSettings* GameSettings;
	
};
