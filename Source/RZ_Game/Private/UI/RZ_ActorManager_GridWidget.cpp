/// RemzDNB

#include "UI/RZ_ActorManager_GridWidget.h"
#include "UI/RZ_ActorManager_ActorWidget.h"
#include "Core/RZ_GameState.h"
#include "Core/RZ_GameSettings.h"
#include "RZ_PowerManager.h"
#include "RZ_ProgressBarWidget.h"
//
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"

void URZ_ActorManager_GridWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const ARZ_GameState* GameState = Cast<ARZ_GameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameSettings = GameState->GetGameSettings();
	}
}

void URZ_ActorManager_GridWidget::Update(ARZ_PowerManager* PowerManagerRef, uint8 GridID)
{
	if (!PowerManagerRef->GetPowerGrids().IsValidIndex(GridID)) { return; }

	ActorContainer->ClearChildren();
	
	PowerProgressBar->Update(
		PowerManagerRef->GetPowerGrids()[GridID].ConsumedPower,
		PowerManagerRef->GetPowerGrids()[GridID].ProducedPower
	);
	
	for (const auto& PowerComponent : PowerManagerRef->GetComponentsFromGrid(GridID - 1))
	{
		URZ_ActorManager_ActorWidget* ActorWidget = CreateWidget<URZ_ActorManager_ActorWidget>(
			GetWorld(),
			GameSettings->ActorManager_ActorWidgetClass
		);
		if (ActorWidget)
		{
			ActorWidget->Update(PowerComponent->GetOwner());
			ActorContainer->AddChild(ActorWidget);
		}
	}
}
