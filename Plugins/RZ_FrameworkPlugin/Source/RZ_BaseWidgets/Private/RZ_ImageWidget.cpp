#include "RZ_ImageWidget.h"
//
#include "Components/TextBlock.h"
#include "Components/Image.h"

void URZ_ImageWidget::Update(const FString& DisplayString, const UMaterialInterface* TextureToDisplay) const
{
	Text->SetText(FText::FromString(DisplayString));
	Image->SetBrushFromSoftMaterial(TextureToDisplay);
}