/// RemzDNB
///
///	RZ_DamageMarkerWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_DamageMarkerWidget.generated.h"

#define DAMAGEMARKER_ZOFFSET 500.0f
#define DAMAGEMARKER_MAXRANDSEED 25.0f
#define DAMAGEMARKER_DELAY 1.0f

class UTextBlock;

UCLASS()
class RZM_SHARED_API URZ_DamageMarkerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION() 
	void Init(float Damage, const FVector& NewHitLocation);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationBPI(float Delay);

private:

	FTimerHandle DelayTimer;
	FVector HitLocation;
	float RandXSeed;

	//

	UPROPERTY(meta = (BindWidget)) UPanelWidget* MainPanel;
	UPROPERTY(meta = (BindWidget)) UTextBlock* DamageText;
};
