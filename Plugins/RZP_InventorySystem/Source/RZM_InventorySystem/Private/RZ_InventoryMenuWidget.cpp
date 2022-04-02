/// RemzDNB

// InventorySystem plugin
#include "RZ_InventoryMenuWidget.h"
#include "RZ_InventorySlotWidget.h"
#include "RZ_InventoryComponent.h"
// Engine
#include "Components/PanelWidget.h"
#include "Kismet/GameplayStatics.h"

void URZ_InventoryMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InventorySystemModuleSettings = Cast<IRZ_InventorySystemModuleInterface>(
			UGameplayStatics::GetGameInstance(GetWorld()))
		->GetInventorySystemModuleSettings();
}

void URZ_InventoryMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateItemSlots();
}

void URZ_InventoryMenuWidget::OnNewInventoryComponent(URZ_InventoryComponent* NewItemManagerComp)
{
	InventoryCT = NewItemManagerComp;
	InventoryCT->OnInventoryUpdated.AddUniqueDynamic(this, &URZ_InventoryMenuWidget::UpdateItemSlots); // nah
	CreateItemSlots();
}

void URZ_InventoryMenuWidget::CreateItemSlots()
{
	if (!InventorySystemModuleSettings) { return; }
	if (!InventoryCT) { return; }
	
	InventorySlotContainer->ClearChildren();
	
	for (uint8 Index = 0; Index < MAXINVENTORYSLOTS; Index++)
	{
		URZ_InventorySlotWidget* InventoryItemSlot = CreateWidget<URZ_InventorySlotWidget>(
			GetWorld(),
			InventorySystemModuleSettings->InventorySlot_Menu_WidgetClass
		);
		if (InventoryItemSlot)
		{
			InventorySlotContainer->AddChild(InventoryItemSlot);
			InventorySlotWidgets.Add(InventoryItemSlot);
			
			InventoryItemSlot->Init(InventoryCT, Index);
			//ItemSlotWidget->OnButtonPressed.AddUniqueDynamic(this, &URZ_InventoryMenuWidget::UpdateItemActorsWidgets);
		}
	}
}

void URZ_InventoryMenuWidget::UpdateItemSlots()
{
	for (const auto& InventorySlot : InventorySlotWidgets)
	{
		InventorySlot->Update();
	}
}
