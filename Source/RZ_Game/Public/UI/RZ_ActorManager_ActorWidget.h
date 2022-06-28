// RemzDNB

#pragma once

#include "RZM_PowerSystem.h"
#include "Blueprint/UserWidget.h"
#include "RZ_ActorManager_ActorWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class RZ_GAME_API URZ_ActorManager_ActorWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void Update(AActor* ActorRef);

private:
	
	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* ThumbnailImage;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* ProducedPowerText;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* ConsumedPowerText;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* StatusText;

};
