///	RemzDNB

// RZ_Game
#include "Character/RZ_Character.h"
#include "Character/RZ_CharacterMovementComponent.h"
#include "Core/RZ_GameInstance.h"
#include "Core/RZ_GameState.h"
#include "AI/RZ_PawnAIController.h"
#include "UI/RZ_OTM_Character_Widget.h"
#include "Weapon/RZ_Weapon.h"
// FrameworkPlugin
#include "RZ_AbilitySystemComponent.h"
#include "RZ_InventoryComponent.h"
#include "RZ_PerceptionComponent.h"
#include "RZ_BuildingComponent.h"
#include "RZ_CharacterAnimInstance.h"
#include "RZ_AttributeSet.h"
#include "RZ_GameplayAbility.h"
// Engine
#include "Components/SplineMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "GameplayTagContainer.h"

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
	
	OTMWidgetComp = CreateDefaultSubobject<UWidgetComponent>(FName("OTMWidgetComp"));;
	OTMWidgetComp->SetupAttachment(RootComponent);
	OTMWidgetComp->SetCollisionProfileName("IgnoreAll");
	OTMWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	OTMWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	OTMWidgetComp->SetDrawSize(FVector2D(400.0f, 50.f));
	
	InventoryComp = CreateDefaultSubobject<URZ_InventoryComponent>(FName("InventoryComp"));
	BuildingComp = CreateDefaultSubobject<URZ_BuildingComponent>(FName("BuildingComp"));
	PerceptionComp = CreateDefaultSubobject<URZ_PerceptionComponent>(FName("PerceptionComp"));
	//CombatComp = CreateDefaultSubobject<URZ_AttributeComponent>(FName("CombatComp"));

	ASComp = CreateDefaultSubobject<URZ_AbilitySystemComponent>(FName("AbilitySystemComp"));
	ASComp->SetIsReplicated(true);
	ASComp->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	Attributes = CreateDefaultSubobject<URZ_AttributeSet>("Attributes");
	
	AIControllerClass = ARZ_PawnAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	bReplicates = true;
}

void ARZ_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetWorld()->IsGameWorld() == false)
		return;

	//if (Tags.Find("Demo")) { return; }

	GameState = Cast<ARZ_GameState>(GetWorld()->GetGameState());
	GameSettings = Cast<URZ_GameInstance>(GetGameInstance())->GetGameSettings();
	CharacterMovementCT = Cast<URZ_CharacterMovementComponent>(GetMovementComponent());
	CharacterAnimInstance = Cast<URZ_CharacterAnimInstance>(GetMesh()->GetAnimInstance());
	
	InventoryComp->OnNewStorageItemEquipped.AddUniqueDynamic(this, &ARZ_Character::OnInventoryItemEquipped);
}

void ARZ_Character::BeginPlay()
{
	Super::BeginPlay();

	SetActorHiddenInGame(false);
	
	if (!ASComp->HasMatchingGameplayTag(FRZ_CommonTags::Get().GameActor_Enabled))
	{
		SetActorTickEnabled(false);
		SetActorEnableCollision(false);
		SetActorHiddenInGame(false);
		//GetCapsuleComponent()->SetSimulatePhysics(false);
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//DisableComponentsSimulatePhysics();
	}

	InventoryComp->EquipStorageItem(1);
	
	BuildingComp->Init(nullptr, GetMesh());

	OTMWidget = Cast<URZ_OTM_Character_Widget>(OTMWidgetComp->GetWidget());
	if (OTMWidget)
	{
		OTMWidget->Init(Cast<ARZ_PawnAIController>(GetController()));
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

	/*UKismetMathLibrary::FindLookAtRotation(
		FVector(GetActorLocation().X, GetActorLocation().Y, BASEVIEWHEIGHT),
		PlayerTargetLocation)
	;*/


	SetAnimData(TempCharacterAnimData);

	//
}

void ARZ_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Server GAS init.
	ASComp->InitAbilityActorInfo(this, this);
	InitializeAttributes();
	AddDefaultAbilities();
}

void ARZ_Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Client GAS init.
	ASComp->InitAbilityActorInfo(this, this);
	InitializeAttributes();
}

void ARZ_Character::OnWorldHover(bool bNewIsHovered)
{
	GetMesh()->SetRenderCustomDepth(bNewIsHovered);
}

void ARZ_Character::OnWorldSelection(bool bNewSelected)
{

}

#pragma region +++ InventorySystem ...

void ARZ_Character::OnAttachedToInventory()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ARZ_Character::OnEquippedByInventory(bool bNewIsSelected)
{
	IRZ_InventoryItemInterface::OnEquippedByInventory(bNewIsSelected);

	bNewIsSelected ? BuildingComp->StartDemoBuild() : BuildingComp->StopDemoBuild();
}

void ARZ_Character::OnWantToBeUsedByInventory(bool bNewWantsTouse)
{
	if (bNewWantsTouse)
	{
		if (BuildingComp)
		{
			BuildingComp->StartBuild();
		}
	}
}

void ARZ_Character::OnInventoryItemEquipped(const FRZ_InventorySlotData& SlotData)
{
	const IRZ_InventorySystemInterface* InventorySystemInterface =
		Cast<IRZ_InventorySystemInterface>(GetWorld()->GetGameInstance());

	const FRZ_ItemSettings* ThisItemSettings =  InventorySystemInterface->GetInventorySystemSettings()
		->GetItemSettingsFromTableRow(SlotData.ActorName);
	if (!ThisItemSettings) { return; }

	FRZ_CharacterAnimData TempCharacterAnimData = GetCharacterAnimData();

	//UE_LOG(LogTemp, Warning, TEXT("ARZ_Character::OnInventoryItemEquipped : %s"), *ItemInterface->GetItemSettings().DisplayName.ToString());
	
	switch (ThisItemSettings->AnimType)
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

	/*if (ItemSettings->Type == ERZ_ItemType::Weapon)
	{
		ARZ_Weapon* Weapon = Cast<ARZ_Weapon>(SelectedItem);
		if (Weapon)
		{
			Weapon->OnWeaponFired.AddUniqueDynamic(this, &ARZ_Character::OnWeaponFired);
		}
	}*/

	SetAnimData(TempCharacterAnimData);
}
#pragma endregion

#pragma region +++ BuildingSystem ...

float ARZ_Character::GetBuildCenterZOffsetLocation() const
{
	float OutRadius;
	float OutHalfHeight;
	GetCapsuleComponent()->GetUnscaledCapsuleSize(OutRadius, OutHalfHeight);
	return OutHalfHeight;
}

void ARZ_Character::OnDemoBuildStart()
{
	SetActorTickEnabled(false);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(false);
}

void ARZ_Character::OnDemoBuildStop()
{
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ARZ_Character::OnBuildStart()
{
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void ARZ_Character::OnBuildStop()
{
	SetActorTickEnabled(false);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void ARZ_Character::OnBuildEnd()
{
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

#pragma endregion

#pragma region +++ Combat ...

void ARZ_Character::OnDeath()
{
	//SetLifeSpan(10.0f);

	GameState->ReportPawnEndPlay(this);
	
	if (Cast<AAIController>(GetOwner()))
	{
		//GetOwner()->Destroy();
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

	/*if (GetMesh() && GetMesh()->GetPhysicsAsset())
	{
		GetCapsuleComponent()->SetCollisionProfileName("PawnCapsule_Disabled");
		GetMesh()->SetCollisionProfileName("PawnMesh_Physics");
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->WakeAllRigidBodies();
		GetMesh()->bBlendPhysics = true;
	}*/
}

void ARZ_Character::SetOnHitMaterial(bool bNewIsEnabled)
{
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

#pragma region +++ GAS ...

void ARZ_Character::InitializeAttributes()
{
	if (ASComp && SetDefaultAttributesGEClass)
	{
		FGameplayEffectContextHandle EffectContext = ASComp->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		const FGameplayEffectSpecHandle SpecHandle = ASComp->MakeOutgoingSpec(
			SetDefaultAttributesGEClass,
			1,
			EffectContext
		);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle EffectHandle = ASComp->ApplyGameplayEffectSpecToSelf(
				*SpecHandle.Data.Get()
			);
			/*FActiveGameplayEffectHandle EffectHandle = ASComp->ApplyGameplayEffectSpecToTarget(
				*SpecHandle.Data.Get(),
				ASComp
			);*/
		}
	}
}

void ARZ_Character::AddDefaultAbilities()
{
	check(ASComp);
	
	if (HasAuthority())
	{
		for (TSubclassOf<URZ_GameplayAbility>& Ability : DefaultAbilities)
		{
			ASComp->GiveAbility(FGameplayAbilitySpec(
				Ability,
				1,
				0,
				this
			));
		}
	}
}

#pragma endregion