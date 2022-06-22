/// RemzDNB

#include "S2D_PawnOTM_ExpandedWidget.h"
#include "Pawn/RZ_Pawn.h"
// PowerSystem
#include "RZ_PowerWidget.h"
// Engine
#include "Components/TextBlock.h"

void US2D_PawnOTM_ExpandedWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void US2D_PawnOTM_ExpandedWidget::Init(ARZ_Pawn* NewBuildingRef)
{
	BuildingRef = NewBuildingRef;
	Update();
}

void US2D_PawnOTM_ExpandedWidget::Update()
{
	if (!BuildingRef) { return; }

	BuildingNameText->SetText(FText::FromName(BuildingRef->GetItemSettings().DisplayName));
	//PowerWidget->Update(BuildingRef->GetPowerComponent());
}
