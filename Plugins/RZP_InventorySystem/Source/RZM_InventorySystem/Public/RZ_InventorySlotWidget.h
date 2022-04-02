/// RemzDNB
///
///	InventorySlotWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_InventorySystem.h"
//
#include "Blueprint/UserWidget.h"
#include "RZ_InventorySlotWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class RZM_INVENTORYSYSTEM_API URZ_InventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	URZ_InventorySlotWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override; // Called once when created.
	virtual void NativeConstruct() override; // Called when added to viewport manually or by a WidgetSwitcher.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; // Called every frame.

	//
	
	UFUNCTION()
	void Init(URZ_InventoryComponent* NewInventoryComponent, int32 NewSlotID);

	UFUNCTION()
	void Update();

	//

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateBPI(bool bIsOnQuickBar, bool bIsValidItem, bool bIsItemEquipped = false);

	//
	
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	//

	FORCEINLINE int32 GetSlotID() const { return SlotID; }

private:

	UFUNCTION()
	void DebugSlotData();

	//

	const URZ_SharedModuleSettings* SharedModuleSettings;
	URZ_InventorySystemModuleSettings* InventorySystemModuleSettings;
	URZ_InventoryComponent* InventoryComponent;
	int32 SlotID;
	FRZ_InventorySlotInfo SlotInfo;

	//
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* ItemImage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* ItemNameText;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* CornerText;
	
};