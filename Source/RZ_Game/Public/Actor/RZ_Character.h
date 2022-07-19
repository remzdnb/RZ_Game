/// RemzDNB
///
///	Base class for any networked pawn using a navmesh.
///	Implements custom MovementComponent, ItemManager plugin and AI.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
#include "RZ_AnimationSystem.h"
//
#include "AbilitySystem/RZ_AbilitySystemComponent.h"
//
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagAssetInterface.h"
#include "RZ_InventorySystem.h"
#include "RZ_CharacterAnimInstance.h"
#include "RZ_AttributeComponent.h"
#include "RZ_Character.generated.h"

#define DEFAULTRELATIVEMESHLOCATION FVector(0.0f, 0.0f, -90.0f)
#define DEFAULTRELATIVEMESHROTATION FRotator(0.0f, -90.0f, 0.0f)

class URZ_GameplayAbility;
class URZ_AbilitySystemComponent;
class URZ_AttributeSet;
//
class URZ_CharacterMovementComponent;
class URZ_AttributeComponent;
class URZ_InventoryComponent;

UCLASS()
class RZ_GAME_API ARZ_Character : public ACharacter,
                                  public IRZ_ActorInterface,
                                  public IRZ_PawnInterface,
                                  public IRZ_InventoryOwnerInterface,
                                  public IRZ_CharacterAnimInterface,
                                  public IAbilitySystemInterface,
                                  public IGameplayTagAssetInterface
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
	
	FORCEINLINE URZ_AttributeComponent* GetPawnCombatComponent() const { return AttributeComp; }
	FORCEINLINE URZ_CharacterAnimInstance* GetCharacterAnimInstance() const { return CharacterAnimInstance; }
	FORCEINLINE class UBehaviorTree* GetPawnBehaviorTree() const { return PawnBehaviorTree; };
	
	/// ActorInterface implementation.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	public:

	FORCEINLINE virtual const FVector& GetPlayerTargetLocation() const override { return PlayerTargetLocation; }
	FORCEINLINE virtual void SetPlayerTargetLocation(const FVector& NewPlayerTargetLocation) override
	{
		PlayerTargetLocation = NewPlayerTargetLocation;
	};

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector PlayerTargetLocation;
	
	// Pawn interface
	
	virtual class UBehaviorTree* GetBehaviorTree() override;
	virtual void SetActiveTarget(AActor* NewActiveTarget) override;
	virtual void SetWantToFire(bool bNewWantToFire) override;
	
	// IGameplayTagAssetInterface
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = GameplayTags; return; }
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

	// IAbilitySystemInterface

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemCT; }

	//
	
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
	URZ_AttributeComponent* AttributeComp;



	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName ActorName = "DefaultCharacterActorName";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* PawnBehaviorTree;
	
	//
	
	UPROPERTY()
	URZ_AttributeSet* Attributes;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_InventoryComponent* InventoryComp;

	//
	
	UFUNCTION()
	virtual void OnInventoryItemEquipped(const FRZ_InventorySlotData& SlotData);

	UFUNCTION()
	virtual void OnItemUsed(AActor* UsedItem);

	UFUNCTION()
	virtual void OnWeaponFired(class ARZ_Weapon* Weapon);

public:

	// InventoryOwnerInterface.

	virtual URZ_InventoryComponent* GetInventoryComponent() const override { return InventoryComp; }
	virtual USceneComponent* GetAttachComponent() const override { return GetMesh(); }

	/// Combat
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:


private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_AttributeComponent* CombatComp;

	UFUNCTION()
	void OnDeath();

	UFUNCTION(NetMulticast, Reliable)
	void OnDeath_Multicast();

	UFUNCTION()
	void SetOnHitMaterial(bool bNewIsEnabled);

	//
	
	UPROPERTY()
	FTimerHandle OnHitTimerHandle;
};

