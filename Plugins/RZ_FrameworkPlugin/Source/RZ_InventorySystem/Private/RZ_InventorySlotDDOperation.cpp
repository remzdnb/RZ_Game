#include "RZ_InventorySlotDDOperation.h"
#include "RZ_ItemSlot_Inventory_Widget.h"
#include "RZ_ItemSlot_Inventory_Widget.h"

URZ_InventorySlotDDOperation::URZ_InventorySlotDDOperation()
{
}

void URZ_InventorySlotDDOperation::Init(URZ_ItemSlot_Inventory_Widget* NewInventorySlotWidget)
{
	InventorySlotWidget = NewInventorySlotWidget;
}
