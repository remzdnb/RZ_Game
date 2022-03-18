#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_LogWidget.generated.h"

class UTextBlock;

UCLASS()
class RZ_UIMANAGERPLUGIN_API URZ_LogWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	
	UFUNCTION()
	void AddGameNotification(const FString& NotificationString);
	
private:

	UPROPERTY(meta = (BindWidget)) UTextBlock* GameNotificationText;
};
