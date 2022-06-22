/// RemzDNB

#include "Building/RZ_BuildingOTM_BaseWidget.h"
#include "Building/RZ_BuildingOTM_CollapsedWidget.h"
#include "Building/RZ_BuildingOTM_ExpandedWidget.h"
//
#include "Components/WidgetSwitcher.h"

URZ_BuildingOTM_BaseWidget::URZ_BuildingOTM_BaseWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsExpanded = false;
}

void URZ_BuildingOTM_BaseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WidgetSwitcher->ClearChildren();
	
	CollapsedWidget = CreateWidget<URZ_BuildingOTM_CollapsedWidget>(
		GetWorld(),
		CollapsedWidgetClass
	);
	if (CollapsedWidget)
	{
		//CollapsedWidget->Initialize()
		WidgetSwitcher->AddChild(CollapsedWidget);
	}

	ExpandedWidget = CreateWidget<URZ_BuildingOTM_ExpandedWidget>(
		GetWorld(),
		ExpandedWidgetClass
	);
	if (ExpandedWidget)
	{
		//CollapsedWidget->Initialize()
		WidgetSwitcher->AddChild(ExpandedWidget);
	}
}

void URZ_BuildingOTM_BaseWidget::Init(ARZ_Pawn* NewBuildingRef)
{
	ExpandedWidget->Init(NewBuildingRef);
}

void URZ_BuildingOTM_BaseWidget::ToggleBuildingOTM_BPI_Implementation()
{
	bIsExpanded = !bIsExpanded;
	WidgetSwitcher->SetActiveWidgetIndex(bIsExpanded);		
}
