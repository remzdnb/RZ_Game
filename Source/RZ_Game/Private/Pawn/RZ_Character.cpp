///	RemzDNB

// RZ_Game
#include "Pawn/RZ_Character.h"
#include "Pawn/RZ_CharacterMovementComponent.h"
#include "RZ_AttributeComponent.h"
#include "Core/RZ_GameInstance.h"
#include "Core/RZ_GameState.h"
#include "Core/RZ_GameSettings.h"
#include "AI/RZ_PawnAIController.h"
#include "AbilitySystem/RZ_AttributeSet.h"
#include "AbilitySystem/RZ_AbilitySystemComponent.h"
#include "AbilitySystem/RZ_GameplayAbility.h"
// WeaponSystem plugin
#include "RZ_Weapon.h"
// InventorySystem plugin
#include "RZ_InventoryComponent.h"
// AnimationSystem plugin
#include "RZ_CharacterAnimInstance.h"
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
	
	DOREPLIFETIME_CONDITION(ARZ_Character, PlayerTargetLocation, COND_SkipOwner);
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

	AbilitySystemCT = CreateDefaultSubobject<URZ_AbilitySystemComponent>(FName("AbilitySystemComp"));
	AbilitySystemCT->SetIsReplicated(true);
	AbilitySystemCT->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	Attributes = CreateDefaultSubobject<URZ_AttributeSet>("Attributes");
	
	AttributeComp = CreateDefaultSubobject<URZ_AttributeComponent>(FName("AttributeComp"));
	CombatComp = CreateDefaultSubobject<URZ_CombatComponent>(FName("CombatComp"));
	InventoryCT = CreateDefaultSubobject<URZ_InventoryComponent>(FName("InventoryComp"));
	
	AIControllerClass = ARZ_PawnAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::Disabled;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
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
	CharacterMovementCT = Cast<URZ_CharacterMovementComponent>(GetMovementComponent());
	CharacterAnimInstance = Cast<URZ_CharacterAnimInstance>(GetMesh()->GetAnimInstance());
	
	AttributeComp->OnHealthReachedZero.AddUniqueDynamic(this, &ARZ_Character::OnDeath);
	InventoryCT->OnItemAdded.AddUniqueDynamic(this, &ARZ_Character::OnInventoryItemAdded);
}

void ARZ_Character::BeginPlay()
{
	Super::BeginPlay();

	// Spawn starting items.

	GameState->ReportPawnBeginPlay(this);

	SetActorMode(ERZ_ActorMode::Visible_Enabled);
	
	for (const auto& ItemName : GameSettings->DefaultItems)
	{
		//InventoryComp->AddItem(ItemName);
	}
}

void ARZ_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// erf, onrep ?
	
	if (CharacterMovementCT)
	{

		
		if (CharacterMovementCT->GetIsSprinting())
		{
			bUseControllerRotationYaw = false;
			CharacterMovementCT->bOrientRotationToMovement = true;
		}
		else
		{
			bUseControllerRotationYaw = true;
			CharacterMovementCT->bOrientRotationToMovement = false;
		}
	}

	//

	FRZ_CharacterAnimData TempCharacterAnimData = GetCharacterAnimData();

	if (CharacterMovementCT)
	{
		if (CharacterMovementCT->GetIsSprinting())
			TempCharacterAnimData.LowerBodyAnimStance = ERZ_LowerBodyAnimStance::Run;
		else
			TempCharacterAnimData.LowerBodyAnimStance = ERZ_LowerBodyAnimStance::Walk;
	}

	TempCharacterAnimData.ViewRotation = GetControlRotation();

	UKismetMathLibrary::FindLookAtRotation(
		FVector(GetActorLocation().X, GetActorLocation().Y, BASEVIEWHEIGHT),
		PlayerTargetLocation)
	;


	SetAnimData(TempCharacterAnimData);

	//
}

void ARZ_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Server GAS init.
	AbilitySystemCT->InitAbilityActorInfo(this, this);
	InitializeAttributes();
	AddDefaultAbilities();
}

void ARZ_Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Client GAS init.
	AbilitySystemCT->InitAbilityActorInfo(this, this);
	InitializeAttributes();

	if (AbilitySystemCT && InputComponent)
	{
		// bind to do
	}
}

void ARZ_Character::SetPlayerTargetLocation(const FVector& NewPlayerTargetLocation)
{
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
	if (!InventoryCT) { return; }

	InventoryCT->SetWantToUseEquippedItem(bNewWantToFire);
}

void ARZ_Character::InitializeAttributes()
{
	if (AbilitySystemCT && SetDefaultAttributesEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemCT->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemCT->MakeOutgoingSpec(
			SetDefaultAttributesEffect,
			1,
			EffectContext
		);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle EffectHandle = AbilitySystemCT->ApplyGameplayEffectSpecToTarget(
				*SpecHandle.Data.Get(),
				AbilitySystemCT
			);
		}
	}
}

void ARZ_Character::AddDefaultAbilities()
{
	check(AbilitySystemCT);
	
	if (HasAuthority())
	{
		for (TSubclassOf<URZ_GameplayAbility>& Ability : DefaultAbilites)
		{
			AbilitySystemCT->GiveAbility(FGameplayAbilitySpec(
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

void ARZ_Character::OnInventoryItemAdded(AActor* AddedItem)
{
	IRZ_ActorInterface* ItemInterface = Cast<IRZ_ActorInterface>(AddedItem);
	if (ItemInterface)
	{
		if (ItemInterface->GetActorSettings().Type == ERZ_ActorType::Weapon)
		{
			AddedItem->AttachToComponent(
				GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				"hand_rSocket"
			);
		}
	}
}

void ARZ_Character::OnInventoryItemSelected(AActor* SelectedItem)
{
	IRZ_ActorInterface* ItemInterface = Cast<IRZ_ActorInterface>(SelectedItem);
	if (!ItemInterface) { return; }

	FRZ_CharacterAnimData TempCharacterAnimData = GetCharacterAnimData();
	
	switch (ItemInterface->GetActorSettings().AnimType)
	{
	case ERZ_ItemAnimType::Pistol:
		TempCharacterAnimData.UpperBodyAnimStance = ERZ_UpperBodyAnimStance::Pistol;
		break;
	case ERZ_ItemAnimType::Rifle:
		TempCharacterAnimData.UpperBodyAnimStance = ERZ_UpperBodyAnimStance::Rifle;
		break;
	case ERZ_ItemAnimType::Sword:
		TempCharacterAnimData.UpperBodyAnimStance = ERZ_UpperBodyAnimStance::Sword;
		break;
	default:
		TempCharacterAnimData.UpperBodyAnimStance = ERZ_UpperBodyAnimStance::Default;
	}

	if (ItemInterface->GetActorSettings().Type == ERZ_ActorType::Weapon)
	{
		ARZ_Weapon* Weapon = Cast<ARZ_Weapon>(SelectedItem);
		if (Weapon)
		{
			Weapon->OnWeaponFired.AddUniqueDynamic(this, &ARZ_Character::OnWeaponFired);
		}
	}

	SetAnimData(TempCharacterAnimData);
}

void ARZ_Character::OnItemUsed(AActor* UsedItem)
{
	if (CharacterAnimInstance)
	{
		CharacterAnimInstance->StartUseAnimation();
	}
}

void ARZ_Character::OnWeaponFired(ARZ_Weapon* Weapon)
{
	if (!CharacterAnimInstance) { return; }

	CharacterAnimInstance->StartUseAnimation();
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