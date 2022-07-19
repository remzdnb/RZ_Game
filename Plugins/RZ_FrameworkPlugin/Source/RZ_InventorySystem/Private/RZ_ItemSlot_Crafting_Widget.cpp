/// RemzDNB

// InventorySystem Plugin
#include "RZ_ItemSlot_Crafting_Widget.h"
#include "RZ_ItemSlot_Display_Widget.h"
#include "RZ_InventoryComponent.h"
// Engine
#include "Components/PanelWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

URZ_ItemSlot_Crafting_Widget::URZ_ItemSlot_Crafting_Widget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void URZ_ItemSlot_Crafting_Widget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	InventorySystemSettings = Cast<IRZ_InventorySystemInterface>(GetWorld()->GetGameInstance())
		->GetInventorySystemSettings();
}

void URZ_ItemSlot_Crafting_Widget::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply URZ_ItemSlot_Crafting_Widget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (OwnerInventoryComp)
	{
		OwnerInventoryComp->StartCraftingItem(ActorToCraftName);	
	}

	//OnCraftingSlotPressed.Broadcast(ActorToCraftName);
	
	return FReply::Handled();
}

void URZ_ItemSlot_Crafting_Widget::Init(URZ_InventoryComponent* NewOwnerInventoryComp, const FName& NewActorToCraftName)
{
	OwnerInventoryComp = NewOwnerInventoryComp;
	ActorToCraftName = NewActorToCraftName;

	Update();
}

void URZ_ItemSlot_Crafting_Widget::Update()
{
	const FRZ_ItemSettings* ActorSettings = InventorySystemSettings->GetItemSettingsFromTableRow(ActorToCraftName);
	if (!ActorSettings) { return; }

	ActorNameText->SetText(FText::FromString(ActorSettings->DisplayName.ToString()));
	ActorThumbnailImage->SetBrushFromTexture(ActorSettings->ThumbnailTexture);
	ActorThumbnailImage->SetRenderScale(FVector2D(ActorSettings->ThumbnailRenderScale));
	ActorThumbnailImage->SetRenderTransformAngle(ActorSettings->ThumbnailRenderAngle);

	if (CraftingComponentSlotsContainer) // ?
	{
		CraftingComponentSlotsContainer->ClearChildren();
	
		for (const auto& CraftingComponent : ActorSettings->CraftingComponents)
		{
			URZ_ItemSlot_Display_Widget* ItemSlotWidget = CreateWidget<URZ_ItemSlot_Display_Widget>(
				GetWorld(),
				InventorySystemSettings->CraftingComponentSlotWidgetClass
			);
			if (ItemSlotWidget)
			{
				CraftingComponentSlotsContainer->AddChild(ItemSlotWidget);
				ItemSlotWidget->Update(CraftingComponent.Key, CraftingComponent.Value);
			}
		}
	}

	if (OwnerInventoryComp)
	{
		if (OwnerInventoryComp->HasItemsInStorage(ActorSettings->CraftingComponents))
		{
			OnCraftableStatusUpdatedBPI(true);
		}
		else
		{
			OnCraftableStatusUpdatedBPI(false);
		}
	}
}
