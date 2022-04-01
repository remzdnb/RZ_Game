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

	// Initialize slots.

	InventorySlotContainer->ClearChildren();
	
	for (uint8 Index = 0; Index < MAXQUICKSLOTS; Index++)
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
	}
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
	InventoryComp = NewInventoryComp;
	InventoryComp->OnInventoryUpdated.AddUniqueDynamic(this, &URZ_InventoryHUDWidget::UpdateInventorySlotWidgets);
	UpdateInventorySlotWidgets();
}

void URZ_InventoryHUDWidget::UpdateInventorySlotWidgets()
{
	if (!InventoryComp) { return; }

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("URZ_InventoryMenuWidget::UpdateInventoryMenuWidget"));;
	
	uint8 Index = 0;
	for (const auto& InventorySlotWidget : InventorySlotWidgets)
	{
		InventorySlotWidgets[Index]->UpdateFromItemSettings(InventoryComp, InventoryComp->GetInventorySlots()[Index]);
		
		Index++;
	}
}
