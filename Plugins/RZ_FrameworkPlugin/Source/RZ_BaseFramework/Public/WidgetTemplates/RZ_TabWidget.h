/// RemzDNB
///
///	RZ_TabWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_TabWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTabPressed, uint8, TabID);

class UTextBlock;

UCLASS()
class RZ_BASEFRAMEWORK_API URZ_TabWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	//

	FTabPressed OnTabPressed;

	//

	UFUNCTION()
	void Init(uint8 NewTabID, FString DisplayString);

	UFUNCTION(BlueprintNativeEvent)
	void SetIsSelected_BPN(bool bNewIsSelected);

private:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DisplayText;

	//

	UPROPERTY()
	uint8 TabID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsSelected;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsHovered;
	
	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* StatValueText;
};
