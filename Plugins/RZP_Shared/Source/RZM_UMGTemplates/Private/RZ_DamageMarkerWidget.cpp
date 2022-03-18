#include "RZ_DamageMarkerWidget.h"
//
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "TimerManager.h"

void URZ_DamageMarkerWidget::Init(float PrimaryDamage, float SecondaryDamage, const FVector& NewHitLocation, const FLinearColor& Color)
{
	HitLocation = NewHitLocation;
	
	PrimaryDamageText->SetText(FText::FromString(FString::FromInt(PrimaryDamage)));
	PrimaryDamageText->SetColorAndOpacity(Color);
	
	if (SecondaryDamage > 0)
		SecondaryDamageText->SetText(FText::FromString("( " + FString::FromInt(SecondaryDamage) + " )"));
	else
		SecondaryDamageText->SetVisibility(ESlateVisibility::Collapsed);

	RandXSeed = FMath::RandRange(DAMAGEMARKER_MAXRANDSEED * -1, DAMAGEMARKER_MAXRANDSEED);

	GetWorld()->GetTimerManager().SetTimer(DelayTimer, this, &URZ_DamageMarkerWidget::RemoveFromParent, DAMAGEMARKER_DELAY, false, DAMAGEMARKER_DELAY);

	PlayAnimationBPI(DAMAGEMARKER_DELAY);
}

void URZ_DamageMarkerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	FVector2D TargetScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), HitLocation, TargetScreenPosition, false);
	TargetScreenPosition.X = TargetScreenPosition.X - MainPanel->GetDesiredSize().X / 2;
	TargetScreenPosition.Y = TargetScreenPosition.Y - MainPanel->GetDesiredSize().Y / 2;

	MainPanel->SetRenderTranslation(TargetScreenPosition);
}
