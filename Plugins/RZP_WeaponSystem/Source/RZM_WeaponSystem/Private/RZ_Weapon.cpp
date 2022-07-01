#include "RZ_Weapon.h"
#include "RZ_SensingComponent.h"
//
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "RZ_CombatComponent.h"

ARZ_Weapon::ARZ_Weapon() :
	ItemState(ERZ_WeaponState::Ready)
{
	RootSceneComp = CreateDefaultSubobject<USceneComponent>(FName("RootSceneComp"));
	RootComponent = RootSceneComp;

	RootMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(FName("RootMeshComp"));
	RootMeshComp->SetCollisionProfileName("IgnoreAll");
	RootMeshComp->SetCustomDepthStencilValue(1);
	RootMeshComp->SetupAttachment(RootComponent);

	CombatComp = CreateDefaultSubobject<URZ_CombatComponent>(FName("CombatComp"));
	
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	bIsTurretWeapon = false;
	bWantToUse = false;
	LastUseTime = 0.0f;
}

void ARZ_Weapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!GetWorld()->IsGameWorld()) { return; }

	InitActorSettings(GetWorld(), DataTableRowName);

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

	WeaponSystemModuleSettings = Cast<IRZ_WeaponSystemModuleInterface>(GetGameInstance())
		->GetWeaponSystemModuleSettings();
}

void ARZ_Weapon::BeginPlay()
{
	Super::BeginPlay();

	SetActorMode(ERZ_ActorMode::Hidden_Disabled);
}

void ARZ_Weapon::SetPlayerTargetLocation(const FVector& NewPlayerTargetLocation)
{
	PlayerTargetLocation = NewPlayerTargetLocation;
}

/*void ARZ_Weapon::OnInventorySelection(bool bNewIsSelected)
{
	bIsSelected = bNewIsSelected;
}*/

void ARZ_Weapon::SetItemState(ERZ_WeaponState NewItemState)
{
	ItemState = NewItemState;
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

#pragma region +++ RZ_ActorInterface ...

const FName& ARZ_Weapon::GetTableRowName()
{
	return DataTableRowName;
}

void ARZ_Weapon::SetWantToUse(bool bNewWantToUse)
{
	bWantToUse = bNewWantToUse;
}

void ARZ_Weapon::SetActorMode(ERZ_ActorMode NewActorMode)
{
	IRZ_ActorInterface::SetActorMode(NewActorMode);

	if (NewActorMode == ERZ_ActorMode::Hidden_Disabled || ActorMode == ERZ_ActorMode::Visible_Disabled)
	{
		if (TurretComp) { TurretComp->ToggleTurretAI(false); }
		
		SetWantToUse(false);
	}
	else
	{
		if (TurretComp) { TurretComp->ToggleTurretAI(true); }
	}
}

#pragma endregion

