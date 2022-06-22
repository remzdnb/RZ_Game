/// RemzDNB
///
/// RZ_BuildingOTM_BaseWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Pawn/RZ_Pawn.h"
#include "Blueprint/UserWidget.h"
#include "RZ_BuildingOTM_BaseWidget.generated.h"

class URZ_BuildingOTM_CollapsedWidget;
class URZ_BuildingOTM_ExpandedWidget;

UCLASS()
class RZ_GAME_API URZ_BuildingOTM_BaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	explicit URZ_BuildingOTM_BaseWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;

	//

	UFUNCTION()
	void Init(ARZ_Pawn* NewBuildingRef);

	UFUNCTION(BlueprintNativeEvent)
	void ToggleBuildingOTM_BPI();
	
private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<URZ_BuildingOTM_CollapsedWidget> CollapsedWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<URZ_BuildingOTM_ExpandedWidget> ExpandedWidgetClass;

	//
	
	URZ_BuildingOTM_CollapsedWidget* CollapsedWidget;
	URZ_BuildingOTM_ExpandedWidget* ExpandedWidget;

	//
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	//

	bool bIsExpanded;
};
