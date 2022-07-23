/// RemzDNB

#include "RZ_OTM_Character_Widget.h"
#include "Pawn/RZ_Pawn.h"
//
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "LauncherServices/Public/ILauncherProfile.h"


URZ_OTM_Character_Widget::URZ_OTM_Character_Widget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsExpanded = false;
}

void URZ_OTM_Character_Widget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void URZ_OTM_Character_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void URZ_OTM_Character_Widget::Init(ARZ_PawnAIController* NewAIController)
{
	if (NewAIController)
	{
		ActionListWidget->Init(NewAIController);
	}

	Update();
}

void URZ_OTM_Character_Widget::Update()
{
}

void URZ_OTM_Character_Widget::ToggleOTM_BPI_Implementation()
{
	bIsExpanded = !bIsExpanded;
	WidgetSwitcher->SetActiveWidgetIndex(bIsExpanded);		
}
