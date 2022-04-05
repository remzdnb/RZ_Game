/// RemzDNB
///
///	RZ_MenuLayoutWidget.cpp
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "RZ_MenuLayoutWidget.h"
#include "RZ_ButtonWidget.h"
#include "RZM_UISystem.h"
#include "RZ_UIManager.h"
// Engine
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/GameStateBase.h"

void URZ_MenuLayoutWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	UISettings = Cast<IRZ_UISystemPluginSettingsInterface>(GetGameInstance())->GetUIManagerModuleSettings();
	
	ButtonsContainerPanel->ClearChildren();
	WidgetSwitcher->ClearChildren();

	SwitcherIndex = -1;
	
	//CreateMenuWidget("Quit", UISettings->ExitWBP, false, ExitTab);
}

void URZ_MenuLayoutWidget::Init(ARZ_UIManager* NewUIManager)
{
	UIManager = NewUIManager;
}

UUserWidget* URZ_MenuLayoutWidget::CreateMenuWidget(const FName& TabName, TSubclassOf<UUserWidget> WidgetClass, bool bShouldCreateTab, class URZ_ButtonWidget* TabToAssign)
{
	if (WidgetClass == nullptr)
		return nullptr;
	
	UUserWidget* const NewWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), WidgetClass);
	if (NewWidget)
	{
		const uint8 NewWidgetID = LoadedWidgets.Num(); // Find new valid ID

		WidgetSwitcher->AddChild(NewWidget);
		LoadedWidgets.Add(TabName, NewWidget);
		
		// Create / Load Tab

		if (bShouldCreateTab)
		{
			URZ_ButtonWidget* NewTabWidget = CreateWidget<URZ_ButtonWidget>(GetWorld(), UISettings->MenuLayoutButtonWBP); // Create new TabWidget
			if (NewTabWidget)
			{
				ButtonsContainerPanel->AddChild(NewTabWidget);
				TabWidgets.Add(NewTabWidget);
				NewTabWidget->Init(NewWidgetID, TabName);
				NewTabWidget->OnButtonPressed.AddUniqueDynamic(this, &URZ_MenuLayoutWidget::SetActiveWidgetByIndex);
			}
		}
		else
		{
			TabWidgets.Add(TabToAssign);
			TabToAssign->Init(NewWidgetID, TabName);
			TabToAssign->OnButtonPressed.AddDynamic(this, &URZ_MenuLayoutWidget::SetActiveWidgetByIndex);
		}

		// Load widget

		SetActiveWidgetByIndex(1);
		
		return NewWidget;
	}

	return nullptr;
}

void URZ_MenuLayoutWidget::SetActiveWidgetByIndex(uint8 Index)
{
	if (Index == SwitcherIndex)
		return;

	if (TabWidgets.IsValidIndex(Index) == false)
		return;
	
	// Set all tabs as unselected, then select the right one.

	for (URZ_ButtonWidget* TabWidget : TabWidgets)
		TabWidget->SetIsSelected_BPN(false);
	
	TabWidgets[Index]->SetIsSelected_BPN(true);

	// Widget switcher

	WidgetSwitcher->SetActiveWidgetIndex(Index);
	SwitcherIndex = Index;

	OnNewActiveWidgetBPI();

	//
	
	if (UIManager)
	{
		uint8 MapIndex = 0;
		for (auto& MapRow : LoadedWidgets)
		{
			if (MapIndex == Index)
			{
				//UIManager->BroadcastMenuWidgetSelected(MapRow.Key, TabWidgets[Index]);
			}

			MapIndex++;
		}
	}
}

void URZ_MenuLayoutWidget::SetActiveWidgetByRef(UUserWidget* WidgetRef)
{
	if (WidgetSwitcher->GetActiveWidget() == WidgetRef)
		return;

	WidgetSwitcher->SetActiveWidget(WidgetRef);
}