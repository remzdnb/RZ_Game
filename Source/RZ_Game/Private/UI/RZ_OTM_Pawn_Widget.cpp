/// RemzDNB

#include "RZ_OTM_Pawn_Widget.h"
#include "RZ_PowerManager.h"
#include "RZ_PowerComponent.h"
#include "Pawn/RZ_Pawn.h"
//
#include "RZ_ButtonWidget.h"
//
#include "RZ_BuildingComponent.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "LauncherServices/Public/ILauncherProfile.h"


URZ_OTM_Pawn_Widget::URZ_OTM_Pawn_Widget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsExpanded = false;
}

void URZ_OTM_Pawn_Widget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void URZ_OTM_Pawn_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (BuildingComp)
	{
		if (BuildingComp->GetIsBuilding())
		{
			Collapsed_BuildPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Collapsed_BuildProgressBar->SetPercent((GetWorld()->GetTimeSeconds() - BuildingComp->GetBuildStartTime()) / BuildingComp->GetBuildableActorSettings().BuildTime);
		}
		else
		{
			Collapsed_BuildPanel->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void URZ_OTM_Pawn_Widget::Init(ARZ_Pawn* NewPawnRef)
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
			PowerCompRef->OnPowerComponentUpdated.AddUniqueDynamic(this, &URZ_OTM_Pawn_Widget::Update);
		}
	}

	Update();
}

void URZ_OTM_Pawn_Widget::Update()
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

void URZ_OTM_Pawn_Widget::ToggleBuildingOTM_BPI_Implementation()
{
	bIsExpanded = !bIsExpanded;
	WidgetSwitcher->SetActiveWidgetIndex(bIsExpanded);		
}
