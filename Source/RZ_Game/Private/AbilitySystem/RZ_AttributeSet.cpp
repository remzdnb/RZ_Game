/// RemzDNB

#include "AbilitySystem/RZ_AttributeSet.h"
//
#include "Net/UnrealNetwork.h"

URZ_AttributeSet::URZ_AttributeSet()
{
}

void URZ_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(URZ_AttributeSet, Health, COND_None, REPNOTIFY_Always);
}

void URZ_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URZ_AttributeSet, Health, OldValue);
}

void URZ_AttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URZ_AttributeSet, Armor, OldValue);
}
