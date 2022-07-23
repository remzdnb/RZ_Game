/// RemzDNB

#include "RZ_ItemSlot_Container_Widget.h"
#include "RZ_ItemSlot_Inventory_Widget.h"
#include "RZ_InventoryComponent.h"
// Engine
#include "Components/PanelWidget.h"
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridSlot.h"

URZ_ItemSlot_Container_Widget::URZ_ItemSlot_Container_Widget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void URZ_ItemSlot_Container_Widget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	InventorySystemSettings = Cast<IRZ_InventorySystemInterface>(GetWorld()->GetGameInstance())
		->GetInventorySystemSettings();
}

void URZ_ItemSlot_Container_Widget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URZ_ItemSlot_Container_Widget::Update(const URZ_InventoryComponent* InventoryComp, ERZ_InventorySlotType SlotType) const
{
	SlotContainer->ClearChildren();
	
	if (SlotType == ERZ_InventorySlotType::Storage)
	{
		for (const auto& StorageSlot : InventoryComp->GetStorageSlots())
		{
			if (StorageSlot.InventorySlotSignature.SlotID == 0)
			{
				continue;
			}
			
			int32 NewSlotID = StorageSlot.InventorySlotSignature.SlotID - 1;
			
			URZ_ItemSlot_Inventory_Widget* StorageSlotWidget = CreateWidget<URZ_ItemSlot_Inventory_Widget>(
				GetWorld(),
				InventorySystemSettings->StorageSlotWidgetClass
			);
			if (StorageSlotWidget)
			{
				const int32 ColumnID = NewSlotID % MAXCOLUMNS;
				const int32 RowID = NewSlotID / MAXCOLUMNS;
				SlotContainer->AddChild(StorageSlotWidget);
				SlotContainer->AddChildToUniformGrid(StorageSlotWidget, RowID, ColumnID);
				StorageSlotWidget->Init(StorageSlot.InventorySlotSignature);
				
				//StorageSlotWidget->OnInventorySlotWidgetHovered.AddUniqueDynamic(this, &URZ_InventoryMenuWidget::OnStorageSlotHovered);
			}
		}
	}

	if (SlotType == ERZ_InventorySlotType::Attached)
	{
		for (const auto& AttachedSlot : InventoryComp->GetAttachedSlots())
		{
			URZ_ItemSlot_Inventory_Widget* AttachedSlotWidget = CreateWidget<URZ_ItemSlot_Inventory_Widget>(
				GetWorld(),
				InventorySystemSettings->AttachedSlotWidgetClass
			);
			if (AttachedSlotWidget)
			{
				SlotContainer->AddChildToUniformGrid(AttachedSlotWidget, 0, AttachedSlot.InventorySlotSignature.SlotID);
				AttachedSlotWidget->Init(AttachedSlot.InventorySlotSignature);

				//StorageSlotWidget->OnInventorySlotWidgetHovered.AddUniqueDynamic(this, &URZ_InventoryMenuWidget::OnStorageSlotHovered);
			}
		}
	}
}
