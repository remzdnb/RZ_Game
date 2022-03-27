/// RemzDNB
///
///	Base class for any networked pawn using a navmesh.
///	Implements custom MovementComponent, ItemManager plugin and AI.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_GameTypes.h"
#include "RZ_GameInterfaces.h"
#include "RZ_CharacterAnimInterfaces.h"
//
#include "CoreMinimal.h"
#include "RZM_ItemActor.h"
#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h"
#include "Pawn/RZ_PawnCombatComponent.h"
#include "RZ_Character.generated.h"

#define DEFAULTRELATIVEMESHLOCATION FVector(0.0f, 0.0f, -90.0f)
#define DEFAULTRELATIVEMESHROTATION FRotator(0.0f, -90.0f, 0.0f)

class URZ_GameSettings;
//
class ARZ_Item;
class URZ_ItemManagerComponent;
class URZ_PawnCombatComponent;

UCLASS()
class RZ_GAME_API ARZ_Character : public ACharacter,
                                  public IGameplayTagAssetInterface,
                                  public IRZ_InteractionInterface,
                                  public IRZ_CharacterAnimInterface,
                                  public IRZ_ProjectileInterface
{
	GENERATED_BODY()

public:

	ARZ_Character(const FObjectInitializer& ObjectInitializer);
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Init(ERZ_PawnOwnership Ownership);

	// IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = GameplayTags; return; }
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

private:

	class URZ_CharacterMovementComponent* CharacterMovement;

	UPROPERTY()
	ERZ_PawnOwnership PawnOwnership;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer GameplayTags;
	
	//

	TWeakObjectPtr<URZ_GameSettings> GameSettings;
	
	/// Controls
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

public:
	
	UPROPERTY(Replicated)
	FVector TargetLocation;

	/// Interaction
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	
	virtual void StartHover() override;
	virtual void StopHover() override;
	virtual void Use(class ARZ_PlayerController* InstigatorController) override;
	
	void OnUsed(class ARZ_BattlePlayerController* PlayerController);
	
	/// ItemManager
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	
	URZ_ItemManagerComponent* GetItemManager() const { return ItemManager; }

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class URZ_ItemManagerComponent* ItemManager;

private:

	UFUNCTION()
	void OnItemSpawned(ARZ_Item* SpawnedItem);
	
	UFUNCTION()
	void OnItemEquipped(const FName& ItemName);

	/// Targeting
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:

	UPROPERTY()
	class USplineMeshComponent* TargetSplineMesh;

	void SetupTargetSplineMesh();
	void UpdateTargetSplineMesh();

	/// Combat
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	// Projectile Interface

	virtual void OnProjectileCollision(float ProjectileDamage, const FVector& HitLocation, AController* InstigatorController) override;

	//

	FORCEINLINE URZ_PawnCombatComponent* GetPawnCombatComponent() const { return PawnCombatComp; }

private:

	UFUNCTION()
	void OnDeath();

	UFUNCTION(NetMulticast, Reliable)
	void OnDeath_Multicast();

	UFUNCTION()
	void SetOnHitMaterial(bool bNewIsEnabled);

	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_PawnCombatComponent* PawnCombatComp;

	UPROPERTY()
	FTimerHandle OnHitTimerHandle;

	/// AI
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	FORCEINLINE class UAIPerceptionComponent* GetAIPerceptionComponent() const { return AIPerceptionComp; }
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ARZ_Character")
	class UBehaviorTree* BehaviorTree;
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* AIPerceptionComp;
	
	/// Animation
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	virtual const FRZ_CharacterAnimData& GetCharacterAnimData() override;
	
private:

	FRZ_CharacterAnimData CharacterAnimData;
};

