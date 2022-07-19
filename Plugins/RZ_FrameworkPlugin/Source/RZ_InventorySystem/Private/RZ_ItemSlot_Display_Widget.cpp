/// RemzDNB

//
#include "RZ_ItemSlot_Display_Widget.h"
#include "RZ_InventoryComponent.h"
// Engine
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

URZ_ItemSlot_Display_Widget::URZ_ItemSlot_Display_Widget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void URZ_ItemSlot_Display_Widget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	InventorySystemSettings = Cast<IRZ_InventorySystemInterface>(GetWorld()->GetGameInstance())
		->GetInventorySystemSettings();
}

void URZ_ItemSlot_Display_Widget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URZ_ItemSlot_Display_Widget::Update(const FName& ItemNameToDisplayString, int32 StackSizeToDisplay) const
{
	if (ItemNameToDisplayString == EMPTYITEMSLOTNAME)
	{
		if (NameText) { NameText->SetVisibility(ESlateVisibility::Hidden); }
		if (ThumbnailImage) { ThumbnailImage->SetVisibility(ESlateVisibility::Hidden); }
		if (StackSizeText) { StackSizeText->SetVisibility(ESlateVisibility::Hidden); }
		if (StackSizeImage) { StackSizeImage->SetVisibility(ESlateVisibility::Hidden); }
		
		return;
	}
	
	const FRZ_ItemSettings* ActorSettings = InventorySystemSettings->GetItemSettingsFromTableRow(ItemNameToDisplayString);
	if (!ActorSettings) { return; }

	if (NameText)
	{
		NameText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		NameText->SetText(FText::FromString(ActorSettings->DisplayName.ToString()));
	}

	if (ThumbnailImage)
	{
		ThumbnailImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ThumbnailImage->SetBrushFromTexture(ActorSettings->ThumbnailTexture);
		ThumbnailImage->SetRenderScale(FVector2D(ActorSettings->ThumbnailRenderScale));
		ThumbnailImage->SetRenderTransformAngle(ActorSettings->ThumbnailRenderAngle);
	}

	if (StackSizeText)
	{
		StackSizeText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		StackSizeText->SetText(FText::FromString(FString::FromInt(StackSizeToDisplay)));
	}

	if (StackSizeImage)
	{
		StackSizeImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}
