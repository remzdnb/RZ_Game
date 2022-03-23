///	RemzDNB

#include "Character/RZ_Character.h"
#include "Game/RZ_GameInstance.h"
#include "Game/RZ_GameState.h"
#include "Game/RZ_GameSettings.h"
/// CharacterPlugin
#include "RZ_CharacterMovementComponent.h"
/// ItemPlugin
#include "RZ_Item.h"
#include "RZ_ItemManagerComponent.h"
/// Engine
#include "Components/SplineMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/RZ_PlayerController.h"
#include "Net/UnrealNetwork.h"

ARZ_Character::ARZ_Character(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<URZ_CharacterMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
	GetCapsuleComponent()->InitCapsuleSize(28.0f, 85.0f);
	GetCapsuleComponent()->SetCollisionProfileName("Pawn");

	GetMesh()->SetRelativeLocation(DEFAULTRELATIVEMESHLOCATION);
	GetMesh()->SetRelativeRotation(DEFAULTRELATIVEMESHROTATION);
	GetMesh()->SetCollisionProfileName("CharacterMesh");
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCustomDepthStencilValue(1);
	
	ItemManager = CreateDefaultSubobject<URZ_ItemManagerComponent>(FName("ItemManager"));

	bUseControllerRotationPitch = false; // not here ?
	bUseControllerRotationYaw = false; // not here ?
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
	//ItemManager->OnItemSpawned.AddUniqueDynamic(this, &ARZ_Character::OnItemSpawned);
}

void ARZ_Character::BeginPlay()
{
	Super::BeginPlay();

	GameSettings = Cast<URZ_GameInstance>(GetGameInstance())->GetGameSettings();
	
	BehaviorTree = GameSettings->CharacterBehaviorTree;
	
	SetupTargetSplineMesh();
}

void ARZ_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/// Rotate self to target location.
	
	SetActorRotation(FRotator(
		0.0f,
		UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation).Yaw,
		0.0f
	));
/*
	/// Pass down target location to equipped item.

	if (ItemManager->GetEquippedItem())
	{
		ItemManager->GetEquippedItem()->OwnerTargetLocation = TargetLocation;
	}

	///

	UpdateTargetSplineMesh(); */
}

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
	SpawnedItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_r");
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
			TargetSplineMesh->GetComponentLocation().X,
			TargetSplineMesh->GetComponentLocation().Y,
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
	DOREPLIFETIME_CONDITION(ARZ_Character, bRotateToTarget, COND_SkipOwner);
}

