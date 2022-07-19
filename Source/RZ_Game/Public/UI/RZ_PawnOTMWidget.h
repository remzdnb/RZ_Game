/// RemzDNB
///
/// RZ_PawnOTMWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_BuildingSystem.h"
#include "RZ_PowerSystem.h"
#include "RZ_Game.h"
#include "Blueprint/UserWidget.h"
#include "RZ_PawnOTMWidget.generated.h"

class UWidgetSwitcher;
class UProgressBar;
class UTextBlock;
class UImage;

UCLASS()
class URZ_PawnOTMWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	explicit URZ_PawnOTMWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//

	UFUNCTION()
	void Init(ARZ_Pawn* NewPawnRef);

	UFUNCTION()
	void Update();

	//

	UFUNCTION(BlueprintNativeEvent)
	void ToggleBuildingOTM_BPI();
	
private:
	
	ARZ_Pawn* PawnRef;
	URZ_BuildingComponent* BuildingComp;
	URZ_PowerComponent* PowerCompRef;
	
	//

	bool bIsExpanded;

	//

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* Collapsed_BuildingProgressBar;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Collapsed_PowerImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Expanded_PawnNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Expanded_GridIDText;
};
