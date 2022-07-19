/// RemzDNB
///
///	InventorySlotWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_InventorySystem.h"
//
#include "Blueprint/UserWidget.h"
#include "RZ_ItemSlot_Inventory_Widget.generated.h"

class UTextBlock;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FInventorySlotWidgetHovered,
	const FRZ_InventorySlotSignature&, SlotSignature,
	bool, bNewIsHovered
);

UCLASS()
class RZ_INVENTORYSYSTEM_API URZ_ItemSlot_Inventory_Widget : public UUserWidget
{
	GENERATED_BODY()

public:

	URZ_ItemSlot_Inventory_Widget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override; // Called once when created.
	virtual void NativeConstruct() override; // Called when added to viewport manually or by a WidgetSwitcher.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	//

	FInventorySlotWidgetHovered OnInventorySlotWidgetHovered;

	FRZ_InventorySlotSignature SlotSignature;

	const URZ_InventorySystemSettings* InventorySystemSettings;
	
	//
	
	void Init(const FRZ_InventorySlotSignature& NewInventorySlotSignature);

	//

	UFUNCTION(BlueprintImplementableEvent)
	void OnSlotInfoUpdatedBPI(const FRZ_InventorySlotData& SlotInfo);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnSlotSelectionBPI(bool bNewIsSelected);

	UFUNCTION(BlueprintImplementableEvent)
	void OnQuickBarSelectionBPI(bool bNewIsSelected);

private:

	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UImage* ThumbnailImage;

	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UPanelWidget* StackSizeContainer;
	
	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UTextBlock* StackSizeText;
	
	//
	
	void Update();
	void DebugSlotInfos();
};