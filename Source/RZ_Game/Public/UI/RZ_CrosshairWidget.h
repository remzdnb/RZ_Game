#pragma once

#include "CoreMinimal.h"
#include "RZ_Game.h"
#include "Blueprint/UserWidget.h"
#include "RZ_CrosshairWidget.generated.h"

class ARZ_PlayerController;

UCLASS()
class RZ_GAME_API URZ_CrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UFUNCTION(BlueprintImplementableEvent) void OnSpawnModeEnabledBPI(bool bNewIsEnabled);
	
	UFUNCTION(BlueprintImplementableEvent) void StartRunAnimationBPI(float AnimTime);
	UFUNCTION(BlueprintImplementableEvent) void StartEquipAnimationBPI(float AnimTime);
	UFUNCTION(BlueprintImplementableEvent) void StartUseAnimationBPI(float AnimTime);
	UFUNCTION(BlueprintImplementableEvent) void StartReloadAnimationBPI(float AnimTime);

private:

	UFUNCTION()
	void OnPlayerControllerModeUpdated(ERZ_PlayerControllerMode NewMode);

	ARZ_PlayerController* OwningPlayerController;
};
