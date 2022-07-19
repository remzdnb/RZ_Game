/// RemzDNB
///
///	RZ_UIManager.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "RZ_UIManager.h"
#include "RZ_UISystem.h"
#include "RZ_MenuLayoutWidget.h"
//
#include "Components/PanelWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "EngineUtils.h"

ARZ_UIManager::ARZ_UIManager() :
	bIsMenuOpen(false),
	bIsHUDOpen(true)
{
}

void ARZ_UIManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld() == false)
		return;
	
	UISettings = Cast<IRZ_UISystemPluginSettingsInterface>(GetGameInstance())->GetUIManagerModuleSettings();

	///

	MenuLayoutWidget = CreateWidget<URZ_MenuLayoutWidget>(GetOwningPlayerController(), UISettings->MenuLayoutWBP);
	if (MenuLayoutWidget)
	{
		MenuLayoutWidget->Init(this);
	}
}

void ARZ_UIManager::BeginPlay()
{
	Super::BeginPlay();
}

void ARZ_UIManager::ToggleMenu(bool bNewIsOpen)
{
	if (MenuLayoutWidget == nullptr)
		return;

	if (bNewIsOpen == true)
	{
		if (MenuLayoutWidget->IsInViewport() == false)
			MenuLayoutWidget->AddToViewport();
		
		bIsMenuOpen = true;
	}
	else
	{
		if (MenuLayoutWidget->IsInViewport() == true)
			MenuLayoutWidget->RemoveFromParent();
		
		bIsMenuOpen = false;
	}
}

void ARZ_UIManager::ToggleHUD(bool bNewIsOpen)
{
	if (bNewIsOpen == true && IsHUDOpen() == false)
	{
		for (const auto& HUDWidget : HUDWidgets)
		{
			HUDWidget->AddToViewport();
			bIsHUDOpen = true;
		}
	}
	
	if (bNewIsOpen == false && IsHUDOpen() == true)
	{
		for (const auto& HUDWidget : HUDWidgets)
		{
			HUDWidget->RemoveFromParent();
			bIsHUDOpen = false;
		}
	}
}

UUserWidget* ARZ_UIManager::CreateMenuWidget(TSubclassOf<UUserWidget> WidgetClass, const FName& WidgetName)
{
	if (GetMenuLayoutWidget())
	{
		return GetMenuLayoutWidget()->CreateMenuWidget(WidgetName, WidgetClass, true);
	}

	return nullptr;
}

UUserWidget* ARZ_UIManager::CreateHUDWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	if (NewWidget)
	{
		NewWidget->AddToViewport();
		HUDWidgets.Add(NewWidget);
		return NewWidget;
	}

	return nullptr;
}

void ARZ_UIManager::AddHUDWidget(UUserWidget* WidgetToAdd, bool bSaveReference)
{

	if (bSaveReference)
	{
		HUDWidgets.Add(WidgetToAdd);
	}
	
	if (IsHUDOpen())
	{
		WidgetToAdd->AddToViewport();
	}
	
	/*if (BattleHUDWidget == nullptr)
	return;
	
	BattleHUDWidget->GetMainCanvas()->AddChild(NewWidget);

	UCanvasPanelSlot* const CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(NewWidget);
	CanvasPanelSlot->bAutoSize = true;*/ // ?? Refresh
}
