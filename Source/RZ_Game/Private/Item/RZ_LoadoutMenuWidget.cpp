/// RemzDNB
///
///	RZ_LoadoutMenuWidget.cpp
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Item/RZ_LoadoutMenuWidget.h"
#include "Item/RZ_ItemButtonWidget.h"
#include "Game/RZ_GameInstance.h"
/// ItemManagerPlugin
#include "RZ_ItemManagerPluginInterfaces.h"
#include "RZ_ItemManagerPluginSettings.h"
/// ItemActorPlugin
#include "RZ_ItemActorPluginSettings.h"
/// Engine
#include "Components/PanelWidget.h"

void URZ_LoadoutMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	/// Get settings data from interfaced GameState.
	
	ItemManagerPluginSettings = Cast<URZ_GameInstance>(GetGameInstance())->GetItemManagerPluginSettings();
	ItemActorPluginSettings = Cast<URZ_GameInstance>(GetGameInstance())->GetItemActorPluginSettings();
	
	/// Initialize slot tabs.

	SlotTabsContainer->ClearChildren();

	/*uint8 Index = 0;
	for (const auto RowName : ItemManagerPluginSettings->AttachedItemSlotsConfigsDT->GetRowNames())
	{
		URZ_SlotTabWidget* TabWidget = CreateWidget<URZ_SlotTabWidget>(
			GetWorld(),
			ItemUIPluginSettings->SlotTabWidgetClass
		);
		if (TabWidget)
		{
			TabWidget->Init(Index, *ItemManagerPluginSettings->GetAttachedItemSlotConfigFromRow(RowName));
			TabWidget->OnSlotTabPressed.AddUniqueDynamic(this, &URZ_LoadoutMenuWidget::UpdateItemTabs);
			SlotTabsContainer->AddChild(TabWidget);
		}

		Index++;
	}*/
}

void URZ_LoadoutMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URZ_LoadoutMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void URZ_LoadoutMenuWidget::UpdateItemTabs(uint8 TabIndex)
{
   /* ItemTabsContainer->ClearChildren();
    
	for (const auto& RowName : ItemActorPluginSettings->ItemDT->GetRowNames())
	{
		const FRZ_ItemData* ItemData = ItemActorPluginSettings->GetItemDataFromRow(RowName);
		if (ItemData)
		{
			URZ_ItemButtonWidget* ItemTabWidget = CreateWidget<URZ_ItemButtonWidget>(
				GetWorld(),
				ItemUIPluginSettings->ItemTabWidgetClass
			);
			if (ItemTabWidget)
			{
				ItemTabWidget->UpdateFromItemData(*ItemData);
				ItemTabsContainer->AddChild(ItemTabWidget);
				//TabWidget->OnSlotTabPressed.AddUniqueDynamic(this, &URZ_LoadoutMenuWidget::UpdateItemTabs);
			}
		}
	}*/
}
