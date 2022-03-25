/// RemzDNB
///
///	RZ_LoadoutHUDWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_LoadoutHUDWidget.generated.h"

class URZ_ItemActorModuleSettings;
class URZ_ItemManagerModuleSettings;
class URZ_ItemUIModuleSettings;
class URZ_ItemManagerComponent;
class URZ_ItemActorWidget;

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

public:

	UFUNCTION() // Must be called from player controller when a new ItemManager owner is spawned.
	void OnNewItemManagerComponent(URZ_ItemManagerComponent* NewItemManagerComp);
	
private:

	UFUNCTION()
	void CreateActorWidgets();

	UFUNCTION()
	void UpdateActorWidgets();

	//
	
	URZ_ItemActorModuleSettings* ItemActorModuleSettings;
	URZ_ItemManagerModuleSettings* ItemManagerModuleSettings;
	URZ_ItemUIModuleSettings* ItemUIModuleSettings;
	URZ_ItemManagerComponent* ItemManagerComp;
	TArray<URZ_ItemActorWidget*> ItemActorWidgets;
	
	//

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ItemActorWidgetsContainer;
	
	//
	
	/*UFUNCTION()
	void OnItemSlotMouseEnter(const FName& ItemDataRowName);

	UFUNCTION()
	void OnItemSlotMouseLeave(); */
};
