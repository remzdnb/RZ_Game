// RemzDNB

#include "UI/RZ_ActorManager_ActorWidget.h"
#include "RZ_PowerManager.h"
#include "RZ_PowerComponentInterface.h"
//
#include "EngineUtils.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void URZ_ActorManager_ActorWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void URZ_ActorManager_ActorWidget::Update(AActor* ActorRef)
{
	if (!ActorRef) { return; }
	
	NameText->SetText(FText::FromString(ActorRef->GetName()));

	//
	
	IRZ_ActorInterface* ActorInterface = Cast<IRZ_ActorInterface>(ActorRef);
	if (ActorInterface)
	{
		ThumbnailImage->SetBrushFromTexture(ActorInterface->GetActorSettings().ThumbnailTexture);
	}

	//
	
	IRZ_PowerComponentInterface* PowerUserInterface = Cast<IRZ_PowerComponentInterface>(ActorRef);
	if (PowerUserInterface)
	{
		if (PowerUserInterface->GetPowerComponent()->GetPowerComponentSettings().PowerDelta > 0)
		{
			ProducedPowerText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ProducedPowerText->SetText(FText::FromString(
				"+ " +
				FString::FromInt(PowerUserInterface->GetPowerComponent()->GetPowerComponentSettings().PowerDelta) +
				" Mw/H"
			));
		}
		else
		{
			ProducedPowerText->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (PowerUserInterface->GetPowerComponent()->GetPowerComponentSettings().PowerDelta < 0)
		{
			ConsumedPowerText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ConsumedPowerText->SetText(FText::FromString(
				"- " +
				FString::FromInt(PowerUserInterface->GetPowerComponent()->GetPowerComponentSettings().PowerDelta * -1) +
				" Mw/H"
			));
		}
		else
		{
			ConsumedPowerText->SetVisibility(ESlateVisibility::Collapsed);
		}

		const FString StatusString = PowerUserInterface->GetPowerComponent()->GetIsPowered() ? "Online" : "Offline";
		const FSlateColor Color = PowerUserInterface->GetPowerComponent()->GetIsPowered()
			                          ? FSlateColor(FColor::Green)
			                          : FSlateColor(FColor::Red);

		StatusText->SetText(FText::FromString(StatusString));
		StatusText->SetColorAndOpacity(Color);
	}
}


