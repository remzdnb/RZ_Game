/// RemzDNB

#include "RZ_HUDLayoutWidget.h"
#include "RZ_ActorPanelWidget.h"
#include "RZ_ItemSlot_Display_Widget.h"
#include "Character/RZ_Character.h"
#include "Core/RZ_GameInstance.h"
#include "Core/RZ_PlayerController.h"
// InventorySystem plugin
#include "RZ_Actor2DRenderer.h"
#include "RZ_InventoryComponent.h"
#include "RZ_ItemSlot_Display_Widget.h"
// Engine
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Core/RZ_PlayerController.h"
#include "Kismet/GameplayStatics.h"

void URZ_HUDLayoutWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RZGameSettings = Cast<URZ_GameInstance>(GetWorld()->GetGameInstance())->GetGameSettings();
	
	InventorySystemSettings = Cast<IRZ_InventorySystemInterface>(UGameplayStatics::GetGameInstance(GetWorld()))
		->GetInventorySystemSettings();
	
	const FActorSpawnParameters SpawnParameters;
	
	SelectedCharacterRenderer = GetWorld()->SpawnActor<ARZ_Actor2DRenderer>(
		RZGameSettings->SelectedCharacterRenderer,
		FVector(-1000000.0f, 0.0f, 10000.0f),
		FRotator(0.0f, 180.0f, 0.0f),
		SpawnParameters
	);
	SelectedTargetPawnRenderer = GetWorld()->SpawnActor<ARZ_Actor2DRenderer>(
		RZGameSettings->SelectedTargetPawnRendererClass,
		FVector(-1000000.0f, -1000000.0f, 10000.0f),
		FRotator(0.0f, 180.0f, 0.0f),
		SpawnParameters
	);
	SelectedItemRenderer = GetWorld()->SpawnActor<ARZ_Actor2DRenderer>(
		RZGameSettings->SelectedItemRendererClass,
		FVector(1000000.0f, 1000000.0f, 10000.0f),
		FRotator::ZeroRotator,
		SpawnParameters
	);

	Cast<ARZ_PlayerController>(GetOwningPlayer())->OnNewCharacterSelected.AddUniqueDynamic(
		this, &URZ_HUDLayoutWidget::OnNewCharacterSelected
	);
	Cast<ARZ_PlayerController>(GetOwningPlayer())->OnNewTargetPawnSelected.AddUniqueDynamic(
		this, &URZ_HUDLayoutWidget::OnNewTargetPawnSelected
	);
}

void URZ_HUDLayoutWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void URZ_HUDLayoutWidget::OnNewCharacterSelected(ARZ_Character* NewPossessedCharacter)
{
	OwnerInventoryComp = Cast<URZ_InventoryComponent>(
		NewPossessedCharacter->GetComponentByClass(URZ_InventoryComponent::StaticClass())
	);
	if (!OwnerInventoryComp) { return; }

	SelectedCharacterPanel->Update(NewPossessedCharacter, SelectedCharacterRenderer);

	IRZ_InventoryItemInterface* ItemInterface = Cast<IRZ_InventoryItemInterface>(NewPossessedCharacter);
	if (ItemInterface)
	{
		SelectedCharacterRenderer->SpawnNewActor(ItemInterface->GetItemName());
	}

	OwnerInventoryComp->OnNewStorageItemEquipped.AddUniqueDynamic(this, &URZ_HUDLayoutWidget::OnNewOwnInventoryItemEquipped);
}

void URZ_HUDLayoutWidget::OnNewTargetPawnSelected(APawn* NewSelectedPawn)
{
	OwnerInventoryComp = Cast<URZ_InventoryComponent>(
		NewSelectedPawn->GetComponentByClass(URZ_InventoryComponent::StaticClass())
	);
	if (!OwnerInventoryComp) { return; }

	SelectedTargetPawnPanel->Update(NewSelectedPawn, SelectedTargetPawnRenderer);

	IRZ_InventoryItemInterface* ItemInterface = Cast<IRZ_InventoryItemInterface>(NewSelectedPawn);
	if (ItemInterface)
	{
		SelectedTargetPawnRenderer->SpawnNewActor(ItemInterface->GetItemName());
	}
}

void URZ_HUDLayoutWidget::OnNewOwnInventoryItemEquipped(const FRZ_InventorySlotData& SlotData)
{
	if (OwnerInventoryComp) { return; }

	if (SlotData.InventorySlotSignature.SlotID != 0)
	{
		//SelectedItemPanel->Update()
		
		const IRZ_InventoryItemInterface* ItemInterface = Cast<IRZ_InventoryItemInterface>(
			SlotData.SpawnedActor
		);
		if (ItemInterface)
		{
			SelectedItemRenderer->SpawnNewActor(SlotData.ActorName);
		}
	}
}
