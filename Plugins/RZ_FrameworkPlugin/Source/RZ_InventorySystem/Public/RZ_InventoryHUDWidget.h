/// RemzDNB
///
///	RZ_InventoryHUDWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_InventorySystem.h"
#include "Blueprint/UserWidget.h"
#include "RZ_InventoryHUDWidget.generated.h"

class URZ_InventoryComponent;

UCLASS()
class RZ_INVENTORYSYSTEM_API URZ_InventoryHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//
	
	UFUNCTION()
	void OnNewInventoryComponent(URZ_InventoryComponent* NewInventoryComp);
	
private:
	
	const URZ_InventorySystemSettings* InventorySystemSettings;
	URZ_InventoryComponent* InventoryComp;

	//
	
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* QuickSlotsContainer;

	//

	UPROPERTY()
	TArray<URZ_ItemSlot_Inventory_Widget*> QuickSlotsWidgets;

	//

	UFUNCTION()
	void CreateSlotWidgets();
	
	UFUNCTION()
	void UpdateSlotWidgets();
};
