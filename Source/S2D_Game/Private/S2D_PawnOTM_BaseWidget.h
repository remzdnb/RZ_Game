/// RemzDNB
///
/// S2D_PawnOTM_BaseWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Pawn/RZ_Pawn.h"
#include "Blueprint/UserWidget.h"
#include "S2D_PawnOTM_BaseWidget.generated.h"

class US2D_PawnOTM_CollapsedWidget;
class US2D_PawnOTM_ExpandedWidget;

UCLASS()
class US2D_PawnOTM_BaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	explicit US2D_PawnOTM_BaseWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;

	//

	UFUNCTION()
	void Init(ARZ_Pawn* NewBuildingRef);

	UFUNCTION(BlueprintNativeEvent)
	void ToggleBuildingOTM_BPI();
	
private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<US2D_PawnOTM_CollapsedWidget> CollapsedWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<US2D_PawnOTM_ExpandedWidget> ExpandedWidgetClass;

	//
	
	US2D_PawnOTM_CollapsedWidget* CollapsedWidget;
	US2D_PawnOTM_ExpandedWidget* ExpandedWidget;

	//
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	//

	bool bIsExpanded;
};
