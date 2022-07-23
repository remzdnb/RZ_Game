/// RemzDNB
///
/// RZ_OTM_Character_Widget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_BuildingSystem.h"
#include "RZ_PowerSystem.h"
#include "RZ_Game.h"
#include "AI/RZ_AIActionListWidget.h"
#include "Blueprint/UserWidget.h"
#include "RZ_OTM_Character_Widget.generated.h"

class UWidgetSwitcher;

UCLASS()
class URZ_OTM_Character_Widget : public UUserWidget
{
	GENERATED_BODY()

public:

	explicit URZ_OTM_Character_Widget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//

	UFUNCTION()
	void Init(ARZ_PawnAIController* NewAIController);

	UFUNCTION()
	void Update();

	//

	UFUNCTION(BlueprintNativeEvent)
	void ToggleOTM_BPI();
	
private:

	bool bIsExpanded;

	//

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	URZ_AIActionListWidget* ActionListWidget;

	//
	
	ARZ_Pawn* PawnRef;
};
