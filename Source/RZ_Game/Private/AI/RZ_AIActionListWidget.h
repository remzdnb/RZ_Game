/// RemzDNB
///
/// RZ_AIActionListWidget
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
#include "Blueprint/UserWidget.h"
#include "RZ_AIActionListWidget.generated.h"

class UWidgetSwitcher;
class UProgressBar;
class UTextBlock;
class UImage;

UCLASS()
class URZ_AIActionListWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	explicit URZ_AIActionListWidget(const FObjectInitializer& ObjectInitializer);

	//

	virtual void NativeOnInitialized() override;

	//

	UFUNCTION()
	void Init(ARZ_PawnAIController* NewOwnerPawnAIController);

	UFUNCTION()
	void Update();

	//
	
private:
	
	TWeakObjectPtr<ARZ_PawnAIController> OwnerPawnAIController;
	URZ_GameSettings* GameSettings;

	//

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ActionSlotContainer;
	
};
