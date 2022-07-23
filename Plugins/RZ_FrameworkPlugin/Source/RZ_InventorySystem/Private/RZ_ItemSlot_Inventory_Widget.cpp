/// RemzDNB

// InventorySystem Plugin
#include "RZ_ItemSlot_Inventory_Widget.h"
#include "RZ_InventorySlotDDOperation.h"
#include "RZ_InventoryComponent.h"
// Engine
#include "RZ_ItemSlot_Crafting_Widget.h"
#include "RZ_ItemSlot_Display_Widget.h"
#include "Components/PanelWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"

URZ_ItemSlot_Inventory_Widget::URZ_ItemSlot_Inventory_Widget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void URZ_ItemSlot_Inventory_Widget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	InventorySystemSettings = Cast<IRZ_InventorySystemInterface>(GetWorld()->GetGameInstance())
		->GetInventorySystemSettings();
}

void URZ_ItemSlot_Inventory_Widget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URZ_ItemSlot_Inventory_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	Update();
}

void URZ_ItemSlot_Inventory_Widget::Init(const FRZ_InventorySlotSignature& NewInventorySlotSignature)
{
	SlotSignature = NewInventorySlotSignature;
//	SlotSignature.OwnerInventoryComp->OnInventoryUpdated.AddUniqueDynamic(this, &URZ_ItemSlot_Inventory_Widget::Update);
}

void URZ_ItemSlot_Inventory_Widget::Update()
{
	//GEngine->AddOnScreenDebugMessage(484, 3.0f, FColor::Red, "URZ_ItemSlot_Inventory_Widget::Update 0");
	if (!SlotSignature.OwnerInventoryComp) { return; }
	
	FRZ_InventorySlotData InventorySlotData;
	URZ_InventoryComponent::GetSlotDataFromSlotSignature(SlotSignature, InventorySlotData);
	
	// Empty slot.
	
	if (InventorySlotData.ActorName == "None")
	{
		if (NameText) { NameText->SetVisibility(ESlateVisibility::Hidden); }
		if (ThumbnailImage) { ThumbnailImage->SetVisibility(ESlateVisibility::Hidden); }
		if (StackSizeContainer) { StackSizeContainer->SetVisibility(ESlateVisibility::Hidden); }
		return;
	}
	
	// Valid item.
	
	const FRZ_ItemSettings* ItemSettings = InventorySystemSettings->GetItemSettingsFromTableRow(InventorySlotData.ActorName);
	if (!ItemSettings) { return; }
	
	if (NameText)
	{
		NameText->SetVisibility(ESlateVisibility::SelfHitTestInvisible); 
		NameText->SetText(FText::FromString(ItemSettings->DisplayName.ToString()));
	}

	if (ThumbnailImage)
	{
		ThumbnailImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible); 
		ThumbnailImage->SetBrushFromTexture(ItemSettings->ThumbnailTexture);
		ThumbnailImage->SetRenderScale(FVector2D(ItemSettings->ThumbnailRenderScale));
		ThumbnailImage->SetRenderTransformAngle(ItemSettings->ThumbnailRenderAngle);
	}

	if (ItemSettings->MaxStackSize > 1)
	{
		if (StackSizeContainer) { StackSizeContainer->SetVisibility(ESlateVisibility::SelfHitTestInvisible); }
		
		if (StackSizeText)
		{
			StackSizeText->SetText(FText::FromString(FString::FromInt(InventorySlotData.StackSize)));
		}
	}
	else
	{
		if (StackSizeContainer) { StackSizeContainer->SetVisibility(ESlateVisibility::Hidden); }
	}

	// Selection

	if (SlotSignature.SlotType == ERZ_InventorySlotType::Storage)
	{
		//UE_LOG(LogTemp, Warning, TEXT("URZ_ItemSlot_Inventory_Widget::Update x"));
		SlotSignature.SlotID == SlotSignature.OwnerInventoryComp->GetEquippedStorageSlotID() ? OnSlotSelectionBPI(true) : OnSlotSelectionBPI(false);
	}
}

#pragma region +++ UMG events ...

void URZ_ItemSlot_Inventory_Widget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	OnInventorySlotWidgetHovered.Broadcast(SlotSignature, true);
}

void URZ_ItemSlot_Inventory_Widget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	OnInventorySlotWidgetHovered.Broadcast(SlotSignature, false);
}

FReply URZ_ItemSlot_Inventory_Widget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	//if (ActorSlotWidgetType == ERZ_InventorySlotType::CraftActor)
	//{
		//WidgetOwnerInventoryComp->StartCraftingItem(DisplayedActorName);
	//}
	
	FReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;

	DebugSlotInfos();

	return Reply;
}

FReply URZ_ItemSlot_Inventory_Widget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	
	if (SlotSignature.OwnerInventoryComp)
	{
		FRZ_InventorySlotData SlotData;
		URZ_InventoryComponent::GetSlotDataFromSlotSignature(SlotSignature, SlotData);

		if (SlotSignature.SlotType == ERZ_InventorySlotType::Storage)
		{
			SlotSignature.OwnerInventoryComp->EquipStorageItem(SlotSignature.SlotID);
		}
	}

	return FReply::Handled();
}

void URZ_ItemSlot_Inventory_Widget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                                   UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (InventorySystemSettings == nullptr) { return; }
	//if (!SlotInfo.AttachedActor) { return; }

	if (!SlotSignature.OwnerInventoryComp) { return; }
	
	FRZ_InventorySlotData InventorySlotData;
	URZ_InventoryComponent::GetSlotDataFromSlotSignature(SlotSignature, InventorySlotData);
	
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(URZ_InventorySlotDDOperation::StaticClass());
	URZ_InventorySlotDDOperation* const DDOperation = Cast<URZ_InventorySlotDDOperation>(OutOperation);
	if (DDOperation)
	{
		DDOperation->Pivot = EDragPivot::MouseDown;
		//DDOperation->Init(this);

		URZ_ItemSlot_Display_Widget* DragWidget = CreateWidget<URZ_ItemSlot_Display_Widget>(
			GetWorld(),
			InventorySystemSettings->DragSlotWidgetClass
		);
		if (DragWidget)
		{
			DragWidget->Update(InventorySlotData.ActorName, InventorySlotData.StackSize);
			DDOperation->DefaultDragVisual = DragWidget;
		}

		ThumbnailImage->SetVisibility(ESlateVisibility::Hidden);
		GetOwningPlayer()->CurrentMouseCursor = EMouseCursor::None;
	}
}

void URZ_ItemSlot_Inventory_Widget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
}

void URZ_ItemSlot_Inventory_Widget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
}

bool URZ_ItemSlot_Inventory_Widget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                       UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	/*const URZ_InventorySlotDDOperation* InDragOperation = Cast<URZ_InventorySlotDDOperation>(InOperation);
	if (InDragOperation && WidgetOwnerInventoryComp)
	{
		URZ_InventoryComponent::NewSlotOperation(
			InDragOperation->InventorySlotWidget->GetOwnerInventoryComp(),
			InDragOperation->InventorySlotWidget->GetSlotID(),
			WidgetOwnerInventoryComp,
			SlotID
		);
	}*/

	//InventoryComp->OnInventoryUpdated.Broadcast();
	
	GetOwningPlayer()->CurrentMouseCursor = EMouseCursor::Crosshairs;
	//ItemImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	return true;
}

void URZ_ItemSlot_Inventory_Widget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	ThumbnailImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	GetOwningPlayer()->CurrentMouseCursor = EMouseCursor::Crosshairs;
}

#pragma endregion

void URZ_ItemSlot_Inventory_Widget::DebugSlotInfos()
{
	if (!InventorySystemSettings->bDebugActorSlotWidget) { return; }
	if (!SlotSignature.OwnerInventoryComp) { return; }
	
	FRZ_InventorySlotData InventorySlotData;
	URZ_InventoryComponent::GetSlotDataFromSlotSignature(SlotSignature, InventorySlotData);
	
	UE_LOG(LogTemp, Display, TEXT(" "));
	UE_LOG(LogTemp, Display, TEXT("----------- URZ_ItemSlot_Display_Widget::DebugSlotData --------------------------"));
	
	UE_LOG(LogTemp, Display, TEXT(" "));
	UE_LOG(LogTemp, Display, TEXT("--- Widget Slot Signature ---"));
	UE_LOG(LogTemp, Display, TEXT(" "));

	const FString OwnerInventoryCompString = GetNameSafe(SlotSignature.OwnerInventoryComp);
	UE_LOG(LogTemp, Display, TEXT("OwnerInventoryComp == %s"), *OwnerInventoryCompString);
	
	FString TypeString = "";
	if (SlotSignature.SlotType == ERZ_InventorySlotType::Attached) { TypeString = "Type == Attachment"; }
	else if (SlotSignature.SlotType == ERZ_InventorySlotType::Storage) { TypeString = "Type == Storage"; }
	UE_LOG(LogTemp, Display, TEXT("%s"), *TypeString);
	
	const FString SlotIDString = "ID == " + FString::FromInt(SlotSignature.SlotID);
	UE_LOG(LogTemp, Display, TEXT("%s"), *SlotIDString);

	UE_LOG(LogTemp, Display, TEXT(" "));
	UE_LOG(LogTemp, Display, TEXT("--- Item Slot Signature ---"));
	UE_LOG(LogTemp, Display, TEXT(" "));

	UE_LOG(LogTemp, Display, TEXT("OwnerInventoryComp == %s"),
	       *GetNameSafe(InventorySlotData.InventorySlotSignature.OwnerInventoryComp)
	);
	UE_LOG(LogTemp, Display, TEXT("ID == %s"), *FString::FromInt(InventorySlotData.InventorySlotSignature.SlotID));

	FString ItemSignatureType = "";
	if (InventorySlotData.InventorySlotSignature.SlotType == ERZ_InventorySlotType::Attached) { ItemSignatureType = "Type == Attachment"; }
	else if (InventorySlotData.InventorySlotSignature.SlotType == ERZ_InventorySlotType::Storage) { ItemSignatureType = "Type == Storage"; }
	UE_LOG(LogTemp, Display, TEXT("%s"), *ItemSignatureType);
	
	UE_LOG(LogTemp, Display, TEXT(" "));
	UE_LOG(LogTemp, Display, TEXT("--- Item Slot Data ---"));
	UE_LOG(LogTemp, Display, TEXT(" "));

	const FString SlotNameString = "ActorName == " + InventorySlotData.ActorName.ToString();
	UE_LOG(LogTemp, Display, TEXT("%s"), *SlotNameString)

	const FString StackSizeString = "StackSize == " + FString::FromInt(InventorySlotData.StackSize);
	UE_LOG(LogTemp, Display, TEXT("%s"), *StackSizeString)

	if (InventorySlotData.SpawnedActor)
	{
		UE_LOG(LogTemp, Display, TEXT("SpawnedActor Ref == %s"), *InventorySlotData.SpawnedActor->GetName());
	}

	UE_LOG(LogTemp, Display, TEXT(" "));
	UE_LOG(LogTemp, Display, TEXT("---------------------------------------------------------------------------------"));
}

