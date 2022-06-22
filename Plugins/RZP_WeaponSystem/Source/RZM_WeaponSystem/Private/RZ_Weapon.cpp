#include "RZ_Weapon.h"
#include "RZM_WeaponSystem.h"
//
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

ARZ_Weapon::ARZ_Weapon() :
	ItemState(ERZ_WeaponState::Ready)
{
	RootSceneCT = CreateDefaultSubobject<USceneComponent>(FName("RootSceneCT"));
	RootComponent = RootSceneCT;

	RootSkeletalMeshCT = CreateDefaultSubobject<USkeletalMeshComponent>(FName("RootSkeletalMeshCT"));
	RootSkeletalMeshCT->SetCollisionProfileName("IgnoreAll");
	RootSkeletalMeshCT->SetCustomDepthStencilValue(1);
	RootSkeletalMeshCT->SetupAttachment(RootComponent);

	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;

	bWantToUse = false;
	LastUseTime = 0.0f;
}

void ARZ_Weapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!GetWorld()->IsGameWorld()) { return; }

	InitItemSettings(GetWorld(), DataTableRowName);

	WeaponSystemModuleSettings = Cast<IRZ_WeaponSystemModuleInterface>(GetGameInstance())
		->GetWeaponSystemModuleSettings();
}

void ARZ_Weapon::BeginPlay()
{
	Super::BeginPlay();
}

void ARZ_Weapon::SetPlayerTargetLocation(const FVector& NewPlayerTargetLocation)
{
	PlayerTargetLocation = NewPlayerTargetLocation;
}

void ARZ_Weapon::OnInventorySelection(bool bNewIsSelected)
{
	bIsSelected = bNewIsSelected;
}

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

#pragma region +++ ItemInterace ...

const FName& ARZ_Weapon::GetTableRowName()
{
	return DataTableRowName;
}

void ARZ_Weapon::SetWantToUse(bool bNewWantToUse)
{
	bWantToUse = bNewWantToUse;
}

#pragma endregion

