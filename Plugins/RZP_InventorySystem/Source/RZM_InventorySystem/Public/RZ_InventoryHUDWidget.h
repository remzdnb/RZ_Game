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

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//
	
	UFUNCTION()
	void OnNewInventoryComponent(URZ_InventoryComponent* NewInventoryComp);
	
private:
	
	URZ_InventorySystemModuleSettings* InventorySystemModuleSettings;
	URZ_InventoryComponent* InventoryComp;

	//
	
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* InventorySlotContainer;

	//

	UPROPERTY()
	TArray<URZ_InventorySlotWidget*> InventorySlotWidgets;

	//

	UFUNCTION()
	void CreateSlotWidgets();
	
	UFUNCTION()
	void UpdateSlotWidgets();
};
