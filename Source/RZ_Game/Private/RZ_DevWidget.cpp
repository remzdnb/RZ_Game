#include "RZ_DevWidget.h"
#include "Player/RZ_PlayerController.h"
//
#include "Components/ComboBoxString.h"
#include "Game/RZ_GameSettings.h"

void URZ_DevWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	//PCOwner = Cast<ARZ_PlayerController>(GetOwningPlayer());
	//if (PCOwner == nullptr)
		//return;
	
	/*for (auto& RowName : Cast<URZ_BaseGameInstance>(GetGameInstance())->GameSettings->ControlSettingsPresets->GetRowNames())
	{
		PresetComboBox->AddOption(RowName.ToString());
	}
	
	PresetComboBox->SetSelectedOption(PCOwner->GetPlayerConfigPresetName().ToString());
	PresetComboBox->OnSelectionChanged.AddUniqueDynamic(this, &URZ_DevWidget::OnComboBoxSelectionChanged);*/
}

void URZ_DevWidget::OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	//PCOwner->UpdateControlSettings(*SelectedItem);
}