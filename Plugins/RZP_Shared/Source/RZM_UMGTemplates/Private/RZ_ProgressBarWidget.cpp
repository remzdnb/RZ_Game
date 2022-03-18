#include "RZ_ProgressBarWidget.h"
//
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void URZ_ProgressBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TargetPercent = 1.0f;
}

void URZ_ProgressBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	LerpedProgressBar->SetPercent(FMath::Lerp(LerpedProgressBar->Percent, TargetPercent, PROGRESSBAR_LERPSPEED));
}

void URZ_ProgressBarWidget::Update(float Value, float MaxValue)
{
	ValueText->SetText(FText::FromString(FString::FromInt(Value)));
	TargetPercent = Value / MaxValue;
	BaseProgressBar->SetPercent(Value / MaxValue);
}