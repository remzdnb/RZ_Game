/// RemzDNB

// InventorySystem plugin
#include "RZ_ActorPanelWidget.h"
#include "RZ_ItemSlot_Inventory_Widget.h"
#include "RZ_ItemSlot_Crafting_Widget.h"
#include "RZ_Actor2DRenderer.h"
#include "RZ_InventoryComponent.h"
#include "RZ_PerceptionComponent.h"
// Engine
#include "RZ_AbilitySystemComponent.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Core/RZ_GameInstance.h"
#include "Kismet/GameplayStatics.h"

void URZ_ActorPanelWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PawnRendererImage->SetBrushFromMaterial(ItemRendererMaterial);
}

void URZ_ActorPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URZ_ActorPanelWidget::Update(APawn* PawnRef, ARZ_Actor2DRenderer* ItemRenderer)
{
	PawnNameText->SetText(FText::FromString(GetNameSafe(PawnRef)));

	//
	
	UAbilitySystemComponent* ASComp = Cast<UAbilitySystemComponent>(
		PawnRef->GetComponentByClass(UAbilitySystemComponent::StaticClass())
	);
	HealthAttributeWidget->Init(ASComp, URZ_AttributeSet::GetHealthAttribute());
	StaminaAttributeWidget->Init(ASComp, URZ_AttributeSet::GetShieldAttribute());

	//
	
	const IRZ_InventoryItemInterface* PawnItemInterface = Cast<IRZ_InventoryItemInterface>(PawnRef);
	if (PawnItemInterface)
	{
		PawnRendererImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PawnItemImage->SetVisibility(ESlateVisibility::Hidden);
		
		if (ItemRenderer)
		{
			ItemRenderer->SpawnNewActor(PawnItemInterface->GetItemName());
		}

		const URZ_InventoryComponent* PawnInventoryComp = Cast<URZ_InventoryComponent>(
			PawnRef->GetComponentByClass(URZ_InventoryComponent::StaticClass())
		);
		if (PawnInventoryComp)
		{
			StorageSlotContainerWidget->Update(PawnInventoryComp, ERZ_InventorySlotType::Storage);
			AttachedSlotContainerWidget->Update(PawnInventoryComp, ERZ_InventorySlotType::Attached);
		}
	}
	else
	{
		PawnRendererImage->SetVisibility(ESlateVisibility::Hidden);
		PawnItemImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		/*const IRZ_InventorySystemInterface* InventorySystemInterface =
			Cast<IRZ_InventorySystemInterface>(GetWorld()->GetGameInstance());
		const FRZ_ItemSettings* ItemSettings =
			InventorySystemInterface->GetInventorySystemSettings()->GetItemSettingsFromTableRow(SlotData.ActorName);
		if (ItemSettings)
		{
			PawnItemImage->SetBrushFromTexture(ItemSettings->ThumbnailTexture);
			PawnItemImage->SetRenderScale(FVector2D(ItemSettings->ThumbnailRenderScale));
		}*/
	}
	

}
