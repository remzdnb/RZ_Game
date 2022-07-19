/// RemzDNB

#include "RZ_PawnOTMWidget.h"
#include "RZ_PowerManager.h"
#include "RZ_PowerComponent.h"
#include "Actor/RZ_Pawn.h"
//
#include "WidgetTemplates/RZ_ButtonWidget.h"
//
#include "RZ_BuildingComponent.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "LauncherServices/Public/ILauncherProfile.h"


URZ_PawnOTMWidget::URZ_PawnOTMWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsExpanded = false;
}

void URZ_PawnOTMWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void URZ_PawnOTMWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (BuildingComp)
	{
		if (BuildingComp->GetIsBuilding())
		{
			Collapsed_BuildingProgressBar->SetPercent(GetWorld()->GetTimeSeconds() - BuildingComp->GetBuildableActorSettings().BuildTime);
		}
	}
}

void URZ_PawnOTMWidget::Init(ARZ_Pawn* NewPawnRef)
{
	PawnRef = NewPawnRef;
	if (PawnRef)
	{
		BuildingComp = Cast<URZ_BuildingComponent>(PawnRef->GetComponentByClass(URZ_BuildingComponent::StaticClass()));
		if (BuildingComp)
		{
				
		}
		
		PowerCompRef = Cast<URZ_PowerComponent>(PawnRef->GetComponentByClass(URZ_PowerComponent::StaticClass()));
		if (PowerCompRef)
		{
			PowerCompRef->OnPowerComponentUpdated.AddUniqueDynamic(this, &URZ_PawnOTMWidget::Update);
		}
	}

	Update();
}

void URZ_PawnOTMWidget::Update()
{
	if (!PawnRef || !PowerCompRef) { return; }

	PowerCompRef->GetIsPowered()
		? Collapsed_PowerImage->SetVisibility(ESlateVisibility::Hidden)
		: Collapsed_PowerImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	Expanded_PawnNameText->SetText(FText::FromName(PawnRef->GetItemSettings().DisplayName));
	Expanded_GridIDText->SetText(
		FText::FromString("Grid " + FString::FromInt(PawnRef->GetPowerComponent()->GetPowerGridID()))
	);
}

void URZ_PawnOTMWidget::ToggleBuildingOTM_BPI_Implementation()
{
	bIsExpanded = !bIsExpanded;
	WidgetSwitcher->SetActiveWidgetIndex(bIsExpanded);		
}
