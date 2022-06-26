#include "UI/RZ_CrosshairWidget.h"
#include "Core/RZ_PlayerController.h"
//
#include "Kismet/GameplayStatics.h"

void URZ_CrosshairWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Not called
}

void URZ_CrosshairWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URZ_CrosshairWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!OwningPlayerController)
	{
		OwningPlayerController = Cast<ARZ_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (OwningPlayerController)
		{
			OwningPlayerController->OnControllerInteractionModeUpdated.AddUniqueDynamic(
				this,
				&URZ_CrosshairWidget::OnPlayerControllerModeUpdated
			);
		}
	}
}

void URZ_CrosshairWidget::OnPlayerControllerModeUpdated(ERZ_ControllerInteractionMode NewMode)
{
	//UE_LOG(LogTemp,Warning,TEXT("URZ_CrosshairWidget::OnPlayerControllerModeUpdated "));
	
	if (NewMode == ERZ_ControllerInteractionMode::Construction)
	{
		OnSpawnModeEnabledBPI(true);
	}
	else
	{
		OnSpawnModeEnabledBPI(false);
	}
}
