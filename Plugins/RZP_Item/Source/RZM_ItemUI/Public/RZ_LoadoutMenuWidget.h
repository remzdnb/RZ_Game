/// RemzDNB
///
///	RZ_LoadoutMenuWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_LoadoutMenuWidget.generated.h"

UCLASS()
class RZM_ITEMUI_API URZ_LoadoutMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override; // Called once when created.
	virtual void NativeConstruct() override; // Called when added to viewport manually or by a WidgetSwitcher.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; // Called every frame.

private:

	UFUNCTION()
	void UpdateItemActorsWidgets(uint8 TabIndex);

	//
	
	class URZ_ItemActorModuleSettings* ItemActorModuleSettings;
	class URZ_ItemManagerModuleSettings* ItemManagerModuleSettings;
	class URZ_ItemUIModuleSettings* ItemUIModuleSettings;

	//
	
	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ItemSlotsContainer;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ItemActorsContainer;

};



