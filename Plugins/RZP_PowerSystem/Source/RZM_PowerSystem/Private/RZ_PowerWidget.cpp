/// RemzDNB

#include "RZ_PowerWidget.h"
// Engine
#include "Components/TextBlock.h"

URZ_PowerWidget::URZ_PowerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void URZ_PowerWidget::Update(URZ_PowerComponent* NewPowerComp)
{
	PowerComp = NewPowerComp;
	
	if (!PowerComp) { return; }

	if (PowerComp->GetPowerComponentSettings().bIsProducingPower)
	{
		ProducedPowerText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ProducedPowerText->SetText(FText::FromString(
			"Produced Power = " +
			FString::FromInt(PowerComp->GetPowerComponentSettings().MaxProducedPower)
		));
	}
	else
	{
		ProducedPowerText->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (PowerComp->GetPowerComponentSettings().bIsConsumingPower)
	{
		ConsumedPowerText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ConsumedPowerText->SetText(FText::FromString(
			"Consumed Power = " +
			FString::FromInt(PowerComp->GetPowerComponentSettings().MaxConsumedPower)
		));
	}
	else
	{
		ConsumedPowerText->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	GridIDText->SetText(FText::FromString(FString("GridID = ") + FString::FromInt(PowerComp->PowerGridID)));
	
	/*if (IsPoweredText)
	{
		PowerComp->bIsPowered
			? IsPoweredText->SetText(FText::FromString("IsPowered = true"))
			: IsPoweredText->SetText(FText::FromString("IsPowered = false"));
	}*/
}
