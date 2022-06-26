/// RemzDNB
///
///	Base class for any networked pawn using a navmesh.
///	Implements custom MovementComponent, ItemManager plugin and AI.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
#include "RZM_AnimationSystem.h"
//
#include "AbilitySystem/RZ_AbilitySystemComponent.h"
//
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagAssetInterface.h"
#include "RZM_Shared.h"
#include "RZM_WeaponSystem.h"
#include "RZ_CharacterAnimInstance.h"
#include "RZ_Character.generated.h"

#define DEFAULTRELATIVEMESHLOCATION FVector(0.0f, 0.0f, -90.0f)
#define DEFAULTRELATIVEMESHROTATION FRotator(0.0f, -90.0f, 0.0f)

class URZ_GameplayAbility;
class URZ_AbilitySystemComponent;
class URZ_AttributeSet;
//
class URZ_CharacterMovementComponent;
class URZ_PawnCombatComponent;
class URZ_InventoryComponent;

UCLASS()
class RZ_GAME_API ARZ_Character : public ACharacter,
									public IRZ_ActorInterface,
                                  public IRZ_PawnInterface,
                                  public IRZ_WeaponDamageInterface,
                                  public IAbilitySystemInterface,
                                  public IGameplayTagAssetInterface,
                                  public IRZ_CharacterAnimInterface
{
	GENERATED_BODY()

public:

	ARZ_Character(const FObjectInitializer& ObjectInitializer);
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override; // Called on server startup
	virtual void OnRep_PlayerState() override; // Called on client startup
	//virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

//

	virtual const FName& GetTableRowName() override { return LolName; }
	virtual void SetPlayerTargetLocation(const FVector& NewPlayerTargetLocation) override;
	
	// Pawn interface
	
	virtual void InitCombatInterface(ERZ_PawnOwnership NewPawnOwnership, uint8 NewTeamID) override;
	virtual class UBehaviorTree* GetBehaviorTree() override;
	virtual void SetActiveTarget(AActor* NewActiveTarget) override;
	virtual void SetWantToFire(bool bNewWantToFire) override;

	// Projectile Interface

	virtual void OnProjectileCollision(float ProjectileDamage, const FVector& HitLocation,
	                                   AController* InstigatorController) override;
	
	// IGameplayTagAssetInterface
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = GameplayTags; return; }
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

	// IAbilitySystemInterface

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemCT; }

	//

	FORCEINLINE URZ_PawnCombatComponent* GetPawnCombatComponent() const { return PawnCombatCT; }
	FORCEINLINE URZ_InventoryComponent* GetInventoryComponent() const { return InventoryCT; }
	FORCEINLINE URZ_CharacterAnimInstance* GetCharacterAnimInstance() const { return CharacterAnimInstance; }
	FORCEINLINE class UBehaviorTree* GetPawnBehaviorTree() const { return PawnBehaviorTree; };

	//

	UPROPERTY(Replicated)
	FVector PlayerTargetLocation;
	
protected:

	ARZ_GameState* GameState;
	TWeakObjectPtr<URZ_GameSettings> GameSettings;
	URZ_CharacterAnimInstance* CharacterAnimInstance;

	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_AbilitySystemComponent* AbilitySystemCT;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_CharacterMovementComponent* CharacterMovementCT;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_PawnCombatComponent* PawnCombatCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_InventoryComponent* InventoryCT;

	//
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ARZ_Character BP Settings", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* PawnBehaviorTree;
	

	UPROPERTY()
	class URZ_AttributeSet* Attributes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer GameplayTags;
	
	UPROPERTY()
	ERZ_PawnOwnership PawnOwnership;

	/// GAS
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> SetDefaultAttributesEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<URZ_GameplayAbility>> DefaultAbilites;

private:

	UFUNCTION()
	void InitializeAttributes();

	UFUNCTION()
	void AddDefaultAbilities();
	
	/// InventorySystem
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
protected:
	
	UFUNCTION()
	virtual void OnInventoryItemAdded(AActor* AddedItem);
	
	UFUNCTION()
	virtual void OnInventoryItemSelected(AActor* SelectedItem);

	UFUNCTION()
	virtual void OnItemUsed(AActor* UsedItem);

	UFUNCTION()
	virtual void OnWeaponFired(ARZ_Weapon* Weapon);

	/// Combat
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:


private:

	UFUNCTION()
	void OnDeath();

	UFUNCTION(NetMulticast, Reliable)
	void OnDeath_Multicast();

	UFUNCTION()
	void SetOnHitMaterial(bool bNewIsEnabled);

	//
	
	UPROPERTY()
	FTimerHandle OnHitTimerHandle;

	FName LolName = "MDR";
};

