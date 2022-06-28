// RemzDNB

#include "UI/RZ_ActorManager_ActorWidget.h"
#include "RZ_PowerManager.h"
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
	
	IRZ_PowerUserInterface* PowerUserInterface = Cast<IRZ_PowerUserInterface>(ActorRef);
	if (PowerUserInterface)
	{
		if (PowerUserInterface->GetPowerComponent()->GetPowerComponentSettings().MaxProducedPower > 0)
		{
			ProducedPowerText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ProducedPowerText->SetText(FText::FromString(
				"+ " +
				FString::FromInt(PowerUserInterface->GetPowerComponent()->GetPowerComponentSettings().MaxProducedPower) +
				" Mw/H"
			));
		}
		else
		{
			ProducedPowerText->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (PowerUserInterface->GetPowerComponent()->GetPowerComponentSettings().MaxConsumedPower > 0)
		{
			ConsumedPowerText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ConsumedPowerText->SetText(FText::FromString(
				"- " +
				FString::FromInt(PowerUserInterface->GetPowerComponent()->GetPowerComponentSettings().MaxConsumedPower) +
				" Mw/H"
			));
		}
		else
		{
			ConsumedPowerText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}


