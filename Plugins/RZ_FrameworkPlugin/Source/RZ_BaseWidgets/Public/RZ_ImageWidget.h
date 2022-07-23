/// RemzDNB
///
///	RZ_ImageWidget
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_ImageWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class RZ_BASEWIDGETS_API URZ_ImageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UFUNCTION()
	void Update(const FString& DisplayString, const UMaterialInterface* TextureToDisplay) const;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString DisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	uint8 TabID;
	
	//

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Image;
	
};
