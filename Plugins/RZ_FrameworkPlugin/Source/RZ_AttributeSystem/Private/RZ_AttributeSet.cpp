/// RemzDNB

#include "RZ_AttributeSet.h"
//
#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

URZ_AttributeSet::URZ_AttributeSet()
{
	bDebug = true;
}

void URZ_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (bDebug)
	{
		UE_LOG(LogTemp, Display,
		       TEXT("URZ_AttributeSet::PreAttributeChange : FGameplayAttribute == %s // NewValue == %s"),
		       *Attribute.AttributeName,
		       *FString::FromInt(NewValue)
		);
	}
}

void URZ_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (bDebug)
	{
		UE_LOG(LogTemp, Display, TEXT("URZ_AttributeSet::PostGameplayEffectExecute"));
		const FString AttributeString = "Attribute == " + Data.EvaluatedData.Attribute.AttributeName;
		UE_LOG(LogTemp, Display, TEXT("%s"), *AttributeString);
		const FString MagnitudeString = "Magnitude == " + FString::FromInt(Data.EvaluatedData.Magnitude);
		UE_LOG(LogTemp, Display, TEXT("%s"), *MagnitudeString);
		const FString ModifierString = "Modifier == " + UEnum::GetDisplayValueAsText(Data.EvaluatedData.ModifierOp.GetValue()).ToString();
		UE_LOG(LogTemp, Display, TEXT("%s"), *ModifierString);
		UE_LOG(LogTemp, Display, TEXT("-------------------------------------------"));
	}
	
	if (Data.EvaluatedData.Attribute == URZ_AttributeSet::GetHealthAttribute())
	{
		//const float NewHealth = GetHealth() - Data.EvaluatedData.Magnitude;
		//SetHealth(Data.EvaluatedData.Magnitude);
		//SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
	}
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

void URZ_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URZ_AttributeSet, MaxHealth, OldValue);
}

void URZ_AttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URZ_AttributeSet, Shield, OldValue);
}
