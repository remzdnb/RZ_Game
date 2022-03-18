/// RemzDNB
///
///	RZ_LoadoutHUDWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_LoadoutHUDWidget.generated.h"

UCLASS()
class URZ_LoadoutHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override; // Called once when created.
	virtual void NativeConstruct() override; // Called when added to viewport manually or by a WidgetSwitcher.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; // Called every frame.

	///
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* SlotsContainer;

	///

	class ARZ_GameState* GameState;
	class URZ_ItemActorPluginSettings* ItemActorPluginSettings;
	class URZ_ItemManagerPluginSettings* ItemManagerPluginSettings;
	class URZ_ItemUIPluginSettings* ItemUIPluginSettings;
	

	class URZ_ItemManager* ItemManagerCT;
	class ARZ_ItemRenderer* ItemRenderer;

	///

	UFUNCTION()
	void UpdateSlots();
	
	/*UFUNCTION()
	void OnItemSlotMouseEnter(const FName& ItemDataRowName);

	UFUNCTION()
	void OnItemSlotMouseLeave(); */
};
