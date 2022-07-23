/// RemzDNB

#include "RZ_AIActionListWidget.h"
#include "RZ_PawnAIController.h"
//
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "Core/RZ_GameInstance.h"

URZ_AIActionListWidget::URZ_AIActionListWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void URZ_AIActionListWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void URZ_AIActionListWidget::Init(ARZ_PawnAIController* NewOwnerPawnAIController)
{
	GameSettings = Cast<URZ_GameInstance>(GetWorld()->GetGameInstance())->GetGameSettings();
	
	OwnerPawnAIController = NewOwnerPawnAIController;
	if (OwnerPawnAIController.IsValid())
	{
		OwnerPawnAIController->OnActionListUpdated.AddUniqueDynamic(this, &URZ_AIActionListWidget::Update);
	}

	Update();
}

void URZ_AIActionListWidget::Update()
{
	if (!OwnerPawnAIController.IsValid()) { return; }

	ActionSlotContainer->ClearChildren();
	
	for (const auto& Action : OwnerPawnAIController->GetActionList())
	{
		URZ_ImageWidget* ActionSlotWidget = CreateWidget<URZ_ImageWidget>(
			GetWorld(),
			GameSettings->ActionSlotWidgetClass
		);
		if (ActionSlotWidget)
		{
			ActionSlotWidget->Update(Action.ActionName.ToString(), GameSettings->ActionTypeThumbnails.FindRef(Action.ActionName));
			ActionSlotContainer->AddChild(ActionSlotWidget);
		}
	}
}
