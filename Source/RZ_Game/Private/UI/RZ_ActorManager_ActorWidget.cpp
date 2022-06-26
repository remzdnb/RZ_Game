// RemzDNB

#include "UI/RZ_ActorManager_ActorWidget.h"
#include "RZ_PowerManager.h"
//
#include "EngineUtils.h"
#include "Components/TextBlock.h"

void URZ_ActorManager_ActorWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void URZ_ActorManager_ActorWidget::Update(AActor* ActorRef)
{
	ActorNameText->SetText(FText::FromString(ActorRef->GetName()));
}


