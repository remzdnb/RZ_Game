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
class RZM_ITEMUI_API URZ_LoadoutHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override; // Called once when created.
	virtual void NativeConstruct() override; // Called when added to viewport manually or by a WidgetSwitcher.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; // Called every frame.

	///
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:

	UFUNCTION()
	void UpdateSlots();

	//
	
	class URZ_ItemActorModuleSettings* ItemActorModuleSettings;
	class URZ_ItemManagerModuleSettings* ItemManagerModuleSettings;
	class URZ_ItemUIModuleSettings* ItemUIModuleSettings;
	class URZ_ItemManagerComponent* ItemManagerComp;
	class ARZ_ItemRenderer* ItemRenderer;

	//

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ItemSlotsContainer;

	//
	
	/*UFUNCTION()
	void OnItemSlotMouseEnter(const FName& ItemDataRowName);

	UFUNCTION()
	void OnItemSlotMouseLeave(); */
};
