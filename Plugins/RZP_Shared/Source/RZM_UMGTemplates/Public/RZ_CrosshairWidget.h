#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_CrosshairWidget.generated.h"

UCLASS()
class RZM_UMGTEMPLATES_API URZ_CrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent) void StartRunAnimationBPI(float AnimTime);
	UFUNCTION(BlueprintImplementableEvent) void StartEquipAnimationBPI(float AnimTime);
	UFUNCTION(BlueprintImplementableEvent) void StartUseAnimationBPI(float AnimTime);
	UFUNCTION(BlueprintImplementableEvent) void StartReloadAnimationBPI(float AnimTime);
};
