/// RemzDNB
///
///	RZ_MenuLayoutWidget.h
///	Basic menu layout.
///	Creating a menu widget adds it to the WidgetSwitcher, and creates a menu Tab associated with it.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_MenuLayoutWidget.generated.h"

UCLASS()
class RZ_UIMANAGERPLUGIN_API URZ_MenuLayoutWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	///
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UFUNCTION()
	void Init(class ARZ_UIManager* NewUIManager);

	UFUNCTION() // Load UserWidget into the widget switcher and create associated top bar tab.
	class UUserWidget* CreateMenuWidget(const FName& TabName, TSubclassOf<UUserWidget> WidgetClass, bool bShouldCreateTab = true, class URZ_ButtonWidget* TabToAssign = nullptr);

	UFUNCTION()
	void SetActiveWidgetByIndex(uint8 Index);

	UFUNCTION()
	void SetActiveWidgetByRef(UUserWidget* WidgetRef);

	UFUNCTION(BlueprintImplementableEvent)
	void OnNewActiveWidgetBPI();

	///

	UFUNCTION() FORCEINLINE UUserWidget* GetLoadedWidgetByName(const FName& WidgetName) const { return LoadedWidgets.FindRef(WidgetName); }

private:

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ButtonsContainerPanel;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	class URZ_ButtonWidget* ExitButton;

	///
	
	class ARZ_UIManager* UIManager;
	class URZ_UIManagerPluginSettings* UISettings;

	///
	
	UPROPERTY()
	TArray<class URZ_ButtonWidget*> TabWidgets;

	UPROPERTY()
	TMap<FName, UUserWidget*> LoadedWidgets;

	UPROPERTY()
	uint8 SwitcherIndex;

};
