/// RemzDNB
///
///	URZ_ItemSlot_Container_Widget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_InventorySystem.h"
//
#include "Blueprint/UserWidget.h"
#include "RZ_ItemSlot_Container_Widget.generated.h"

#define MAXCOLUMNS 5

class UImage;
class UTextBlock;

UCLASS()
class RZ_INVENTORYSYSTEM_API URZ_ItemSlot_Container_Widget : public UUserWidget
{
	GENERATED_BODY()

public:

	URZ_ItemSlot_Container_Widget(const FObjectInitializer& ObjectInitializer);
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	//

	UFUNCTION()
	void Update(const URZ_InventoryComponent* InventoryComp, ERZ_InventorySlotType SlotType) const;

private:
	
	const URZ_InventorySystemSettings* InventorySystemSettings;
	
	//

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* SlotContainer;
};