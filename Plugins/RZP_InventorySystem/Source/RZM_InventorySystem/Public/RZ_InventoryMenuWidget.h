/// RemzDNB
///
///	RZ_InventoryMenuWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_InventorySystem.h"
#include "Blueprint/UserWidget.h"
#include "RZ_InventoryMenuWidget.generated.h"

class ARZ_ItemRenderer;

UCLASS()
class RZM_INVENTORYSYSTEM_API URZ_InventoryMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override; // Called once when created.
	virtual void NativeConstruct() override; // Called when added to viewport manually or by a WidgetSwitcher.

	//

	UFUNCTION()
	void OnNewInventoryComponent(URZ_InventoryComponent* NewItemManagerComp);
	
private:

	UFUNCTION()
	void CreateItemSlots();

	UFUNCTION()
	void UpdateItemSlots();

	//
	
	URZ_InventorySystemModuleSettings* InventorySystemModuleSettings;
	URZ_InventoryComponent* InventoryCT;
	ARZ_ItemRenderer* ItemRenderer;

	//
	
	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* InventorySlotContainer;

	UPROPERTY()
	TArray<class URZ_InventorySlotWidget*> InventorySlotWidgets;

};


