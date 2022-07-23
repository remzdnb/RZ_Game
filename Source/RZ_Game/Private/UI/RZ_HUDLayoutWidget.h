/// RemzDNB
///
///	RZ_HUDLayoutWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
#include "RZ_InventorySystem.h"
#include "Blueprint/UserWidget.h"
#include "RZ_HUDLayoutWidget.generated.h"

class URZ_ActorPanelWidget;
class UTextBlock;
class UImage;

UCLASS()
class RZ_GAME_API URZ_HUDLayoutWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	//

	UFUNCTION()
	void OnNewCharacterSelected(ARZ_Character* NewPossessedCharacter);

	UFUNCTION()
	void OnNewTargetPawnSelected(APawn* NewSelectedPawn);

	UFUNCTION()
	void OnNewOwnInventoryItemEquipped(const FRZ_InventorySlotData& SlotData);

private:

	const URZ_GameSettings* RZGameSettings;
	const URZ_InventorySystemSettings* InventorySystemSettings;
	URZ_InventoryComponent* OwnerInventoryComp;

	//
	
	UPROPERTY()
	ARZ_Actor2DRenderer* SelectedCharacterRenderer;

	UPROPERTY()
	ARZ_Actor2DRenderer* SelectedTargetPawnRenderer;

	UPROPERTY()
	ARZ_Actor2DRenderer* SelectedItemRenderer;

	//

	UPROPERTY(meta = (BindWidget))
	URZ_ActorPanelWidget* SelectedCharacterPanel;
	
	UPROPERTY(meta = (BindWidget))
	URZ_ActorPanelWidget* SelectedTargetPawnPanel;

	UPROPERTY(meta = (BindWidget))
	URZ_ActorPanelWidget* SelectedItemPanel;
};



