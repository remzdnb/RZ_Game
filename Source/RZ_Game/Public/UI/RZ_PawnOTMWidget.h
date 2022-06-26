/// RemzDNB
///
/// RZ_PawnOTMWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Pawn/RZ_Pawn.h"
#include "Blueprint/UserWidget.h"
#include "RZ_PawnOTMWidget.generated.h"

class US2D_PawnOTM_CollapsedWidget;
class US2D_PawnOTM_ExpandedWidget;
class UTextBlock;

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
	class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* Expanded_PawnNameText;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* Expanded_GridIDText;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	UPanelWidget* Expanded_ConnectedPowerCompsPanel;

	//

	ARZ_Pawn* PawnRef;

	bool bIsExpanded;
};
