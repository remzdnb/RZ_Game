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

	InventorySystemSettings = Cast<IRZ_InventorySystemModuleInterface>(UGameplayStatics::GetGameInstance(GetWorld()))
		->GetInventorySystemModuleSettings();
}

void URZ_InventoryMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CreateSlots();
}

void URZ_InventoryMenuWidget::OnNewInventoryComponent(URZ_InventoryComponent* NewItemManagerComp)
{
	InventoryComp = NewItemManagerComp;

	CreateSlots();
	
	InventoryComp->OnInventoryUpdated.AddUniqueDynamic(this, &URZ_InventoryMenuWidget::UpdateSlots); // nah
}

void URZ_InventoryMenuWidget::CreateSlots()
{
	if (!InventorySystemSettings) { return; }
	if (!InventoryComp) { return; }
	
	InventorySlotContainer->ClearChildren();
	
	for (int32 Index = 1; Index < MAXINVENTORYSLOTS; Index++)
	{
		URZ_InventorySlotWidget* StorageSlot = CreateWidget<URZ_InventorySlotWidget>(
			GetWorld(),
			InventorySystemSettings->InventorySlot_Menu_WidgetClass
		);
		if (StorageSlot)
		{
			InventorySlotContainer->AddChild(StorageSlot);
			StorageSlotWidgets.Add(StorageSlot);
			StorageSlot->Init(InventoryComp, Index);
		}
	}
}

void URZ_InventoryMenuWidget::UpdateSlots()
{
	for (const auto& InventorySlot : StorageSlotWidgets)
	{
		InventorySlot->Update();
	}
}
