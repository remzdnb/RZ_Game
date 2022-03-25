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

	CreateActorWidgets();
}

void URZ_LoadoutHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URZ_LoadoutHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	/*if (ItemManagerComp == nullptr)
		return;
	
	ARZ_Character* PossessedCharacter = Cast<ARZ_Character>(GetOwningPlayer()->GetPawn());
	if (PossessedCharacter)
	{
		ItemManagerCT = PossessedCharacter->GetItemManager();
		ItemManagerCT->OnItemEquipped.AddUniqueDynamic(this, &URZ_LoadoutHUDWidget::UpdateSlots);
		UpdateSlots();
	}*/
}

void URZ_LoadoutHUDWidget::OnNewItemManagerComponent(URZ_ItemManagerComponent* NewItemManagerComp)
{
	ItemManagerComp = NewItemManagerComp;
	ItemManagerComp->OnItemEquipped.AddUniqueDynamic(this, &URZ_LoadoutHUDWidget::UpdateActorWidgets);
	UpdateActorWidgets();
}

void URZ_LoadoutHUDWidget::CreateActorWidgets()
{
	// Create and init slots from editor DataTable.

	ItemActorWidgetsContainer->ClearChildren();
	
	for (const auto RowName : ItemManagerModuleSettings->ItemSlotsConfigDT->GetRowNames())
	{
		URZ_ItemActorWidget* ItemActorWidget = CreateWidget<URZ_ItemActorWidget>(
			GetWorld(),
			ItemUIModuleSettings->ItemActorHUDWidgetClass
		);
		if (ItemActorWidget)
		{
			//ItemActorWidget->Ini(*ItemManagerPluginSettings->GetAttachedItemSlotConfigFromRow(RowName));
			ItemActorWidgetsContainer->AddChild(ItemActorWidget);
			ItemActorWidgets.Add(ItemActorWidget);
		}
	}
}

void URZ_LoadoutHUDWidget::UpdateActorWidgets()
{
	if (ItemManagerComp == nullptr)
		return;

	uint8 Index = 0;
	for (const auto ItemActorWidget : ItemActorWidgets)
	{
		ItemActorWidget->UpdateFromItemRef(ItemManagerComp->GetAttachedSlots()[Index].AttachedItem);

		if (ItemManagerComp->GetEquippedItem() == ItemManagerComp->GetAttachedSlots()[Index].AttachedItem)
		{
			ItemActorWidget->OnSelectionBPI(true);
		}
		else
		{
			ItemActorWidget->OnSelectionBPI(false);
		}
		
		Index++;
	}

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
