// RemzDNB

#pragma once

#include "RZM_PowerSystem.h"
#include "Blueprint/UserWidget.h"
#include "RZ_ActorManager_ActorWidget.generated.h"

UCLASS()
class RZ_GAME_API URZ_ActorManager_ActorWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	void Update(AActor* ActorRef);

	//

	UPROPERTY(meta = (BindWidget)) class UTextBlock* ActorNameText;
	
};
