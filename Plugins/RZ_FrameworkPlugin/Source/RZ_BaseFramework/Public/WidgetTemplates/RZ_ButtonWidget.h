#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_ButtonWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FButtonPressed, uint8, TabID);

UCLASS()
class RZ_BASEFRAMEWORK_API URZ_ButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void NativeOnInitialized() override;

	UFUNCTION() void Init(uint8 NewTabID, FName TabName);

	UFUNCTION(BlueprintNativeEvent) void SetIsSelected_BPN(bool bNewIsSelected);

	UPROPERTY(BlueprintAssignable) FButtonPressed OnButtonPressed;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString DisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	uint8 TabID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsSelected;

	//

	UFUNCTION() void OnPress();

#pragma region +++++ Binded Widgets ...

private:

	UPROPERTY(meta = (BindWidget)) class UTextBlock* Text;
	UPROPERTY(meta = (BindWidget)) class UButton* Button;

#pragma endregion
	
};
