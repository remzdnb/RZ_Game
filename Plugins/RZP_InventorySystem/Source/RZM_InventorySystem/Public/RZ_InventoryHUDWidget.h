/// RemzDNB
///
///	RZ_InventoryHUDWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_InventorySystem.h"
#include "Blueprint/UserWidget.h"
#include "RZ_InventoryHUDWidget.generated.h"

class URZ_InventoryComponent;
class URZ_InventorySlotWidget;

UCLASS()
class RZM_INVENTORYSYSTEM_API URZ_InventoryHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override; // Called once when created.
	virtual void NativeConstruct() override; // Called when added to viewport manually or by a WidgetSwitcher.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; // Called every frame.

public:

	UFUNCTION() // Must be called from player controller when a new ItemManager owner is spawned.
	void OnNewInventoryComponent(URZ_InventoryComponent* NewInventoryComp);
	
private:

	UFUNCTION()
	void UpdateInventorySlotWidgets();

	//
	
	URZ_InventorySystemModuleSettings* InventorySystemModuleSettings;
	URZ_InventoryComponent* InventoryComp;

	//
	
	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* InventorySlotContainer;

	UPROPERTY()
	TArray<URZ_InventorySlotWidget*> InventorySlotWidgets;
};
