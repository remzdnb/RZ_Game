/// RemzDNB

// InventorySystem Plugin
#include "RZ_InventorySlotWidget.h"
#include "RZ_InventorySlotDDOperation.h"
// Engine
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"

URZ_InventorySlotWidget::URZ_InventorySlotWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void URZ_InventorySlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InventorySystemModuleSettings = Cast<IRZ_InventorySystemModuleInterface>(
			UGameplayStatics::GetGameInstance(GetWorld()))
		->GetInventorySystemModuleSettings();
}

void URZ_InventorySlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URZ_InventorySlotWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void URZ_InventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void URZ_InventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

FReply URZ_InventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	DebugSlotData();
	
	FReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	return Reply;
}

void URZ_InventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (InventorySystemModuleSettings == nullptr) { return; }
	if (SlotSettings.ItemName == "Empty") { return; }
	
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(URZ_InventorySlotDDOperation::StaticClass());
	URZ_InventorySlotDDOperation* const DDOperation = Cast<URZ_InventorySlotDDOperation>(OutOperation);
	if (DDOperation)
	{
		DDOperation->Pivot = EDragPivot::MouseDown;
		DDOperation->Init(this);

		URZ_InventorySlotWidget* DragWidget = CreateWidget<URZ_InventorySlotWidget>(
			GetWorld(),
			InventorySystemModuleSettings->InventorySlot_Menu_WidgetClass
		);
		if (DragWidget)
		{
			DragWidget->UpdateAsDragSlot(SlotSettings.ItemName);
			DDOperation->DefaultDragVisual = DragWidget;
		}

		ItemImage->SetVisibility(ESlateVisibility::Hidden);
		GetOwningPlayer()->CurrentMouseCursor = EMouseCursor::None;
	}
}

void URZ_InventorySlotWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
}

void URZ_InventorySlotWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
}

bool URZ_InventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	URZ_InventorySlotDDOperation* const InDragOperation = Cast<URZ_InventorySlotDDOperation>(InOperation);
	if (InDragOperation)
	{
		InventoryComponent->SwapItems(SlotSettings.SlotID, InDragOperation->ItemSlotWidget->GetSlotSettings().SlotID);
	}
	
	//GetOwningPlayer()->CurrentMouseCursor = EMouseCursor::Default;
	//ItemImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	return true;
}

void URZ_InventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	ItemImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void URZ_InventorySlotWidget::UpdateFromItemSettings(URZ_InventoryComponent* NewInventoryComponent, FRZ_InventorySlotSettings& NewSlotSettings)
{
	InventoryComponent = NewInventoryComponent;
	SlotSettings = NewSlotSettings;
	
	if (!InventorySystemModuleSettings) { return; }

	const FRZ_InventoryItemSettings* ItemSettings =
		InventorySystemModuleSettings->GetInventoryItemSettingsFromDataTable(SlotSettings.ItemName);
	
	if (SlotSettings.ItemName == "Empty" || !ItemSettings)
	{
		ItemImage->SetVisibility(ESlateVisibility::Hidden);
		CornerImage->SetVisibility(ESlateVisibility::Hidden);
		ItemNameText->SetVisibility(ESlateVisibility::Hidden);
		CornerText->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		ItemImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CornerImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ItemNameText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		CornerText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		ItemNameText->SetText(FText::FromString(ItemSettings->DisplayName.ToString()));
		ItemImage->SetBrushFromTexture(ItemSettings->ThumbnailTexture);
		ItemImage->SetBrushSize(FVector2D(ItemSettings->ThumbnailSize.X, ItemSettings->ThumbnailSize.Y));
	}
}

void URZ_InventorySlotWidget::UpdateAsDragSlot(FName NewDataTableRowName)
{
	if (!InventorySystemModuleSettings) { return; }

	const FRZ_InventoryItemSettings* ItemSettings =
		InventorySystemModuleSettings->GetInventoryItemSettingsFromDataTable(NewDataTableRowName);
	
	ItemImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	ItemImage->SetBrushFromTexture(ItemSettings->ThumbnailTexture);
	
	ItemNameText->SetVisibility(ESlateVisibility::Hidden);
	FillImage->SetVisibility(ESlateVisibility::Hidden);
	FrameImage->SetVisibility(ESlateVisibility::Hidden);
	CornerImage->SetVisibility(ESlateVisibility::Hidden);
	CornerText->SetVisibility(ESlateVisibility::Hidden);
}

/*void URZ_InventorySlotWidget::UpdateFromItemRef(const ARZ_Item* Item) const
{
	if (Item == nullptr)
		return;

	if (Item->GetItemData() == nullptr)
		return;
	
	ItemNameText->SetText(FText::FromString(Item->GetItemData()->DisplayName.ToString()));
	ItemImage->SetBrushFromTexture(Item->GetItemData()->ThumbnailTexture);
}*/

void URZ_InventorySlotWidget::DebugSlotData()
{
	if (!InventorySystemModuleSettings) { return; }
	if (!InventorySystemModuleSettings->bDebugInventorySlotWidget) { return; }

	UE_LOG(LogTemp, Display, TEXT(" "));
	UE_LOG(LogTemp, Display,
	       TEXT("----------- URZ_ItemSlotWidget::DebugSlotData --------------------------------------"));

	if (SlotSettings.ItemActor)
	{
		UE_LOG(LogTemp, Display, TEXT("URZ_ItemSlotWidget::DebugSlotData - ItemActor Ref : %s"),
		       *SlotSettings.ItemActor->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("URZ_ItemSlotWidget::DebugSlotData - ItemActor Ref : nullptr"));
	}

	FString SlotIDString = "URZ_ItemSlotWidget::DebugSlotData - Slot ID : " + FString::FromInt(SlotSettings.SlotID);
	UE_LOG(LogTemp, Display, TEXT("%s"), *SlotIDString);

	FString SlotNameString = "URZ_ItemSlotWidget::DebugSlotData - Item name : " + SlotSettings.ItemName.ToString();
	UE_LOG(LogTemp, Display, TEXT("%s"), *SlotNameString);

	UE_LOG(LogTemp, Display, TEXT(" "));
}

