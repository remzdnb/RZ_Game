#include "RZ_StatWidget.h"
//
#include "Components/TextBlock.h"

void URZ_StatWidget::Init(float NewStatValue)
{
	StatNameText->SetText(FText::FromString(StatName));
	StatValueText->SetText(FText::FromString(FString::FromInt(NewStatValue)));
}
