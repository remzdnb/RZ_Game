#include "Weapon/RZ_Weapon.h"
#include "RZ_PerceptionComponent.h"
//
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "RZ_InventoryComponent.h"
#include "Weapon/RZ_TurretComponent.h"
#include "Core/RZ_GameInstance.h"

ARZ_Weapon::ARZ_Weapon()
{
	RootSceneComp = CreateDefaultSubobject<USceneComponent>(FName("RootSceneComp"));
	RootComponent = RootSceneComp;
	
	RootMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(FName("RootMeshComp"));
	RootMeshComp->SetCollisionProfileName("IgnoreAll");
	RootMeshComp->SetCustomDepthStencilValue(1);
	RootMeshComp->SetupAttachment(RootComponent);

	InventoryComp = CreateDefaultSubobject<URZ_InventoryComponent>("InventoryComp");
	
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	bIsTurretWeapon = false;
	bOwnerWantToUse = false;
	LastUseTime = 0.0f;
}

void ARZ_Weapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!GetWorld()->IsGameWorld()) { return; }

	RZGameSettings = Cast<URZ_GameInstance>(GetWorld()->GetGameInstance())->GetGameSettings();
	
	// Init ActorSettings.

	IRZ_InventorySystemInterface* InventorySystemInterface =
		Cast<IRZ_InventorySystemInterface>(GetWorld()->GetGameInstance());

	if (!InventorySystemInterface) { return; }
	if (!InventorySystemInterface->GetInventorySystemSettings()) { return; }
	if (!InventorySystemInterface->GetInventorySystemSettings()->GetItemSettingsFromTableRow(GetItemName())) { return; }

	ItemSettings = *InventorySystemInterface->GetInventorySystemSettings()->GetItemSettingsFromTableRow(GetItemName());

	// Init
	
	if (bIsTurretWeapon)
	{
		TurretComp = NewObject<URZ_TurretComponent>(this, FName("TurretComp"));
		if (TurretComp)
		{
			TurretComp->RegisterComponent();
			TurretComp->Init(RootMeshComp);
			//TurretComp->OnTargetLocked.AddUniqueDynamic(this, &ARZ_Weapon::SetWantToUse);
		}
	}

	//WeaponSystemModuleSettings = Cast<IRZ_WeaponSystemModuleInterface>(GetGameInstance())
		//->GetWeaponSystemModuleSettings();
}

void ARZ_Weapon::BeginPlay()
{
	Super::BeginPlay();
}

void ARZ_Weapon::OnAttachedToInventory()
{
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	WeaponState = ERZ_WeaponState::Ready;

	const IRZ_InventoryOwnerInterface* InventoryOwnerInterface = Cast<IRZ_InventoryOwnerInterface>(GetOwner());
	if (InventoryOwnerInterface)
	{
		const FAttachmentTransformRules TransformRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
		AttachToComponent(InventoryOwnerInterface->GetAttachComponent(), TransformRules, "VB LHS_ik_hand_r");

		if (RZGameSettings->bDebugWeapons)
		{
			UE_LOG(LogTemp, Display, TEXT("%s :: OnAttachedToInventory - AttachComponent == %s"),
			       *this->GetName(), *GetNameSafe(InventoryOwnerInterface->GetAttachComponent()));
		}
	}
}

void ARZ_Weapon::OnEquippedByInventory(bool bNewIsSelected)
{
	SetActorHiddenInGame(!bNewIsSelected);
	SetActorTickEnabled(bNewIsSelected);

	if (bNewIsSelected)
		WeaponState = ERZ_WeaponState::Ready;
	else
	{
		WeaponState = ERZ_WeaponState::Disabled;
	}
}
