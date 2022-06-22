/// RemzDNB
///
/// RZ_PowerWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "RZ_PowerComponent.h"
#include "Blueprint/UserWidget.h"
#include "RZ_PowerWidget.generated.h"

class UTextBlock;

UCLASS()
class RZM_POWERSYSTEM_API URZ_PowerWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	explicit URZ_PowerWidget(const FObjectInitializer& ObjectInitializer);
	
	//
	
	UFUNCTION()
	void Update(URZ_PowerComponent* NewPowerComp);

private:

	URZ_PowerComponent* PowerComp;
	
	//

	UPROPERTY(VisibleAnywhere, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* ProducedPowerText;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* ConsumedPowerText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget, AllowPrivateAccess = "true"))
	UTextBlock* GridIDText;

};
