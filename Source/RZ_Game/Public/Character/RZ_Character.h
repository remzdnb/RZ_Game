/// RemzDNB
///
///	Base class for any networked pawn using a navmesh.
///	Implements custom MovementComponent, ItemManager plugin and AI.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_GameInterfaces.h"
#include "RZ_CharacterAnimInterfaces.h"
//
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RZ_Character.generated.h"

#define DEFAULTRELATIVEMESHLOCATION FVector(0.0f, 0.0f, -90.0f)
#define DEFAULTRELATIVEMESHROTATION FRotator(0.0f, -90.0f, 0.0f)

class URZ_GameSettings;

class ARZ_Item;
class URZ_ItemManagerComponent;

UCLASS()
class RZ_GAME_API ARZ_Character : public ACharacter,
                                  public IRZ_InteractionInterface,
                                  public IRZ_CharacterAnimInterface
{
	GENERATED_BODY()

public:

	ARZ_Character(const FObjectInitializer& ObjectInitializer);
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	class URZ_CharacterMovementComponent* CharacterMovement;

	//

	TWeakObjectPtr<URZ_GameSettings> GameSettings;
	
	/// Controls
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

public:
	
	UPROPERTY(Replicated)
	FVector TargetLocation;

	UPROPERTY(Replicated)
	bool bRotateToTarget;

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

	/// AI
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UPROPERTY()
	class UBehaviorTree* BehaviorTree;
	
private:

	
	
	/// Animation
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	virtual const FRZ_CharacterAnimData& GetCharacterAnimData() override;
	
private:

	FRZ_CharacterAnimData CharacterAnimData;
};

