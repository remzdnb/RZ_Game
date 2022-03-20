/// RemzDNB
///
///	RZ_LoadoutHUDWidget.cpp
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Item/RZ_LoadoutHUDWidget.h"
#include "Item/RZ_ItemButtonWidget.h"
#include "Character/RZ_Character.h"
#include "Game/RZ_GameInstance.h"
#include "Game/RZ_GameState.h"
#include "Game/RZ_GameSettings.h"
/// ItemManager Plugin
#include "RZ_ItemManagerComponent.h"
#include "RZ_ItemManagerComponent.h"
//
#include "GameFramework/GameStateBase.h"
#include "Components/PanelWidget.h"

void URZ_LoadoutHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	/// Get settings data from interfaced GameState.
	
	ItemManagerPluginSettings = Cast<IRZ_ItemManagerEditorSettingsInterface>(GetGameInstance())->GetItemManagerEditorSettings();
	
	//IRZ_ItemUIPluginSettingsInterface* Interface_02 = Cast<IRZ_ItemUIPluginSettingsInterface>(GameState);
	//ItemUIPluginSettings = Cast<URZ_ItemUIPluginSettings>(Interface_02->GetItemUIPluginSettings());

	/// Create SlotWidgets from DataTable.

	/*ItemSlotsContainer->ClearChildren();
	
	for (const auto RowName : ItemManagerPluginSettings->AttachedItemSlotsConfigsDT->GetRowNames())
	{
		URZ_ItemSlotWidget* ItemSlotWidget = CreateWidget<URZ_ItemSlotWidget>(
			GetWorld(),
			ItemUIPluginSettings->ItemSlotWidgetClass
		);
		if (ItemSlotWidget)
		{
			ItemSlotWidget->InitSlot(*ItemManagerPluginSettings->GetAttachedItemSlotConfigFromRow(RowName));
			ItemSlotsContainer->AddChild(ItemSlotWidget);
		}
	}*/
}

void URZ_LoadoutHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URZ_LoadoutHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (ItemManagerCT == nullptr)
		return;
	
	ARZ_Character* PossessedCharacter = Cast<ARZ_Character>(GetOwningPlayer()->GetPawn());
	if (PossessedCharacter)
	{
		ItemManagerCT = PossessedCharacter->GetItemManager();
		ItemManagerCT->OnItemEquipped.AddUniqueDynamic(this, &URZ_LoadoutHUDWidget::UpdateSlots);
		UpdateSlots();
	}
}

void URZ_LoadoutHUDWidget::UpdateSlots()
{
	if (ItemManagerCT == nullptr)
		return;
	
	SlotsContainer->ClearChildren();

	for (const auto GearSlot : ItemManagerCT->GetAttachedSlots())
	{
		URZ_ItemButtonWidget* ItemWidget = CreateWidget<URZ_ItemButtonWidget>(
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
	}
}
