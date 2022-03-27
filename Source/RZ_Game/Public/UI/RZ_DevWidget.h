/// RemzDNB
///
///	RZ_DevWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_ComboBoxWidget.h"
//
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_DevWidget.generated.h"

UCLASS()
class RZ_GAME_API URZ_DevWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

private:

	//UPROPERTY(meta = (BindWidget))
	//class URZ_ComboBoxWidget* PresetsComboBox;

	///

	//ARZ_PlayerController* PCOwner;

	UFUNCTION()
	void OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
};
