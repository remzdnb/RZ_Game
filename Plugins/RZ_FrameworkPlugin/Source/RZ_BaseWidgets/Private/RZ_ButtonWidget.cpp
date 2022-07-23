/// RemzDNB
///
///	RZ_ButtonWidget.cpp
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "RZ_ButtonWidget.h"
//
#include "Components/Button.h"
#include "Components/TextBlock.h"

void URZ_ButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bIsSelected = false;

	if (DisplayText.IsEmpty() == false)
		Text->SetText(FText::FromString(DisplayText));
	else
		Text->SetText(FText::FromString("DefaultText"));

	Button->OnClicked.AddDynamic(this, &URZ_ButtonWidget::OnPress);
}

void URZ_ButtonWidget::Init(uint8 NewTabID, FName TabName)
{
	TabID = NewTabID;
	Text->SetText(FText::FromName(TabName));
}

void URZ_ButtonWidget::SetIsSelected_BPN_Implementation(bool bNewIsSelected)
{
	bIsSelected = bNewIsSelected;
}

void URZ_ButtonWidget::OnPress()
{
	OnButtonPressed.Broadcast(TabID);
}
