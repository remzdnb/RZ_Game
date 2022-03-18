#include "RZ_LogWidget.h"
//
#include "Components/TextBlock.h"

void URZ_LogWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	
}

void URZ_LogWidget::AddGameNotification(const FString& NotificationString)
{
	GameNotificationText->SetText(FText::FromString(NotificationString));
}
