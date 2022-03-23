/// RemzDNB

// ItemUI module
#include "RZ_LoadoutHUDWidget.h"
#include "RZ_ItemActorWidget.h"
#include "RZM_ItemUI.h"
// ItemManager module
#include "RZ_ItemManagerComponent.h"
// Engine
#include "Components/PanelWidget.h"

void URZ_LoadoutHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Get modules settings from interfaced GameInstance.

	ItemActorModuleSettings = Cast<IRZ_ItemActorModuleInterface>(GetGameInstance())->GetItemActorModuleSettings();
	ItemManagerModuleSettings = Cast<IRZ_ItemManagerModuleInterface>(GetGameInstance())->GetItemManagerModuleSettings();
	ItemUIModuleSettings = Cast<IRZ_ItemUIModuleInterface>(GetGameInstance())->GetItemUIModuleSettings();

	// Create and init slots from editor DataTable.

	ItemSlotsContainer->ClearChildren();
	
	for (const auto RowName : ItemManagerModuleSettings->ItemSlotsConfigDT->GetRowNames())
	{
		URZ_ItemActorWidget* ItemActorWidget = CreateWidget<URZ_ItemActorWidget>(
			GetWorld(),
			ItemUIModuleSettings->ItemActorHUDWidgetClass
		);
		if (ItemActorWidget)
		{
			//ItemActorWidget->Ini(*ItemManagerPluginSettings->GetAttachedItemSlotConfigFromRow(RowName));
			ItemSlotsContainer->AddChild(ItemActorWidget);
		}
	}
}

void URZ_LoadoutHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URZ_LoadoutHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	/*if (ItemManagerCT == nullptr)
		return;
	
	ARZ_Character* PossessedCharacter = Cast<ARZ_Character>(GetOwningPlayer()->GetPawn());
	if (PossessedCharacter)
	{
		ItemManagerCT = PossessedCharacter->GetItemManager();
		ItemManagerCT->OnItemEquipped.AddUniqueDynamic(this, &URZ_LoadoutHUDWidget::UpdateSlots);
		UpdateSlots();
	}*/
}

void URZ_LoadoutHUDWidget::UpdateSlots()
{
	if (ItemManagerComp == nullptr)
		return;
	
	ItemSlotsContainer->ClearChildren();

	/*for (const auto GearSlot : ItemManagerCT->GetAttachedSlots())
	{
		URZ_ItemActorWidget* ItemWidget = CreateWidget<URZ_ItemActorWidget>(
			GetWorld(),
			Cast<URZ_GameInstance>(GetGameInstance())->GetGameSettings()->ItemButton_HUD_WidgetClass
		);
		if (ItemWidget)
		{
			ItemWidget->UpdateFromItemRef(GearSlot.AttachedItem);
			SlotsContainer->AddChild(ItemWidget);

			if (ItemManagerCT->GetEquippedItem() == GearSlot.AttachedItem)
			{
				ItemWidget->OnSelectionBPI();
			}
		}
	}*/
}
