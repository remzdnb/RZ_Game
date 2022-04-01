///	RemzDNB

// RZ_Game
#include "Character/RZ_Character.h"
#include "Pawn/RZ_PawnCombatComponent.h"
#include "Game/RZ_GameInstance.h"
#include "Game/RZ_GameState.h"
#include "Game/RZ_GameSettings.h"
#include "AI/RZ_PawnAIController.h"
#include "AbilitySystem/RZ_AttributeSet.h"
#include "AbilitySystem/RZ_AbilitySystemComponent.h"
#include "AbilitySystem/RZ_GameplayAbility.h"
// Character plugin
#include "RZ_CharacterMovementComponent.h"
#include "RZ_CharacterAnimInstance.h"
// InventorySystem plugin
#include "RZ_InventoryComponent.h"
/// Engine
#include "Components/SplineMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "GameplayTagContainer.h"

void ARZ_Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(ARZ_Character, TargetLocation, COND_SkipOwner);
}

ARZ_Character::ARZ_Character(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<URZ_CharacterMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
	GetCapsuleComponent()->InitCapsuleSize(28.0f, 85.0f);
	GetCapsuleComponent()->SetCollisionProfileName("PawnCapsule_Enabled");
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	GetMesh()->SetRelativeLocation(DEFAULTRELATIVEMESHLOCATION);
	GetMesh()->SetRelativeRotation(DEFAULTRELATIVEMESHROTATION);
	GetMesh()->SetCollisionProfileName("IgnoreAll");
	GetMesh()->SetGenerateOverlapEvents(false);
	GetMesh()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	GetMesh()->SetCustomDepthStencilValue(1);

	AbilitySystemComp = CreateDefaultSubobject<URZ_AbilitySystemComponent>(FName("AbilitySystemComp"));
	AbilitySystemComp->SetIsReplicated(true);
	AbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	Attributes = CreateDefaultSubobject<URZ_AttributeSet>("Attributes");
	
	PawnCombatCT = CreateDefaultSubobject<URZ_PawnCombatComponent>(FName("PawnCombatComp"));
	InventoryComp = CreateDefaultSubobject<URZ_InventoryComponent>(FName("InventoryComp"));
	
	AIControllerClass = ARZ_PawnAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::Disabled;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	PawnOwnership = ERZ_PawnOwnership::Player;
}

void ARZ_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld() == false)
		return;

	GameState = Cast<ARZ_GameState>(GetWorld()->GetGameState());
	GameSettings = Cast<URZ_GameInstance>(GetGameInstance())->GetGameSettings();

	CharacterMovementComp = Cast<URZ_CharacterMovementComponent>(GetMovementComponent());
	PawnCombatCT->OnHealthReachedZero.AddUniqueDynamic(this, &ARZ_Character::OnDeath);
	
	InventoryComp->OnItemAdded.AddUniqueDynamic(this, &ARZ_Character::OnItemAdded);
	//ItemManagerComp->OnItemSelect.AddUniqueDynamic(this, &ARZ_Character::OnItemEquipped);
}

void ARZ_Character::BeginPlay()
{
	Super::BeginPlay();

	// Spawn starting items.

	GameState->ReportPawnBeginPlay(this);
	
	for (const auto& ItemName : GameSettings->DefaultItems)
	{
		//InventoryComp->AddItem(ItemName);
	}
}

void ARZ_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// erf, onrep ?
	
	if (CharacterMovementComp)
	{
		if (CharacterMovementComp->GetIsSprinting())
		{
			bUseControllerRotationYaw = false;
			CharacterMovementComp->bOrientRotationToMovement = true;
		}
		else
		{
			bUseControllerRotationYaw = true;
			CharacterMovementComp->bOrientRotationToMovement = false;
		}
	}

	//
}

void ARZ_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Server GAS init.
	AbilitySystemComp->InitAbilityActorInfo(this, this);
	InitializeAttributes();
	AddDefaultAbilities();
}

void ARZ_Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Client GAS init.
	AbilitySystemComp->InitAbilityActorInfo(this, this);
	InitializeAttributes();

	if (AbilitySystemComp && InputComponent)
	{
		// bind to do
	}
}

void ARZ_Character::Init(ERZ_PawnOwnership NewPawnOwnership, uint8 NewTeamID)
{
	SetPawnOwnerShip(NewPawnOwnership);
	SetTeamID(NewTeamID);

	if (NewPawnOwnership != ERZ_PawnOwnership::Player)
	{
		SpawnDefaultController();
	}
	/*if (NewPawnOwnerShip == ERZ_PawnOwnership::Player)
	{
		GameplayTags.AddTag(FGameplayTag::RequestGameplayTag(FName("PawnOwnership.Player"))); 
	}
	else
	{
		GameplayTags.AddTag(FGameplayTag::RequestGameplayTag(FName("PawnOwnership.AI"))); 
	}*/
}

UBehaviorTree* ARZ_Character::GetBehaviorTree()
{
	return PawnBehaviorTree;
}

void ARZ_Character::SetActiveTarget(AActor* NewActiveTarget)
{
}

void ARZ_Character::SetWantToFire(bool bNewWantToFire)
{
	if (!InventoryComp) { return; }

	InventoryComp->SetWantToUseEquippedItem(bNewWantToFire);
}

void ARZ_Character::OnProjectileCollision(float ProjectileDamage, const FVector& HitLocation,
                                          AController* InstigatorController)
{
	if (PawnCombatCT)
	{
		PawnCombatCT->ApplyDamage(ProjectileDamage, HitLocation, InstigatorController, this);
	}
}

void ARZ_Character::InitializeAttributes()
{
	if (AbilitySystemComp && SetDefaultAttributesEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComp->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComp->MakeOutgoingSpec(
			SetDefaultAttributesEffect,
			1,
			EffectContext
		);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle EffectHandle = AbilitySystemComp->ApplyGameplayEffectSpecToTarget(
				*SpecHandle.Data.Get(),
				AbilitySystemComp
			);
		}
	}
}

void ARZ_Character::AddDefaultAbilities()
{
	check(AbilitySystemComp);
	
	if (HasAuthority())
	{
		for (TSubclassOf<URZ_GameplayAbility>& Ability : DefaultAbilites)
		{
			AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(
				Ability,
				1,
				static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID),
				this
			));
		}
	}
}

bool ARZ_Character::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	FGameplayTagContainer OwnedTags;
	GetOwnedGameplayTags(OwnedTags);

	return OwnedTags.HasTag(TagToCheck);
}

bool ARZ_Character::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	FGameplayTagContainer OwnedTags;
	GetOwnedGameplayTags(OwnedTags);

	return OwnedTags.HasAll(TagContainer);
}

bool ARZ_Character::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	FGameplayTagContainer OwnedTags;
	GetOwnedGameplayTags(OwnedTags);

	return OwnedTags.HasAny(TagContainer);
}

#pragma region +++ ItemManager ...

void ARZ_Character::OnItemAdded(AActor* AddedItem)
{
	IRZ_PawnInterface* ItemCombatInterface = Cast<IRZ_PawnInterface>(AddedItem);
	if (!ItemCombatInterface) { return; }

	ItemCombatInterface->SetPawnOwnerShip(ERZ_PawnOwnership::Player);

	//
	
	IRZ_ItemActorInterface* ItemInterface = Cast<IRZ_ItemActorInterface>(AddedItem);
	if (!ItemInterface) { return; }
	
	if (ItemInterface->GetItemSettings().Type == ERZ_ItemType::Weapon)
	{
		AddedItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_rSocket");
	}
}

void ARZ_Character::OnItemEquipped(AActor* EquippedItem)
{

}

void ARZ_Character::OnItemUsed(AActor* UsedItem)
{
	URZ_CharacterAnimInstance* CharacterAnimInstance = Cast<URZ_CharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (CharacterAnimInstance)
	{
		CharacterAnimInstance->StartUseAnimation();
	}
}

#pragma endregion

#pragma region +++ Combat ...
/*
void ARZ_Character::OnProjectileCollision(float ProjectileDamage, const FVector& HitLocation, AController* InstigatorController)
{
	if (GetLocalRole() < ROLE_Authority) { return; }
	if (PawnCombatComp == nullptr) { return; }

	PawnCombatComp->ApplyDamage(ProjectileDamage, HitLocation, InstigatorController, nullptr);
}
*/
void ARZ_Character::OnDeath()
{
	SetLifeSpan(10.0f);

	GameState->ReportPawnEndPlay(this);
	
	if (Cast<AAIController>(GetOwner()))
	{
		GetOwner()->Destroy();
		//Destroy();
	}
		/*if (MeleeWeapon)
			MeleeWeapon->OnHolster();
		if (RangedWeapon)
			RangedWeapon->OnHolster();

		if (PController)
		{
			PController->OnCharacterDeath();
		}

		if (AIController)
		{
			ABasePlayerController* KillerPController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			KillerPController->AddMoney(100.0f);
			AIController->Destroy();
		}
	}*/

	//ABaseGameMode* GMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	//GMode->ReportCharacterDeath(this);
	
	OnDeath_Multicast();
}

void ARZ_Character::OnDeath_Multicast_Implementation()
{
	//GetCharacterMovement()->MaxWalkSpeed = CharacterData.DefaultSpeed;

	if (GetMesh() && GetMesh()->GetPhysicsAsset())
	{
		GetCapsuleComponent()->SetCollisionProfileName("PawnCapsule_Disabled");
		GetMesh()->SetCollisionProfileName("PawnMesh_Physics");
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bBlendPhysics = true;
	}
}

void ARZ_Character::SetOnHitMaterial(bool bNewIsEnabled)
{
}

#pragma endregion

const FRZ_CharacterAnimData& ARZ_Character::GetCharacterAnimData()
{
	if (CharacterMovementComp)
	{
		if (CharacterMovementComp->GetIsSprinting())
		{
			CharacterAnimData.LowerBodyAnimStance = ERZ_LowerBodyAnimStance::Run;
		}
		else
		{
			CharacterAnimData.LowerBodyAnimStance = ERZ_LowerBodyAnimStance::Walk;
		}
	}
	
	/*if (ItemManagerComp)
	{
		if (ItemManagerComp->GetEquippedItem())
		{
			if (ItemManagerComp->GetEquippedItem()->GetItemData())
			{
				switch (ItemManagerComp->GetEquippedItem()->GetItemData()->AnimType)
				{

				case ERZ_ItemAnimType::Pistol:
					CharacterAnimData.UpperBodyAnimStance = ERZ_UpperBodyAnimStance::Pistol;
					break;
				case ERZ_ItemAnimType::Rifle:
					CharacterAnimData.UpperBodyAnimStance = ERZ_UpperBodyAnimStance::Rifle;
					break;
				case ERZ_ItemAnimType::Sword:
					CharacterAnimData.UpperBodyAnimStance = ERZ_UpperBodyAnimStance::Sword;
					break;
				default:
					CharacterAnimData.UpperBodyAnimStance = ERZ_UpperBodyAnimStance::Sword;
				}
			}
		}
	}*/
	
	return CharacterAnimData;
}

