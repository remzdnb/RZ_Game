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

	FORCEINLINE int32 GetSlotID() const { return SlotInfo.SlotID; }

	//
	
	UFUNCTION()
	void Init(URZ_InventoryComponent* NewInventoryComponent, int32 NewSlotID);

	UFUNCTION()
	void Update();

	//

	UFUNCTION(BlueprintImplementableEvent)
	void OnSlotSelectionBPI(bool bNewIsSelected);

	UFUNCTION(BlueprintImplementableEvent)
	void OnQuickBarSelectionBPI(bool bNewIsSelected);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* ActorNameText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* ActorThumbnailImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, OptionalWidget, AllowPrivateAccess = "true"))
	UTextBlock* CornerText;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, OptionalWidget, AllowPrivateAccess = "true"))
	UImage* CornerImage;
	
	//
	
	const URZ_SharedModuleSettings* SharedModuleSettings;
	URZ_InventorySystemModuleSettings* InventorySystemModuleSettings;
	URZ_InventoryComponent* InventoryComp;

	//

	UPROPERTY()
	FRZ_InventorySlotInfo SlotInfo;
	
	//

	UFUNCTION()
	void DebugSlotData();
	
};