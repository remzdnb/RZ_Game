/// RemzDNB

#include "RZ_InventoryHUDWidget.h"
#include "RZ_ItemSlot_Inventory_Widget.h"
#include "RZ_InventoryComponent.h"
// Engine
#include "Components/PanelWidget.h"
#include "Kismet/GameplayStatics.h"

void URZ_InventoryHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InventorySystemSettings = Cast<IRZ_InventorySystemInterface>(UGameplayStatics::GetGameInstance(GetWorld()))
		->GetInventorySystemSettings();
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
	if (!NewInventoryComp) { return; }
	
	InventoryComp = NewInventoryComp;

	CreateSlotWidgets();
}

void URZ_InventoryHUDWidget::CreateSlotWidgets()
{
	if (!InventorySystemSettings) { return; }
	if (!InventoryComp) { return; }
	
	QuickSlotsContainer->ClearChildren();
	QuickSlotsWidgets.Empty();

	TArray<FRZ_InventorySlotData> SlotArray;
	InventoryComp->GetSlotsFromQuickBar(InventoryComp->GetSelectedQuickBarID(), SlotArray);
	for (const auto& CurrentSlot : SlotArray)
	{
		URZ_ItemSlot_Inventory_Widget* QuickSlot = CreateWidget<URZ_ItemSlot_Inventory_Widget>(
			GetWorld(),
			InventorySystemSettings->QuickSlotWidgetClass
		);
		if (QuickSlot)
		{
			QuickSlotsContainer->AddChild(QuickSlot);
			QuickSlotsWidgets.Add(QuickSlot);
			QuickSlot->Init(CurrentSlot.InventorySlotSignature);
		}
	}

	//UpdateSlotWidgets();
}

void URZ_InventoryHUDWidget::UpdateSlotWidgets()
{
	if (!InventoryComp) { return; }

	//TArray<FRZ_InventorySlotData> QuickSlotsArray;
	//InventoryComp->GetSlotsFromQuickBar(InventoryComp->GetSelectedQuickBarID(), QuickSlotsArray);

	int32 Index = 0;
	for (auto& QuickSlot : QuickSlotsWidgets)
	{
		//QuickSlot->Upda
	}
}
