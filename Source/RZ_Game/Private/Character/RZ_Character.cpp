///	RemzDNB

// RZ_Game
#include "Character/RZ_Character.h"
#include "Pawn/RZ_PawnCombatComponent.h"
#include "Game/RZ_GameInstance.h"
#include "Game/RZ_GameState.h"
#include "Game/RZ_GameSettings.h"
#include "AI/RZ_CharacterAIController.h"
// CharacterActor Module
#include "RZ_CharacterMovementComponent.h"
// ItemActor Module
#include "RZ_Item.h"
// ItemManager Module
#include "RZ_ItemManagerComponent.h"
/// Engine
#include "Components/SplineMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/RZ_PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "GameplayTagContainer.h"

ARZ_Character::ARZ_Character(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<URZ_CharacterMovementComponent>(
		ACharacter::CharacterMovementComponentName)),
	PawnOwnership(ERZ_PawnOwnership::Player)
{
	GetCapsuleComponent()->InitCapsuleSize(28.0f, 85.0f);
	GetCapsuleComponent()->SetCollisionProfileName("Pawn");

	GetMesh()->SetRelativeLocation(DEFAULTRELATIVEMESHLOCATION);
	GetMesh()->SetRelativeRotation(DEFAULTRELATIVEMESHROTATION);
	GetMesh()->SetCollisionProfileName("CharacterMesh");
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCustomDepthStencilValue(1);
	
	PawnCombatComp = CreateDefaultSubobject<URZ_PawnCombatComponent>(FName("PawnCombatComp"));
	ItemManager = CreateDefaultSubobject<URZ_ItemManagerComponent>(FName("ItemManager"));
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(FName("AIPerceptionComp"));
	
	bUseControllerRotationPitch = false; // not here ?
	bUseControllerRotationYaw = true; // not here ?
	bUseControllerRotationRoll = false; // not here ?
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void ARZ_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld() == false)
		return;

	CharacterMovement = Cast<URZ_CharacterMovementComponent>(GetMovementComponent());
	
	PawnCombatComp->OnHealthReachedZero.AddUniqueDynamic(this, &ARZ_Character::OnDeath);
	ItemManager->OnItemSpawned.AddUniqueDynamic(this, &ARZ_Character::OnItemSpawned);

	GameSettings = Cast<URZ_GameInstance>(GetGameInstance())->GetGameSettings();
	//BehaviorTree = GameSettings->CharacterBehaviorTree;
}

void ARZ_Character::BeginPlay()
{
	Super::BeginPlay();
	
	SetupTargetSplineMesh();

	if (Cast<APlayerController>(GetOwner()))
	{

	}
	else
	{

	}
}

void ARZ_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/// Rotate self to target location.
	
	/*SetActorRotation(FRotator(
		0.0f,
		UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation).Yaw,
		0.0f
	));*/

	/// Pass down target location to equipped item.

	if (ItemManager->GetEquippedItem())
	{
		ItemManager->GetEquippedItem()->OwnerTargetLocation = TargetLocation;
	}

	///

	UpdateTargetSplineMesh();
}

void ARZ_Character::Init(ERZ_PawnOwnership Ownership)
{
	if (Ownership == ERZ_PawnOwnership::Player)
	{
		GameplayTags.AddTag(FGameplayTag::RequestGameplayTag(FName("PawnOwnership.Player"))); 
	}
	else
	{
		GameplayTags.AddTag(FGameplayTag::RequestGameplayTag(FName("PawnOwnership.AI"))); 
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

#pragma region +++ Combat ...

void ARZ_Character::OnProjectileCollision(float ProjectileDamage, const FVector& HitLocation, AController* InstigatorController)
{
	if (GetLocalRole() < ROLE_Authority)
		return;

	if (PawnCombatComp == nullptr)
		return;

	PawnCombatComp->ApplyDamage(ProjectileDamage, HitLocation, InstigatorController, nullptr);
}

void ARZ_Character::OnDeath()
{
	SetLifeSpan(10.0f);
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
		GetMesh()->SetCollisionProfileName("PawnMesh_Collision");
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bBlendPhysics = true;
	}
}

void ARZ_Character::SetOnHitMaterial(bool bNewIsEnabled)
{
}

#pragma endregion

void ARZ_Character::StartHover()
{
	UE_LOG(LogTemp, Warning, TEXT("ARZ_Character::StartHover"));
	
	if (GetMesh())
	{
		GetMesh()->SetCustomDepthStencilValue(1);
		GetMesh()->SetRenderCustomDepth(true);
	}
}

void ARZ_Character::StopHover()
{
	if (GetMesh())
	{
		GetMesh()->SetRenderCustomDepth(false);
	}
}

void ARZ_Character::Use(ARZ_PlayerController* InstigatorController)
{
	//Cast<ARZ_PlayerController>(InstigatorController)->StartPossessCharacter(this);
	//InstigatorController->GetOwnedCharacter()->GetItemManager()->SetNewTargetItemManager(ItemManager);
}

/*URZ_ItemManagerComponent* ARZ_Character::GetItemManager()
{
	return ItemManager;
}*/

void ARZ_Character::OnItemSpawned(ARZ_Item* SpawnedItem)
{
	SpawnedItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_rSocket");
}

void ARZ_Character::OnItemEquipped(const FName& ItemName)
{
	/*const FRZ_ItemData* ItemData = ItemManager->GetIMPluginSettings()->GetItemDataFromRow(ItemName);
	if (ItemData)
	{
		GetWorld()->SpawnActorDeferred<ARZ_SpawnManager>(
			ARZ_SpawnManager::StaticClass(),
			FTransform(),
			this,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
		if (SpawnManager)
		{
			UGameplayStatics::FinishSpawningActor(SpawnManager, FTransform());
		}
	}*/

	// Link weapons delegates to update animations
}

void ARZ_Character::OnUsed(class ARZ_BattlePlayerController* PlayerController)
{
	// authority

	//PlayerController->GetOwnedCharacter()->GetItemManager()->SetNewTargetItemManager(ItemManager);
}

void ARZ_Character::SetupTargetSplineMesh()
{
	TargetSplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	TargetSplineMesh->RegisterComponentWithWorld(GetWorld());
	TargetSplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	TargetSplineMesh->SetMobility(EComponentMobility::Movable);
	TargetSplineMesh->SetStaticMesh(GameSettings->EngineCylinderMesh);
	TargetSplineMesh->SetMaterial(0, GameSettings->TargetSplineMeshMaterial);
	TargetSplineMesh->SetWorldScale3D(FVector(1.0f, 0.05f, 0.05f));
	TargetSplineMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	TargetSplineMesh->SetForwardAxis(ESplineMeshAxis::Z, true);
}

void ARZ_Character::UpdateTargetSplineMesh()
{
	if (IsLocallyControlled() == false)
		return;
	
	if (TargetSplineMesh == nullptr)
		return;
	
	if (ItemManager && ItemManager->GetEquippedItem() != nullptr)
	{
		/// Snap mesh to projectile plane.

		TargetSplineMesh->SetWorldLocation(FVector(
			GetMesh()->GetSocketLocation("hand_rSocket").X,
			GetMesh()->GetSocketLocation("hand_rSocket").Y,
			TOPDOWNPROJECTILEPLANEHEIGHT
		));

		//
		
		/*const FVector Start = FVector(OwnerCharacter->GetProjectileStartSphere()->GetComponentLocation().X, OwnerCharacter->GetProjectileStartSphere()->GetComponentLocation().Y, GInstance->GetGlobalData().ProjectilePlaneHeight);

		const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Start, OwnerCharacter->GetTargetPoint());
		const FVector End = Start + (Rotation.Vector() * 50000);
		const FCollisionQueryParams TraceParams;
		FHitResult Hit;

		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);*/

		const float Distance = FVector::Dist(TargetSplineMesh->GetComponentLocation(), TargetLocation);

		TargetSplineMesh->SetEndPosition(FVector(Distance, 0.0f, 0.0f), true);
		TargetSplineMesh->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(TargetSplineMesh->GetComponentLocation(), TargetLocation));

		TargetSplineMesh->SetVisibility(true);
	}
	else
	{
		TargetSplineMesh->SetVisibility(false);
	}
}

const FRZ_CharacterAnimData& ARZ_Character::GetCharacterAnimData()
{
	if (CharacterMovement)
	{
		if (CharacterMovement->GetIsSprinting())
		{
			CharacterAnimData.LowerBodyAnimStance = ERZ_LowerBodyAnimStance::Run;
		}
		else
		{
			CharacterAnimData.LowerBodyAnimStance = ERZ_LowerBodyAnimStance::IdleWalk;
		}
	}
	
	if (ItemManager)
	{
		if (ItemManager->GetEquippedItem())
		{
			if (ItemManager->GetEquippedItem()->GetItemData())
			{
				switch (ItemManager->GetEquippedItem()->GetItemData()->AnimType)
				{
				case ERZ_ItemAnimType::Hands:
					CharacterAnimData.UpperBodyAnimStance = ERZ_UpperBodyAnimStance::Hands;
					break;
				case ERZ_ItemAnimType::Sword:
					CharacterAnimData.UpperBodyAnimStance = ERZ_UpperBodyAnimStance::Sword;
					break;
				case ERZ_ItemAnimType::Pistol:
					CharacterAnimData.UpperBodyAnimStance = ERZ_UpperBodyAnimStance::Pistol;
					break;
				case ERZ_ItemAnimType::Rifle:
					CharacterAnimData.UpperBodyAnimStance = ERZ_UpperBodyAnimStance::Rifle;
					break;
				}
			}
		}
	}
	
	return CharacterAnimData;
}

void ARZ_Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION(ARZ_Character, TargetLocation, COND_SkipOwner);
}

