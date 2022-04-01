#pragma once

#include "RZM_InventorySystem.h"
//
#include "Blueprint/DragDropOperation.h"
#include "RZ_InventorySlotDDOperation.generated.h"

class URZ_InventorySlotWidget;

UCLASS()
class RZM_INVENTORYSYSTEM_API URZ_InventorySlotDDOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:

	URZ_InventorySlotDDOperation();

	void Init(URZ_InventorySlotWidget* NewItemSlotWidget);

	//

	UPROPERTY()
	URZ_InventorySlotWidget* ItemSlotWidget;
};
