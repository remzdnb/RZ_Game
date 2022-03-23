/// RemzDNB
///
///	RZ_ItemActorWidget.cpp
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "RZ_ItemActorWidget.h"
/// ItemActorPlugin
#include "RZ_Item.h"
#include "RZM_ItemActor.h"
/// Engine
#include "Components/Image.h"
#include "Components/TextBlock.h"

URZ_ItemActorWidget::URZ_ItemActorWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void URZ_ItemActorWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	ItemActorPluginSettings = Cast<IRZ_ItemActorModuleInterface>(GetGameInstance())->GetItemActorModuleSettings();
}

void URZ_ItemActorWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URZ_ItemActorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void URZ_ItemActorWidget::UpdateFromItemData(const FRZ_ItemData& ItemData)
{
	ItemNameText->SetText(FText::FromString(ItemData.DisplayName.ToString()));

	SlotNameText->SetVisibility(ESlateVisibility::Hidden);
	ItemImage->SetBrushFromTexture(ItemData.ThumbnailTexture);
	ItemImage->SetBrushSize(FVector2D(160.0f, 80.0f));
	
}

void URZ_ItemActorWidget::UpdateFromItemRef(const ARZ_Item* Item) const
{
	if (Item == nullptr)
		return;

	if (Item->GetItemData() == nullptr)
		return;
	
	ItemNameText->SetText(FText::FromString(Item->GetItemData()->DisplayName.ToString()));
	ItemImage->SetBrushFromTexture(Item->GetItemData()->ThumbnailTexture);
}

