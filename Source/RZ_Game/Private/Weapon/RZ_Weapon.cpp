#include "Weapon/RZ_Weapon.h"
#include "RZ_SensingComponent.h"
//
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "RZ_AttributeComponent.h"
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

	//CombatComp = CreateDefaultSubobject<URZ_AttributeComponent>(FName("CombatComp"));
	
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	bIsTurretWeapon = false;
	bWantToUse = false;
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
			TurretComp->OnTargetLocked.AddUniqueDynamic(this, &ARZ_Weapon::SetWantToUse);
		}
	}

	//WeaponSystemModuleSettings = Cast<IRZ_WeaponSystemModuleInterface>(GetGameInstance())
		//->GetWeaponSystemModuleSettings();
}

void ARZ_Weapon::BeginPlay()
{
	Super::BeginPlay();
}

void ARZ_Weapon::OnAttachedToInventory(URZ_InventoryComponent* InventoryCompRef)
{
	UE_LOG(LogTemp, Warning, TEXT("ARZ_Weapon::OnAttachedToInventory"));
	
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	WeaponState = ERZ_WeaponState::Ready;

	IRZ_InventoryOwnerInterface* InventoryOwnerInterface = Cast<IRZ_InventoryOwnerInterface>(InventoryCompRef->GetOwner());
	if (InventoryOwnerInterface)
	{
		
	}
	const FAttachmentTransformRules TransformRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	AttachToComponent(InventoryOwnerInterface->GetAttachComponent(), TransformRules, "VB IK_Hand_R");
}

void ARZ_Weapon::OnSelectedByInventory(bool bNewIsSelected)
{
	SetActorHiddenInGame(!bNewIsSelected);
	SetActorTickEnabled(bNewIsSelected);

	if (bNewIsSelected)
		WeaponState = ERZ_WeaponState::Ready;
	else
	{
		WeaponState = ERZ_WeaponState::Disabled;
	}

	UE_LOG(LogTemp, Warning, TEXT("ARZ_Weapon::OnSelectedByInventory %i"), bNewIsSelected);
}

void ARZ_Weapon::CalcSingleTrace(TArray<FHitResult> HitResults, const FVector& TraceStart, const FVector& TraceEnd)
{
	//const FVector TraceStart = RootSkeletalMeshCT->GetSocketLocation("MuzzleSocket_00");
	//const FVector TraceEnd = PlayerTargetLocation;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetOwner());

	// well we need to check for teams here, so maybe with gameplay tags / abilities ?
	
	GetWorld()->LineTraceMultiByChannel(
		HitResults,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		TraceParams
	);
}
