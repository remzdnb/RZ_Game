/// RemzDNB
///
///	InventorySlotWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_InventorySystem.h"
//
#include "Blueprint/UserWidget.h"
#include "RZ_ItemSlot_Crafting_Widget.generated.h"

class UImage;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCraftingSlotPressedDelegate, const FName&, ActorToCraftName);

UCLASS()
class RZ_INVENTORYSYSTEM_API URZ_ItemSlot_Crafting_Widget : public UUserWidget
{
	GENERATED_BODY()

public:

	URZ_ItemSlot_Crafting_Widget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	//

	FCraftingSlotPressedDelegate OnCraftingSlotPressed;

	//

	UFUNCTION()
	void Init(URZ_InventoryComponent* NewOwnerInventoryComp, const FName& NewActorToCraftName);

	UFUNCTION()
	void Update();

	//
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnCraftableStatusUpdatedBPI(bool bNewIsCraftable);

private:
	
	const URZ_InventorySystemSettings* InventorySystemSettings;
	
	//

	UPROPERTY()
	URZ_InventoryComponent* OwnerInventoryComp;
	
	UPROPERTY()
	FName ActorToCraftName;

	//

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* ActorNameText;

	UPROPERTY(meta = (BindWidget, AllowPrivateAccess = "true"))
	UImage* ActorThumbnailImage;

	UPROPERTY(meta = (BindWidget, OptionalWidget, AllowPrivateAccess = "true"))
	UPanelWidget* CraftingComponentSlotsContainer;
};