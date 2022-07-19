/// RemzDNB
///
///	URZ_ItemSlot_Display_Widget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_InventorySystem.h"
//
#include "Blueprint/UserWidget.h"
#include "RZ_ItemSlot_Display_Widget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class RZ_INVENTORYSYSTEM_API URZ_ItemSlot_Display_Widget : public UUserWidget
{
	GENERATED_BODY()

public:

	URZ_ItemSlot_Display_Widget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	//

	UFUNCTION()
	void Update(const FName& ItemNameToDisplayString, int32 StackSizeToDisplay) const;

private:
	
	const URZ_InventorySystemSettings* InventorySystemSettings;
	
	//

	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UImage* ThumbnailImage;

	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UTextBlock* StackSizeText;

	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UImage* StackSizeImage;
};