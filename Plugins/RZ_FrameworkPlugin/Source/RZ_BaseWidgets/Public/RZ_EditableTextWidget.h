#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_EditableTextWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTextValidated, const FText&, Text);

UCLASS()
class RZ_BASEWIDGETS_API URZ_EditableTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION() const FText GetText();
	UFUNCTION() void SetText(const FText& Text);
	UFUNCTION() void OnTextCommited(const FText& Text, ETextCommit::Type CommitMethod);
	UFUNCTION() void OnTextChanged(const FText& Text);

	UPROPERTY(BlueprintAssignable) FTextValidated OnTextValidated;

#pragma region +++++ BP Exposed Properties ...

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString DefaultTitle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString DefaultText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTexture2D* DefaultImage;

#pragma endregion

#pragma region +++++ Binded Widgets ...

private:

	UPROPERTY(meta = (BindWidget)) class UTextBlock* TitleText;
	UPROPERTY(meta = (BindWidget)) class UEditableText* EditableText;
	UPROPERTY(meta = (BindWidget)) class UImage* Image;

#pragma endregion

};
