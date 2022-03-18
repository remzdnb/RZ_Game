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
class URZ_LoadoutMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override; // Called once when created.
	virtual void NativeConstruct() override; // Called when added to viewport manually or by a WidgetSwitcher.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; // Called every frame.

	//
	


private:

	class URZ_ItemActorPluginSettings* ItemActorPluginSettings;
	class URZ_ItemManagerPluginSettings* ItemManagerPluginSettings;
	class URZ_ItemUIPluginSettings* ItemUIPluginSettings;

	//
	
	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* SlotTabsContainer;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ItemTabsContainer;

	//

	UFUNCTION()
	void UpdateItemTabs(uint8 TabIndex);
};



