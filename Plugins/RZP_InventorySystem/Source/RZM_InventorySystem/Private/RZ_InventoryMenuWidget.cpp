/// RemzDNB

// ItemUI module
#include "RZ_InventoryMenuWidget.h"
#include "RZ_InventorySlotWidget.h"
#include "RZM_InventorySystem.h"
// ItemManager module
#include "RZ_InventoryComponent.h"
// Engine
#include "Components/PanelWidget.h"
#include "Kismet/GameplayStatics.h"

void URZ_InventoryMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Get module settings from interfaced GameInstance.

	InventorySystemModuleSettings = Cast<IRZ_InventorySystemModuleInterface>(
			UGameplayStatics::GetGameInstance(GetWorld()))
		->GetInventorySystemModuleSettings();
	
	// Initialize slots.

	InventorySlotContainer->ClearChildren();
	
	for (uint8 Index = 0; Index < MAXINVENTORYSLOTS; Index++)
	{
		URZ_InventorySlotWidget* InventoryItemSlot = CreateWidget<URZ_InventorySlotWidget>(
			GetWorld(),
			InventorySystemModuleSettings->InventorySlot_Menu_WidgetClass
		);
		if (InventoryItemSlot)
		{
			//InventoryItemSlot->Init(Index, "lol");
			//ItemSlotWidget->OnButtonPressed.AddUniqueDynamic(this, &URZ_InventoryMenuWidget::UpdateItemActorsWidgets);
			InventorySlotContainer->AddChild(InventoryItemSlot);
			InventorySlotWidgets.Add(InventoryItemSlot);
		}
	}

	if (true)
	{
		//UpdateItemActorsWidgets(0);	
	}
}

void URZ_InventoryMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URZ_InventoryMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void URZ_InventoryMenuWidget::OnNewInventoryComponent(URZ_InventoryComponent* NewItemManagerComp)
{
	InventoryComp = NewItemManagerComp;
	InventoryComp->OnInventoryUpdated.AddUniqueDynamic(this, &URZ_InventoryMenuWidget::UpdateInventoryMenuWidgets);
	UpdateInventoryMenuWidgets();
}

void URZ_InventoryMenuWidget::UpdateInventoryMenuWidgets()
{
	if (!InventoryComp) { return; }

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("URZ_InventoryMenuWidget::UpdateInventoryMenuWidget"));;
	
	uint8 Index = 0;
	for (auto& InventorySlot : InventoryComp->InventorySlots)
	{
		//GEngine->AddOnScreenDebugMessage(Index, 5.0f, FColor::Green, *FString::Printf(TEXT("%s"), *InventorySlot.ItemName.ToString()));

		InventorySlotWidgets[Index]->UpdateFromItemSettings(InventoryComp, InventorySlot);
		
		Index++;
	}
	
	/*ItemActorsContainer->ClearChildren();

	for (const auto& RowName : ItemActorModuleSettings->ItemInfoDT->GetRowNames())
	{
		const FRZ_ItemInfo* ItemData = ItemActorModuleSettings->GetItemInfoFromRow(RowName);
		if (ItemData)
		{
			URZ_InventorySlotWidget* ItemActorWidget = CreateWidget<URZ_InventorySlotWidget>(
				GetWorld(),
				ItemUIModuleSettings->ItemActorMenuWidgetClass
			);
			if (ItemActorWidget)
			{
				ItemActorWidget->UpdateFromItemData(*ItemData);
				ItemActorsContainer->AddChild(ItemActorWidget);
				//TabWidget->OnSlotTabPressed.AddUniqueDynamic(this, &URZ_InventoryMenuWidget::UpdateItemTabs);
			}
		}
	}*/
}
