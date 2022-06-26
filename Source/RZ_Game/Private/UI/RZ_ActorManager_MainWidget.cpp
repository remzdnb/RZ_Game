// RemzDNB

#include "UI/RZ_ActorManager_MainWidget.h"
#include "UI/RZ_ActorManager_GridWidget.h"
#include "Core/RZ_GameState.h"
#include "Core/RZ_GameSettings.h"
#include "RZ_PowerManager.h"
//
#include "EngineUtils.h"
#include "Components/WidgetSwitcher.h"

void URZ_ActorManager_MainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const ARZ_GameState* GameState = Cast<ARZ_GameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameSettings = GameState->GetGameSettings();
	}
	
	for (TActorIterator<ARZ_PowerManager> FoundPowerManager(GetWorld()); FoundPowerManager; ++FoundPowerManager)
	{
		PowerManager = *FoundPowerManager;
	}
	if (PowerManager)
	{
		PowerManager->OnPowerManagerUpdated.AddUniqueDynamic(this, &URZ_ActorManager_MainWidget::Update);
		Update();
	}
}

void URZ_ActorManager_MainWidget::Update()
{
	if (!PowerManager) { return; }
	
	WidgetSwitcher->ClearChildren();
	SwitcherButtonsContainer->ClearChildren();

	for (const auto& Grid : PowerManager->GetPowerGrids())
	{
		URZ_ButtonWidget* ButtonWidget = CreateWidget<URZ_ButtonWidget>(
			GetWorld(),
			GameSettings->ActorManager_ButtonWidgetClass
		);
		if (ButtonWidget)
		{
			ButtonWidget->Init(Grid.GridID - 1, *("Grid " + FString::FromInt(Grid.GridID)));
			ButtonWidget->OnButtonPressed.AddUniqueDynamic(this, &URZ_ActorManager_MainWidget::OnSwitcherButtonPressed);
			SwitcherButtonsContainer->AddChild(ButtonWidget);
		}
		
		URZ_ActorManager_GridWidget* GridWidget = CreateWidget<URZ_ActorManager_GridWidget>(
			GetWorld(),
			GameSettings->ActorManager_GridWidgetClass
		);
		if (GridWidget)
		{
			GridWidget->Update(PowerManager, Grid.GridID);
			WidgetSwitcher->AddChild(GridWidget);
		}
	}
}

void URZ_ActorManager_MainWidget::OnSwitcherButtonPressed(uint8 ButtonID)
{
	WidgetSwitcher->SetActiveWidgetIndex(ButtonID);
}



