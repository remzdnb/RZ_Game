#include "RZ_EditableTextWidget.h"
//
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Components/Image.h"

void URZ_EditableTextWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (DefaultTitle.IsEmpty() == false)
		TitleText->SetText(FText::FromString(DefaultTitle));

	if (DefaultText.IsEmpty() == false)
		EditableText->SetText(FText::FromString(DefaultText));

	if (DefaultImage)
		Image->SetBrushFromTexture(DefaultImage, false);

	EditableText->OnTextCommitted.AddDynamic(this, &URZ_EditableTextWidget::OnTextCommited);
	//EditableText->OnTextChanged.AddDynamic(this, &URZ_EditableTextWidget::OnTextChanged);
}

const FText URZ_EditableTextWidget::GetText()
{
	return EditableText->GetText();
}

void URZ_EditableTextWidget::SetText(const FText& Text)
{
	EditableText->SetText(Text);
}

void URZ_EditableTextWidget::OnTextCommited(const FText& Text, ETextCommit::Type CommitMethod)
{
	OnTextValidated.Broadcast(Text);
}

void URZ_EditableTextWidget::OnTextChanged(const FText& Text)
{
	OnTextValidated.Broadcast(Text);
}

FReply URZ_EditableTextWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// Text input will be focused wherever the user clicked on the widget.
	EditableText->SetFocus();
	EditableText->SetKeyboardFocus();

	return FReply::Handled();
}
