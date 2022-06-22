/// RemzDNB

#include "S2D_PawnOTM_BaseWidget.h"
#include "S2D_PawnOTM_CollapsedWidget.h"
#include "S2D_PawnOTM_ExpandedWidget.h"
//
#include "Components/WidgetSwitcher.h"

US2D_PawnOTM_BaseWidget::US2D_PawnOTM_BaseWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsExpanded = false;
}

void US2D_PawnOTM_BaseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WidgetSwitcher->ClearChildren();
	
	CollapsedWidget = CreateWidget<US2D_PawnOTM_CollapsedWidget>(
		GetWorld(),
		CollapsedWidgetClass
	);
	if (CollapsedWidget)
	{
		//CollapsedWidget->Initialize()
		WidgetSwitcher->AddChild(CollapsedWidget);
	}

	ExpandedWidget = CreateWidget<US2D_PawnOTM_ExpandedWidget>(
		GetWorld(),
		ExpandedWidgetClass
	);
	if (ExpandedWidget)
	{
		//CollapsedWidget->Initialize()
		WidgetSwitcher->AddChild(ExpandedWidget);
	}
}

void US2D_PawnOTM_BaseWidget::Init(ARZ_Pawn* NewBuildingRef)
{
	ExpandedWidget->Init(NewBuildingRef);
}

void US2D_PawnOTM_BaseWidget::ToggleBuildingOTM_BPI_Implementation()
{
	bIsExpanded = !bIsExpanded;
	WidgetSwitcher->SetActiveWidgetIndex(bIsExpanded);		
}
