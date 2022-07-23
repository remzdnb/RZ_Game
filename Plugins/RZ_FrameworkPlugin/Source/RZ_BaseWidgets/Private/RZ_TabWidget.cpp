/// RemzDNB

#include "RZ_TabWidget.h"
//
#include "Components/TextBlock.h"

void URZ_TabWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bIsSelected = false;
	bIsHovered = false;
}

FReply URZ_TabWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	OnTabPressed.Broadcast(TabID);
	
	return FReply::Handled();
}

void URZ_TabWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	bIsHovered = true;
}

void URZ_TabWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bIsHovered = false;
}

void URZ_TabWidget::Init(uint8 NewTabID, FString DisplayString)
{
	TabID = NewTabID;
	DisplayText->SetText(FText::FromString(DisplayString));
}

void URZ_TabWidget::SetIsSelected_BPN_Implementation(bool bNewIsSelected)
{
	bIsSelected = bNewIsSelected;
}