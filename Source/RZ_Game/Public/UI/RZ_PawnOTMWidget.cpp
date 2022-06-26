/// RemzDNB

#include "RZ_PawnOTMWidget.h"
#include "RZ_PowerManager.h"
#include "RZ_PowerComponent.h"
//
#include "RZ_ButtonWidget.h"
//
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"


URZ_PawnOTMWidget::URZ_PawnOTMWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsExpanded = false;
}

void URZ_PawnOTMWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void URZ_PawnOTMWidget::Init(ARZ_Pawn* NewPawnRef)
{
	PawnRef = NewPawnRef;
	if (PawnRef)
	{
		PawnRef->GetPowerComponent()->OnPowerComponentUpdated.AddUniqueDynamic(this, &URZ_PawnOTMWidget::Update);
	}

	Update();
}

void URZ_PawnOTMWidget::Update()
{
	Expanded_PawnNameText->SetText(FText::FromName(PawnRef->GetActorSettings().DisplayName));
	Expanded_GridIDText->SetText(FText::FromString("Grid " + FString::FromInt(PawnRef->GetPowerComponent()->GetPowerGridID())));
	Expanded_ConnectedPowerCompsPanel->ClearChildren();
	for (const auto& ConnectedPowerComp : PawnRef->GetPowerComponent()->GetConnectedPowerComps())
	{
		URZ_ButtonWidget* ConnectedPowerCompButton = CreateWidget<URZ_ButtonWidget>(
			GetWorld(),
			PawnRef->GetPowerComponent()->PowerManager->PowerComponentTextWidgetClass
		);
		if (ConnectedPowerCompButton)
		{
			Expanded_ConnectedPowerCompsPanel->AddChild(ConnectedPowerCompButton);
			ConnectedPowerCompButton->Init(0, *ConnectedPowerComp->GetOwner()->GetName());
		}
	}
}

void URZ_PawnOTMWidget::ToggleBuildingOTM_BPI_Implementation()
{
	bIsExpanded = !bIsExpanded;
	WidgetSwitcher->SetActiveWidgetIndex(bIsExpanded);		
}
