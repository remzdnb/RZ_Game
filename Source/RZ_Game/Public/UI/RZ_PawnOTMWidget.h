/// RemzDNB
///
/// RZ_PawnOTMWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
#include "RZ_PowerComponent.h"
#include "Blueprint/UserWidget.h"
#include "RZ_PawnOTMWidget.generated.h"

class UWidgetSwitcher;
class UTextBlock;
class UImage;

UCLASS()
class URZ_PawnOTMWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	explicit URZ_PawnOTMWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;

	//

	UFUNCTION()
	void Init(ARZ_Pawn* NewPawnRef);

	UFUNCTION()
	void Update();

	UFUNCTION(BlueprintNativeEvent)
	void ToggleBuildingOTM_BPI();
	
private:
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	UImage* Collapsed_PowerImage;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* Expanded_PawnNameText;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* Expanded_GridIDText;

	//

	ARZ_Pawn* PawnRef;
	URZ_PowerComponent* PowerCompRef;

	//

	bool bIsExpanded;
};
