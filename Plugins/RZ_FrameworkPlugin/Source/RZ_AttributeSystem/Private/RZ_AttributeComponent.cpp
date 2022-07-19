/// RemzDNB

#include "RZ_AttributeComponent.h"
// Engine
#include "Net/UnrealNetwork.h"

URZ_AttributeComponent::URZ_AttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 1000.0f;
	Health = 1000.0f;
	MaxArmor = 1000.0f;
	Armor = 1000.0f;
}

void URZ_AttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URZ_AttributeComponent::Init(float NewMaxHealth, float NewMaxArmor)
{
	MaxHealth = NewMaxHealth;
	Health = NewMaxHealth;
	MaxArmor = NewMaxArmor;
	Armor = NewMaxArmor;
}

void URZ_AttributeComponent::ApplyDamage(float BaseAmount, const FVector& Location, AController* InstigatorController, AActor* DamageCauser)
{
	//if (bIsDead)
		//return;
	
	if (Health - BaseAmount <= 0)
	{
		Health = 0.0f;
		OnHealthReachedZero.Broadcast();
	}
	else
	{
		Health -= BaseAmount;
	}

	FRZ_DamageInfo DamageInfo;
	DamageInfo.Amount = BaseAmount;
	DamageInfo.NewHealth = Health;
	DamageInfo.MaxHealth = MaxHealth;
	DamageInfo.Location = Location;
	DamageInfo.InstigatorController = InstigatorController;

	//GameState->ReportPawnDamage(DamageInfo);
	
	//OnDamageTaken.Broadcast(DamageInfo);
	//OnHealthUpdated.Broadcast(Health, MaxHealth);

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT(" URZ_AttributeComponent::ApplyDamage"));;
}


void URZ_AttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URZ_AttributeComponent, MaxHealth);
	DOREPLIFETIME(URZ_AttributeComponent, Health);
	DOREPLIFETIME(URZ_AttributeComponent, MaxArmor);
	DOREPLIFETIME(URZ_AttributeComponent, Armor);
	DOREPLIFETIME(URZ_AttributeComponent, bIsDead);
}
