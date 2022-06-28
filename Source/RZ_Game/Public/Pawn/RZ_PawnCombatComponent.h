/// RemzDNB
///
///	RZ_PawnCombatComponent.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Components/ActorComponent.h"
#include "RZ_Game.h"
#include "RZ_PawnCombatComponent.generated.h"

class ARZ_GameState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDamageTakenDelegate, const FRZ_DamageInfo&, DamageInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthUpdatedDelegate, float, NewHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthReachedZeroDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RZ_GAME_API URZ_PawnCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	URZ_PawnCombatComponent();

	virtual void BeginPlay() override;

	//

	UFUNCTION()
	void Init(float NewMaxHealth, float NewMaxArmor);

	UFUNCTION()
	void ApplyDamage(float BaseAmount, const FVector& Location, AController* InstigatorController, AActor* DamageCauser);

	//

	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetHealth() const { return Health; }

	//

	FDamageTakenDelegate OnDamageTaken;
	FHealthUpdatedDelegate OnHealthUpdated;
	FHealthReachedZeroDelegate OnHealthReachedZero;

private:
	
	ARZ_GameState* GameState;

	//
	
	UPROPERTY(Transient, Replicated) float MaxHealth;
	UPROPERTY(Transient, Replicated) float Health;
	UPROPERTY(Transient, Replicated) float MaxArmor;
	UPROPERTY(Transient, Replicated) float Armor;
	UPROPERTY(Transient, Replicated) bool bIsDead;

};
