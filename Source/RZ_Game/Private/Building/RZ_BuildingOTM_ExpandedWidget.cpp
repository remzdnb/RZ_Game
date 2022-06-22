/// RemzDNB

#include "Building/RZ_BuildingOTM_ExpandedWidget.h"
#include "Pawn/RZ_Pawn.h"
// PowerSystem
#include "RZ_PowerWidget.h"
// Engine
#include "Components/TextBlock.h"

void URZ_BuildingOTM_ExpandedWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void URZ_BuildingOTM_ExpandedWidget::Init(ARZ_Pawn* NewBuildingRef)
{
	BuildingRef = NewBuildingRef;
	Update();
}

void URZ_BuildingOTM_ExpandedWidget::Update()
{
	if (!BuildingRef) { return; }

	BuildingNameText->SetText(FText::FromName(BuildingRef->GetItemSettings().DisplayName));
	//PowerWidget->Update(BuildingRef->GetPowerComponent());
}
