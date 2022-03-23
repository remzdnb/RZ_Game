#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_HomeWidget.generated.h"

UCLASS()
class RZM_UIMANAGER_API URZ_HomeWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	
private:

	UFUNCTION() void OnPlayerNameChanged(const FText& PlayerNameText);
	UFUNCTION() void OnMapNameChanged(const FText& MapNameText);
	UFUNCTION() void OnJoinIPChanged(const FText& JoinIPText);
	UFUNCTION() void OnLoadMapButtonPressed(uint8 ButtonID);
	UFUNCTION() void OnJoinServerButtonPressed(uint8 ButtonID);
	UFUNCTION() void OnCreateServerButtonPressed(uint8 ButtonID);

	UPROPERTY(meta = (BindWidget)) class URZ_EditableTextWidget* PlayerNameEText;
	UPROPERTY(meta = (BindWidget)) class URZ_EditableTextWidget* MapNameEText;
	UPROPERTY(meta = (BindWidget)) class URZ_EditableTextWidget* JoinIPEText;
	UPROPERTY(meta = (BindWidget)) class URZ_ButtonWidget* LoadMapButton;
	UPROPERTY(meta = (BindWidget)) class URZ_ButtonWidget* JoinServerButton;
	UPROPERTY(meta = (BindWidget)) class URZ_ButtonWidget* CreateServerButton;

};
