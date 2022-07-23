/// RemzDNB
///
///	Base class for any networked pawn using a navmesh.
///	Implements custom MovementComponent, ItemManager plugin and AI.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Common.h"
#include "RZ_BuildingSystem.h"
#include "RZ_InventorySystem.h"
#include "RZ_PerceptionSystem.h"
#include "RZ_AttributeSystem.h"
#include "RZ_AnimationSystem.h"
#include "RZ_Game.h"
//
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "RZ_Character.generated.h"

#define DEFAULTRELATIVEMESHLOCATION FVector(0.0f, 0.0f, -90.0f)
#define DEFAULTRELATIVEMESHROTATION FRotator(0.0f, -90.0f, 0.0f)

class URZ_CharacterMovementComponent;
//
class UWidgetComponent;

UCLASS()
class RZ_GAME_API ARZ_Character : public ACharacter,
                                  public IRZ_WorldInteractionInterface,
                                  public IRZ_AIOwnerInterface,
                                  public IRZ_InventoryOwnerInterface,
                                  public IRZ_InventoryItemInterface,
                                  public IRZ_BuildableActorInterface,
                                  public IRZ_CharacterAnimInterface,
                                  public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ARZ_Character(const FObjectInitializer& ObjectInitializer);
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override; // Called on server startup
	virtual void OnRep_PlayerState() override; // Called on client startup

	// WorldInteractionInterface implementation. //. controller interface ?

	virtual void OnWorldHover(bool bNewIsHovered) override;
	virtual void OnWorldSelection(bool bNewSelected) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_CharacterMovementComponent* CharacterMovementCT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* OTMWidgetComp;

	UPROPERTY()
	URZ_OTM_Character_Widget* OTMWidget;
	
	//

	ARZ_GameState* GameState;
	TWeakObjectPtr<URZ_GameSettings> GameSettings;
	URZ_CharacterAnimInstance* CharacterAnimInstance;

	/// InventorySystem implementation.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	// InventoryOwnerInterface

	virtual URZ_InventoryComponent* GetInventoryComponent() const override { return InventoryComp; }
	virtual USceneComponent* GetAttachComponent() const override { return GetMesh(); }
	
	// ItemInterface

	FORCEINLINE virtual const FName& GetItemName() const override { return ItemName; }
	FORCEINLINE virtual const FRZ_ItemSettings& GetItemSettings() const override { return ItemSettings; }
	virtual void OnAttachedToInventory() override;
	virtual void OnEquippedByInventory(bool bNewIsSelected) override;
	virtual void OnWantToBeUsedByInventory(bool bNewWantsTouse) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_InventoryComponent* InventoryComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRZ_ItemSettings ItemSettings;

	//

	UFUNCTION()
	virtual void OnInventoryItemEquipped(const FRZ_InventorySlotData& SlotData);

	UFUNCTION()
	virtual void OnItemUsed(AActor* UsedItem);

	UFUNCTION()
	virtual void OnWeaponFired(class ARZ_Weapon* Weapon);

	/// BuildingSystem implementation.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	FORCEINLINE virtual URZ_BuildingComponent* GetBuildingComponent() const override { return BuildingComp; }
	FORCEINLINE virtual float GetBuildCenterZOffsetLocation() const override;
	
	virtual void OnDemoBuildStart() override;
	virtual void OnDemoBuildStop() override;
	virtual void OnBuildStart() override;
	virtual void OnBuildStop() override;
	virtual void OnBuildEnd() override;
	
protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_BuildingComponent* BuildingComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//UStaticMeshComponent* BuildMeshComp;
	
	UPROPERTY()
	UMaterialInterface* BaseMeshDefaultMaterial;

	/// PerceptionSystem implementation.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	URZ_PerceptionComponent* PerceptionComp;
	
	/// AI
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	
	virtual UBehaviorTree* GetBehaviorTree() override { return PawnBehaviorTree; }
	virtual AActor* GetTargetActor() const override { return TargetActor; }
	virtual void SetTargetActor(AActor* NewTargetActor) override { TargetActor = NewTargetActor; }
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* PawnBehaviorTree;

	UPROPERTY()
	AActor* TargetActor;
	
	/// Combat
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
private:

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//URZ_AttributeComponent* CombatComp;

	UFUNCTION()
	void OnDeath();

	UFUNCTION(NetMulticast, Reliable)
	void OnDeath_Multicast();

	UFUNCTION()
	void SetOnHitMaterial(bool bNewIsEnabled);

	//
	
	UPROPERTY()
	FTimerHandle OnHitTimerHandle;
	
	/// GAS
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	// IAbilitySystemInterface

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASComp; }

	//

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//TSubclassOf<UGameplayEffect> SetDefaultAttributesEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<URZ_GameplayAbility>> DefaultAbilities;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UGameplayEffect> SetDefaultAttributesGEClass;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAbilitySystemComponent* ASComp;
	
	UPROPERTY()
	URZ_AttributeSet* Attributes;

	//
	
	UFUNCTION()
	void InitializeAttributes();

	UFUNCTION()
	void AddDefaultAbilities();
};

