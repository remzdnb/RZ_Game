#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_ExitWidget.generated.h"

UCLASS()
class RZM_UISYSTEM_API URZ_ExitWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeOnInitialized() override;

private:

	UPROPERTY(meta = (BindWidget)) class URZ_ButtonWidget* QuitToDesktopButton;
	UPROPERTY(meta = (BindWidget)) class URZ_ButtonWidget* QuitToMenuButton;

	UFUNCTION() void OnQuitToDesktopButtonPressed(uint8 ButtonID);
	UFUNCTION() void OnQuitToMenuButtonButtonPressed(uint8 ButtonID);
};
