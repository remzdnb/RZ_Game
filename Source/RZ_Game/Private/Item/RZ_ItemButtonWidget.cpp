/// RemzDNB
///
///	RZ_ItemButtonWidget.cpp
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Item/RZ_ItemButtonWidget.h"
/// ItemActorPlugin
#include "RZ_Item.h"
#include "RZM_ItemActor.h"
#include "RZM_ItemActor.h"
#include "RZM_ItemActor.h"
/// Engine
#include "Components/Image.h"
#include "Components/TextBlock.h"

URZ_ItemButtonWidget::URZ_ItemButtonWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void URZ_ItemButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	ItemActorPluginSettings = Cast<IRZ_ItemActorEditorSettingsInterface>(GetGameInstance())->GetItemActorEditorSettings();
}

void URZ_ItemButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URZ_ItemButtonWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void URZ_ItemButtonWidget::UpdateFromItemData(const FRZ_ItemData& ItemData)
{
	ItemNameText->SetText(FText::FromString(ItemData.DisplayName.ToString()));

	SlotNameText->SetVisibility(ESlateVisibility::Hidden);
	ItemImage->SetBrushFromTexture(ItemData.ThumbnailTexture);
	ItemImage->SetBrushSize(FVector2D(160.0f, 80.0f));
	
}

void URZ_ItemButtonWidget::UpdateFromItemRef(const ARZ_Item* Item) const
{
	if (Item == nullptr)
		return;

	if (Item->GetItemData() == nullptr)
		return;
	
	ItemNameText->SetText(FText::FromString(Item->GetItemData()->DisplayName.ToString()));
	ItemImage->SetBrushFromTexture(Item->GetItemData()->ThumbnailTexture);
}

