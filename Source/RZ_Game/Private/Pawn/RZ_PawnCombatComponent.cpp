/// RemzDNB

// RZ_Game
#include "Pawn/RZ_PawnCombatComponent.h"
#include "Core/RZ_GameState.h"
// Engine
#include "Net/UnrealNetwork.h"

URZ_PawnCombatComponent::URZ_PawnCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 1000.0f;
	Health = 1000.0f;
	MaxArmor = 1000.0f;
	Armor = 1000.0f;
}

void URZ_PawnCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	GameState = GetWorld()->GetGameState<ARZ_GameState>();
}

void URZ_PawnCombatComponent::Init(float NewMaxHealth, float NewMaxArmor)
{
	MaxHealth = NewMaxHealth;
	Health = NewMaxHealth;
	MaxArmor = NewMaxArmor;
	Armor = NewMaxArmor;
}

void URZ_PawnCombatComponent::ApplyDamage(float BaseAmount, const FVector& Location, AController* InstigatorController, AActor* DamageCauser)
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

	GameState->ReportPawnDamage(DamageInfo);
	
	//OnDamageTaken.Broadcast(DamageInfo);
	//OnHealthUpdated.Broadcast(Health, MaxHealth);

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT(" URZ_PawnCombatComponent::ApplyDamage"));;
}


void URZ_PawnCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URZ_PawnCombatComponent, MaxHealth);
	DOREPLIFETIME(URZ_PawnCombatComponent, Health);
	DOREPLIFETIME(URZ_PawnCombatComponent, MaxArmor);
	DOREPLIFETIME(URZ_PawnCombatComponent, Armor);
	DOREPLIFETIME(URZ_PawnCombatComponent, bIsDead);
}
