/// RemzDNB
///
///	RZ_ItemActorWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_ItemActor.h"
//
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_ItemActorWidget.generated.h"

class ARZ_Item;

UCLASS()
class RZM_ITEMUI_API URZ_ItemActorWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	URZ_ItemActorWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override; // Called once when created.
	virtual void NativeConstruct() override; // Called when added to viewport manually or by a WidgetSwitcher.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; // Called every frame.

	///
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UFUNCTION()
	void UpdateFromItemData(const FRZ_ItemInfo& ItemData);
	
	UFUNCTION()
	void UpdateFromItemRef(const ARZ_Item* Item) const;

	UFUNCTION(BlueprintImplementableEvent)
	void OnSelectionBPI(bool bIsSelected);

protected:

	/*virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;*/

private:

	UPROPERTY(meta = (BindWidget))
	class USizeBox* MainSizeBox;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage;
	
	UPROPERTY(meta = (BindWidget))
	class UImage* FrameImage;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SlotNameText;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemNameText;
	
	//UPROPERTY(meta = (BindWidget))
	//class UTextBlock* QuantityText;

	//

	class URZ_ItemActorModuleSettings* ItemActorPluginSettings;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName ItemName;
};
