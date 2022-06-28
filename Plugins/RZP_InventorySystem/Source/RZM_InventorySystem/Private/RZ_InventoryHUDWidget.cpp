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
	if (!NewInventoryComp) { return; }
	
	InventoryComp = NewInventoryComp;

	CreateSlotWidgets();
	
	InventoryComp->OnInventoryUpdated.AddUniqueDynamic(this, &URZ_InventoryHUDWidget::UpdateSlotWidgets);
}

void URZ_InventoryHUDWidget::CreateSlotWidgets()
{
	if (!InventorySystemModuleSettings) { return; }
	if (!InventoryComp) { return; }
	
	InventorySlotContainer->ClearChildren();

	TArray<FRZ_InventorySlotInfo> SlotArray;
	InventoryComp->GetSlotsFromQuickBar(InventoryComp->GetSelectedQuickBarID(), SlotArray);
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

			InventoryItemSlot->Init(InventoryComp, ItrSlot.SlotID);
		}
	}

	UpdateSlotWidgets();
}

void URZ_InventoryHUDWidget::UpdateSlotWidgets()
{
	if (!InventoryComp) { return; }

	TArray<FRZ_InventorySlotInfo> SlotInfos;
	InventoryComp->GetSlotsFromQuickBar(InventoryComp->GetSelectedQuickBarID(), SlotInfos);

	int32 Index = 0;
	for (const auto& SlotInfo : SlotInfos)
	{
		InventorySlotWidgets[Index]->Init(InventoryComp, SlotInfo.SlotID);
		Index++;
	}
}
