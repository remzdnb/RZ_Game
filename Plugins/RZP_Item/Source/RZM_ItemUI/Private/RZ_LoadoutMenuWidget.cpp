/// RemzDNB

// ItemUI module
#include "RZ_LoadoutMenuWidget.h"
#include "RZ_ItemActorWidget.h"
#include "RZM_ItemUI.h"
// ItemManager module
#include "RZ_ItemManagerComponent.h"
// ItemActor module
#include "RZM_ItemActor.h"
// UMGTemplates module
#include "RZ_ButtonWidget.h"
// Engine
#include "Components/PanelWidget.h"
#include "Kismet/GameplayStatics.h"

void URZ_LoadoutMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Get module settings from interfaced GameInstance.
	
	ItemActorModuleSettings = Cast<IRZ_ItemActorModuleInterface>(UGameplayStatics::GetGameInstance(GetWorld()))
		->GetItemActorModuleSettings();
	ItemManagerModuleSettings = Cast<IRZ_ItemManagerModuleInterface>(UGameplayStatics::GetGameInstance(GetWorld()))
		->GetItemManagerModuleSettings();
	ItemUIModuleSettings = Cast<IRZ_ItemUIModuleInterface>(UGameplayStatics::GetGameInstance(GetWorld()))
		->GetItemUIModuleSettings();
	
	// Initialize item slots.

	ItemSlotsContainer->ClearChildren();

	uint8 Index = 0;
	for (const auto RowName : ItemManagerModuleSettings->ItemSlotsConfigDT->GetRowNames())
	{
		URZ_ButtonWidget* ItemSlotWidget = CreateWidget<URZ_ButtonWidget>(
			GetWorld(),
			ItemUIModuleSettings->ItemSlotWidgetClass
		);
		if (ItemSlotWidget)
		{
			ItemSlotWidget->Init(Index, RowName);
			ItemSlotWidget->OnButtonPressed.AddUniqueDynamic(this, &URZ_LoadoutMenuWidget::UpdateItemActorsWidgets);
			ItemSlotsContainer->AddChild(ItemSlotWidget);
		}

		Index++;
	}

	// Open first item slot.

	if (true)
	{
		UpdateItemActorsWidgets(0);	
	}
}

void URZ_LoadoutMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URZ_LoadoutMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void URZ_LoadoutMenuWidget::UpdateItemActorsWidgets(uint8 TabIndex)
{
	ItemActorsContainer->ClearChildren();

	for (const auto& RowName : ItemActorModuleSettings->ItemDT->GetRowNames())
	{
		const FRZ_ItemData* ItemData = ItemActorModuleSettings->GetItemDataFromRow(RowName);
		if (ItemData)
		{
			URZ_ItemActorWidget* ItemActorWidget = CreateWidget<URZ_ItemActorWidget>(
				GetWorld(),
				ItemUIModuleSettings->ItemActorMenuWidgetClass
			);
			if (ItemActorWidget)
			{
				ItemActorWidget->UpdateFromItemData(*ItemData);
				ItemActorsContainer->AddChild(ItemActorWidget);
				//TabWidget->OnSlotTabPressed.AddUniqueDynamic(this, &URZ_LoadoutMenuWidget::UpdateItemTabs);
			}
		}
	}
}
