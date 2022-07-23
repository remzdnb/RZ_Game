/// RemzDNB
///
///	RZ_AttributeWidget
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "GameplayEffectTypes.h"

#include "RZ_AttributeSet.h"
#include "RZ_BaseWidgets.h"
#include "Blueprint/UserWidget.h"
#include "RZ_AttributeWidget.generated.h"

class UAbilitySystemComponent;
class UTextBlock;
class UImage;

UCLASS()
class RZ_ATTRIBUTESYSTEM_API URZ_AttributeWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	//

	void Init(UAbilitySystemComponent* NewASComp, FGameplayAttribute NewAttribute);
	void Update();

private:
	
	UPROPERTY(meta = (BindWidget))
	URZ_ProgressBarWidget* ProgressBar;

	//

	UAbilitySystemComponent* ASComp;
	FGameplayAttribute Attribute;
	FDelegateHandle UpdateDelegateHandle; 

	//
	
	void OnAttributeChanged(const FOnAttributeChangeData& Data);
};



