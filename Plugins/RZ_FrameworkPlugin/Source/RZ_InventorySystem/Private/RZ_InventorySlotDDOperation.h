#pragma once

#include "RZ_InventorySystem.h"
//
#include "Blueprint/DragDropOperation.h"
#include "RZ_InventorySlotDDOperation.generated.h"

UCLASS()
class RZ_INVENTORYSYSTEM_API URZ_InventorySlotDDOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:

	URZ_InventorySlotDDOperation();

	void Init(class URZ_ItemSlot_Inventory_Widget* NewInventorySlotWidget);

	//

	UPROPERTY()
	URZ_ItemSlot_Inventory_Widget* InventorySlotWidget;
};
