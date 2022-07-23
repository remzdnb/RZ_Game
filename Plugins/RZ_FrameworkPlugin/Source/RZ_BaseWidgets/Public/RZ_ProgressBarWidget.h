///// RZ_ProgressBarWidget.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_ProgressBarWidget.generated.h"

#define PROGRESSBAR_LERPSPEED 0.05f

UCLASS()
class RZ_BASEWIDGETS_API URZ_ProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION() 
	void Update(float Value, float MaxValue, const FString& DisplayString);

private:

	float TargetPercent;

	UPROPERTY(meta = (BindWidget)) class UTextBlock* ValueText;
	UPROPERTY(meta = (BindWidget)) class UProgressBar* BaseProgressBar;
	UPROPERTY(meta = (BindWidget, OptionalWidget)) class UProgressBar* LerpedProgressBar;
};
