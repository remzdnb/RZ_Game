/// RemzDNB

// InventorySystem plugin
#include "RZ_InventoryMenuWidget.h"
#include "RZ_ItemSlot_Inventory_Widget.h"
#include "RZ_ItemSlot_Crafting_Widget.h"
#include "RZ_InventoryComponent.h"
#include "RZ_ItemSlot_Display_Widget.h"
// Engine
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

void URZ_InventoryMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	InventorySystemSettings = Cast<IRZ_InventorySystemInterface>(UGameplayStatics::GetGameInstance(GetWorld()))
		->GetInventorySystemSettings();
	
	//

	//CreateCraftingSlots();
}

void URZ_InventoryMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URZ_InventoryMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (!InventorySystemSettings) { return; }
	
	if (!CraftingProgressBar) { return; }
	
	if (OwnerInventoryComp)
	{
		if (OwnerInventoryComp->GetCraftingTimer().IsValid())
		{
			const FRZ_ItemSettings* ItemSettings =
				InventorySystemSettings->GetItemSettingsFromTableRow(OwnerInventoryComp->GetCraftedItemName());
			if (ItemSettings)
			{
				const float TimeElapsed = GetWorld()->GetTimerManager().GetTimerElapsed(OwnerInventoryComp->GetCraftingTimer());
				CraftingProgressBar->SetPercent(TimeElapsed / ItemSettings->CraftingTime);
			}
			else
			{
				CraftingProgressBar->SetPercent(0);
			}
		}
		else
		{
			CraftingProgressBar->SetPercent(0);
		}
	}
}

void URZ_InventoryMenuWidget::OnNewSelfInventory(URZ_InventoryComponent* NewSelfInventory)
{
	OwnerInventoryComp = NewSelfInventory;
	if (!OwnerInventoryComp) { return; }

	OwnerInventoryComp->OnNewTargetInventory.AddDynamic(this, &URZ_InventoryMenuWidget::OnNewTargetInventory);

	// Create crafting slots.

	CraftingSlotsContainer->ClearChildren();

	for (const auto& RowName : InventorySystemSettings->ItemSettingsDataTable->GetRowNames())
	{
		const FRZ_ItemSettings* ActorSettings = InventorySystemSettings->GetItemSettingsFromTableRow(RowName);
		if (!ActorSettings) { continue; }
		if (ActorSettings->CraftingComponents.Num() == 0) { continue; }
		
		URZ_ItemSlot_Crafting_Widget* CraftingSlot = Cast<URZ_ItemSlot_Crafting_Widget>(CreateWidget<URZ_ItemSlot_Crafting_Widget>(
			GetWorld(),
			InventorySystemSettings->CraftingSlotWidgetClass
		));
		if (CraftingSlot)
		{
			CraftingSlotsContainer->AddChild(CraftingSlot);
			CraftingSlot->Init(NewSelfInventory, RowName);
		}
	}
	
	// Create self gear slots.

	//SelfGearSlotsContainer->ClearChildren();
	
	// Create self storage widgets.
	
	SelfStorageSlotsContainer->ClearChildren();
	
	/*for (int32 Index = 1; Index < MAXINVENTORYSLOTS; Index++)
	{
		URZ_ItemSlot_Inventory_Widget* StorageSlot = CreateWidget<URZ_ItemSlot_Inventory_Widget>(
			GetWorld(),
			InventorySystemSettings->StorageSlotWidgetClass
		);
		if (StorageSlot)
		{
			SelfStorageSlotsContainer->AddChild(StorageSlot);
			if (OwnerInventoryComp->GetStorageSlots().IsValidIndex(Index))
			{
				StorageSlot->Init(OwnerInventoryComp->GetStorageSlots()[Index].InventorySlotSignature);
				StorageSlot->OnInventorySlotWidgetHovered.AddUniqueDynamic(this, &URZ_InventoryMenuWidget::OnStorageSlotHovered);
			}
		}
	}*/

	//  Create target gear slots.

	//TargetGearSlotsContainer->ClearChildren();
	
	// Create target storage slots.
	
	TargetStorageSlotsContainer->ClearChildren();

	/*for (int32 Index = 1; Index < MAXINVENTORYSLOTS; Index++)
	{
		URZ_ItemSlot_Inventory_Widget* StorageSlot = CreateWidget<URZ_ItemSlot_Inventory_Widget>(
			GetWorld(),
			InventorySystemSettings->StorageSlotWidgetClass
		);
		if (StorageSlot)
		{
			TargetStorageSlotsContainer->AddChild(StorageSlot);
			//StorageSlot->InitAsInventorySlot(InventoryComp->GetTargetInventory(), Index);
			//StorageSlot->OnActorSlotWidgetHovered.AddUniqueDynamic(this, &URZ_InventoryMenuWidget::OnSlotHovered);
		}
	}*/

}

void URZ_InventoryMenuWidget::OnNewTargetInventory(const URZ_InventoryComponent* NewTargetInventory)
{
	if (!NewTargetInventory)
	{
		OnTargetInventoryEnabledBPI(false);
	}
	else
	{
		OnTargetInventoryEnabledBPI(true);
	}
}

void URZ_InventoryMenuWidget::OnStorageSlotHovered(const FRZ_InventorySlotSignature& SlotSignature, bool bNewIsHovered)
{
	if (bNewIsHovered == false)
	{
		HoveredSlotSignature.OwnerInventoryComp = nullptr;
	}
	else
	{
		HoveredSlotSignature = SlotSignature;
	}

	UE_LOG(LogTemp, Display, TEXT("URZ_InventoryMenuWidget::OnSlotHovered "));
	RequestActorDetailsUpdate();
}

void URZ_InventoryMenuWidget::RequestActorDetailsUpdate()
{
	FRZ_InventorySlotSignature SlotSignature;
	
	if (HoveredSlotSignature.OwnerInventoryComp)
	{
		SlotSignature = HoveredSlotSignature;
	}
	else
	{
		if (OwnerInventoryComp->GetEquippedStorageSlotID() != 0)
		{
			SlotSignature = OwnerInventoryComp->GetStorageSlots()[OwnerInventoryComp->GetEquippedStorageSlotID()].InventorySlotSignature;
		}
	}

	if (DetailsSlotSignature.IsEqual(SlotSignature) == false)
	{
		UpdateActorDetails(SlotSignature);
	}
}

void URZ_InventoryMenuWidget::UpdateActorDetails(const FRZ_InventorySlotSignature& SlotSignature)
{
	FRZ_InventorySlotData SlotData;
	URZ_InventoryComponent::GetSlotDataFromSlotSignature(SlotSignature, SlotData);

	if (SlotData.ActorName == "None") { return; }

	const FRZ_ItemSettings* ItemSettings = InventorySystemSettings->GetItemSettingsFromTableRow(SlotData.ActorName);
	if (!ItemSettings) { return; }
	
	ItemDetailsNameText->SetText(FText::FromString(ItemSettings->DisplayName.ToString()));
	
	if (SlotData.SpawnedActor)
	{
		ItemDetailsRendererContainer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ItemDetailsThumbnailImage->SetVisibility(ESlateVisibility::Collapsed);
		
		//ActorRenderer->SpawnNewActor(SlotData.ActorName);
		OnActorDetailsEnabledBPI(true);

		// Create attached slot widgets.
	
		/*IRZ_InventoryOwnerInterface* ItemInventoryOwnerInterface = Cast<IRZ_InventoryOwnerInterface>(SlotData.SpawnedActor);
		if (ItemInventoryOwnerInterface)
		{
			ActorDetailsAttachedSlotsContainer->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			ActorDetailsAttachedSlotsContainer->ClearChildren();

			for (const auto& AttachedSlot : ItemInventoryOwnerInterface->GetInventoryComponent()->GetAttachedSlots())
			{
				URZ_ItemSlot_Inventory_Widget* AttachedSlotWidget = CreateWidget<URZ_ItemSlot_Inventory_Widget>(
					GetWorld(),
					InventorySystemSettings->AttachmentSlotWidgetClass
				);
				if (AttachedSlotWidget)
				{
					ActorDetailsAttachedSlotsContainer->AddChild(AttachedSlotWidget);
					AttachedSlotWidget->Init(SlotSignature);
				}
			}
		}
		else
		{
			ActorDetailsAttachedSlotsContainer->SetVisibility(ESlateVisibility::Hidden);
		}*/
	}
	else
	{
		ActorDetailsAttachedSlotsContainer->SetVisibility(ESlateVisibility::Hidden);
		ItemDetailsRendererContainer->SetVisibility(ESlateVisibility::Collapsed);
		ItemDetailsThumbnailImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ItemDetailsThumbnailImage->SetBrushFromTexture(ItemSettings->ThumbnailTexture);
		ItemDetailsThumbnailImage->SetRenderScale(FVector2D(ItemSettings->ThumbnailRenderScale));
	}

	OnNewActorDetailsBPI();

}
