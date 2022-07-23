/// RemzDNB
///
///	RZ_ActorPanelWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_AttributeWidget.h"
#include "RZ_Game.h"
#include "RZ_InventorySystem.h"
#include "RZ_Actor2DRenderer.h"
#include "RZ_ItemSlot_Container_Widget.h"
#include "Blueprint/UserWidget.h"
#include "RZ_ActorPanelWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class RZ_GAME_API URZ_ActorPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	//

	UFUNCTION()
	void Update(APawn* PawnRef, ARZ_Actor2DRenderer* ItemRenderer);

	//UFUNCTION()
	//void UpdateFromItemName();

	//UFUNCTION()
	//void UpdateFromActor();
	
	//

private:
	
	URZ_InventoryComponent* OwnerInventoryComp;
	ARZ_Actor2DRenderer* PawnRenderer;
	AActor* RenderedActor; // ?

	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* ItemRendererMaterial;

	//

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PawnNameText;

	UPROPERTY(meta = (BindWidget))
	URZ_AttributeWidget* HealthAttributeWidget;

	UPROPERTY(meta = (BindWidget))
	URZ_AttributeWidget* StaminaAttributeWidget;

	UPROPERTY(meta = (BindWidget))
	UImage* PawnRendererImage;
	
	UPROPERTY(meta = (BindWidget))
	UImage* PawnItemImage;
	
	UPROPERTY(meta = (BindWidget))
	URZ_ItemSlot_Container_Widget* StorageSlotContainerWidget;
	
	UPROPERTY(meta = (BindWidget))
	URZ_ItemSlot_Container_Widget* AttachedSlotContainerWidget;
};



