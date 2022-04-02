/// RemzDNB

// ItemUI module
#include "RZ_InventoryHUDWidget.h"
#include "RZ_InventorySlotWidget.h"
#include "RZM_InventorySystem.h"
// ItemManager module
#include "RZ_InventoryComponent.h"
// Engine
#include "Components/PanelWidget.h"
#include "Kismet/GameplayStatics.h"

void URZ_InventoryHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Get module settings from interfaced GameInstance.

	InventorySystemModuleSettings = Cast<IRZ_InventorySystemModuleInterface>(
			UGameplayStatics::GetGameInstance(GetWorld()))
		->GetInventorySystemModuleSettings();
}

void URZ_InventoryHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URZ_InventoryHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void URZ_InventoryHUDWidget::OnNewInventoryComponent(URZ_InventoryComponent* NewInventoryComp)
{
	InventoryCT = NewInventoryComp;
	InventoryCT->OnQuickBarSelected.AddUniqueDynamic(this, &URZ_InventoryHUDWidget::CreateSlotWidgets);
	InventoryCT->OnInventoryUpdated.AddUniqueDynamic(this, &URZ_InventoryHUDWidget::UpdateSlotWidgets);
	CreateSlotWidgets();
}

void URZ_InventoryHUDWidget::CreateSlotWidgets()
{
	if (!InventorySystemModuleSettings) { return; }
	if (!InventoryCT) { return; }
	
	InventorySlotContainer->ClearChildren();

	TArray<FRZ_InventorySlotInfo> SlotArray;
	InventoryCT->GetSlotsFromQuickBar(InventoryCT->GetSelectedQuickBarID(), SlotArray);
	for (const auto& ItrSlot : SlotArray)
	{
		URZ_InventorySlotWidget* InventoryItemSlot = CreateWidget<URZ_InventorySlotWidget>(
			GetWorld(),
			InventorySystemModuleSettings->InventorySlot_HUD_WidgetClass
		);
		if (InventoryItemSlot)
		{
			InventorySlotContainer->AddChild(InventoryItemSlot);
			InventorySlotWidgets.Add(InventoryItemSlot);

			InventoryItemSlot->Init(InventoryCT, ItrSlot.SlotID);
		}
	}

	/*for (int32 Index = InventoryCT->GetSelectedQuickBarID() * MAXQUICKSLOTS;
	     Index < InventoryCT->GetSelectedQuickBarID() * MAXQUICKSLOTS + MAXQUICKSLOTS;
	     Index++)
	{
		URZ_InventorySlotWidget* InventoryItemSlot = CreateWidget<URZ_InventorySlotWidget>(
			GetWorld(),
			InventorySystemModuleSettings->InventorySlot_HUD_WidgetClass
		);
		if (InventoryItemSlot)
		{
			//InventoryItemSlot->Init(Index, "lol");
			//ItemSlotWidget->OnButtonPressed.AddUniqueDynamic(this, &URZ_InventoryMenuWidget::UpdateItemActorsWidgets);
			InventorySlotContainer->AddChild(InventoryItemSlot);
			InventorySlotWidgets.Add(InventoryItemSlot);
		}
	}*/

	UpdateSlotWidgets();
}

void URZ_InventoryHUDWidget::UpdateSlotWidgets()
{
	if (!InventoryCT) { return; }

	for (const auto& InventorySlotWidget : InventorySlotWidgets)
	{
		InventorySlotWidget->Update();
	}

	/*for (int32 Index = InventoryCT->GetSelectedQuickBarID() * MAXQUICKSLOTS;
	     Index < InventoryCT->GetSelectedQuickBarID() * MAXQUICKSLOTS + MAXQUICKSLOTS;
	     Index++)
	{
		InventorySlotWidgets[Index]->UpdateFromInventorySlot(InventoryCT, Index);
	}*/
}
