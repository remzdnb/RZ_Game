/// RemzDNB
///
///	RZ_AttributeComponent.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_AttributeSystem.h"
#include "Components/ActorComponent.h"
#include "RZ_AttributeComponent.generated.h"

USTRUCT()
struct RZ_ATTRIBUTESYSTEM_API FRZ_DamageInfo
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	float Amount;

	UPROPERTY()
	float NewHealth;

	UPROPERTY()
	float MaxHealth;

	UPROPERTY()
	FVector Location;
	
	UPROPERTY()
	AController* InstigatorController;

	UPROPERTY()
	APawn* InstigatorPawn;

	FRZ_DamageInfo()
	{
		Amount = 0.0f;
		NewHealth = 0.0f;
		MaxHealth = 0.0f;
		Location = FVector::ZeroVector;
		InstigatorController = nullptr;
		InstigatorPawn = nullptr;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDamageTakenDelegate, const FRZ_DamageInfo&, DamageInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthUpdatedDelegate, float, NewHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthReachedZeroDelegate);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RZ_ATTRIBUTESYSTEM_API URZ_AttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	URZ_AttributeComponent();

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
	
	UPROPERTY(Transient, Replicated) float MaxHealth;
	UPROPERTY(Transient, Replicated) float Health;
	UPROPERTY(Transient, Replicated) float MaxArmor;
	UPROPERTY(Transient, Replicated) float Armor;
	UPROPERTY(Transient, Replicated) bool bIsDead;
	
	FORCEINLINE void SetTeamID(uint8 NewTeamID) { TeamID = NewTeamID; }
	FORCEINLINE uint8 GetTeamID() const { return TeamID; }

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRZ_AttributeComponentSettings CombatComponentSettings;

	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	uint8 TeamID;

};