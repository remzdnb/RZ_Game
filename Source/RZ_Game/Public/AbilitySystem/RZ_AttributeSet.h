// RemzDNB

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RZ_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class RZ_GAME_API URZ_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	URZ_AttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(URZ_AttributeSet, Health);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(URZ_AttributeSet, Armor);

	UFUNCTION()
	virtual void OnRep_Armor(const FGameplayAttributeData& OldValue);
};
