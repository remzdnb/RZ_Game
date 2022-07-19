/// RemzDNB
///
///	RZ_InventoryMenuWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_InventorySystem.h"
#include "Blueprint/UserWidget.h"
#include "RZ_InventoryMenuWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class RZ_INVENTORYSYSTEM_API URZ_InventoryMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	
	const URZ_InventorySystemSettings* InventorySystemSettings;
	
	URZ_InventoryComponent* OwnerInventoryComp;
	ARZ_Item2DRenderer* ActorRenderer;
	AActor* RenderedActor;
	
	//

	FRZ_InventorySlotSignature HoveredSlotSignature;
	FRZ_InventorySlotSignature DetailsSlotSignature;

	//

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* SelfStorageSlotsContainer;
	
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* TargetStorageSlotsContainer;

	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UPanelWidget* SelfAttachedSlotsContainer;

	UPROPERTY(meta = (BindWidget, OptionalWidget))
	UPanelWidget* TargetAttachedSlotsContainer;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* CraftingSlotsContainer;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* CraftingQueueSlotsContainer;
	
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* CraftingProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemDetailsNameText;
	
	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ItemDetailsRendererContainer;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemDetailsThumbnailImage;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ActorDetailsAttachedSlotsContainer;
	
public:
	
	UFUNCTION()
	void OnNewSelfInventory(URZ_InventoryComponent* NewSelfInventory);

	UFUNCTION()
	void OnNewTargetInventory(const URZ_InventoryComponent* NewTargetInventory);

	//

	UFUNCTION(BlueprintImplementableEvent)
	void OnActorDetailsEnabledBPI(bool bNewIsEnabled);

	UFUNCTION(BlueprintImplementableEvent)
	void OnNewActorDetailsBPI();

	UFUNCTION(BlueprintImplementableEvent)
	void OnTargetInventoryEnabledBPI(bool bNewIsEnabled);
	
private:
	
	//

	UFUNCTION()
	void OnStorageSlotHovered(const FRZ_InventorySlotSignature& SlotSignature, bool bNewIsHovered);

	UFUNCTION()
	void RequestActorDetailsUpdate();
	
	UFUNCTION()
	void UpdateActorDetails(const FRZ_InventorySlotSignature& SlotSignature);
	
};



