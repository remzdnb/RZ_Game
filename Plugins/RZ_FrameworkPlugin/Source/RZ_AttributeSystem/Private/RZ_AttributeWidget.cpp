/// RemzDNB

#include "RZ_AttributeWidget.h"
#include "RZ_AttributeSet.h"
// Common
#include "RZ_ProgressBarWidget.h"
// Engine
#include "AbilitySystemComponent.h"

void URZ_AttributeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void URZ_AttributeWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void URZ_AttributeWidget::Init(UAbilitySystemComponent* NewASComp, FGameplayAttribute NewAttribute)
{
	ASComp = NewASComp;
	Attribute = NewAttribute;
	UpdateDelegateHandle.Reset();
	
	if (ASComp)
	{
		UpdateDelegateHandle = ASComp->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(
			this, &URZ_AttributeWidget::OnAttributeChanged
		);
	}

	Update();
}

void URZ_AttributeWidget::Update()
{
	if (ASComp)
	{
		ProgressBar->Update(
			ASComp->GetNumericAttribute(Attribute),
			1000.0f,
			FString::SanitizeFloat(ASComp->GetNumericAttribute(Attribute))
		);
	}
}

void URZ_AttributeWidget::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	Update();
}