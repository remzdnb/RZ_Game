#include "RZ_InventorySlotDDOperation.h"
#include "RZ_InventorySlotWidget.h"

URZ_InventorySlotDDOperation::URZ_InventorySlotDDOperation()
{
}

void URZ_InventorySlotDDOperation::Init(URZ_InventorySlotWidget* NewItemSlotWidget)
{
	ItemSlotWidget = NewItemSlotWidget;
}
