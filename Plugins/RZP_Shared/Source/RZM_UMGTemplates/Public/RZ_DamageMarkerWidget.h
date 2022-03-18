///// RZ_DamageMarker.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_DamageMarkerWidget.generated.h"

#define DAMAGEMARKER_ZOFFSET 500.0f
#define DAMAGEMARKER_MAXRANDSEED 25.0f
#define DAMAGEMARKER_DELAY 1.0f

class UTextBlock;

UCLASS()
class RZM_UMGTEMPLATES_API URZ_DamageMarkerWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	//virtual void NativeOnInitialized();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION() 
	void Init(float PrimaryDamage, float SecondaryDamage, const FVector& NewHitLocation, const FLinearColor& Color);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAnimationBPI(float Delay);

private:

	FTimerHandle DelayTimer;
	FVector HitLocation;
	float RandXSeed;

	//

	UPROPERTY(meta = (BindWidget)) UPanelWidget* MainPanel;
	UPROPERTY(meta = (BindWidget)) UTextBlock* PrimaryDamageText;
	UPROPERTY(meta = (BindWidget)) UTextBlock* SecondaryDamageText;
};
