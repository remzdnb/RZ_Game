/// RemzDNB
///
///	RZ_ComboBoxWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_ComboBoxWidget.generated.h"

UCLASS()
class RZM_UMGTEMPLATES_API URZ_ComboBoxWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	///
	
private:

	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ComboBox;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TitleText;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image;

	///

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString TitleString;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTexture2D* ImageTexture;
};
