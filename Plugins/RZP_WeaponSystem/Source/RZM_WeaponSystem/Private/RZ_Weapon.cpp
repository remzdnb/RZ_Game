#include "RZ_Weapon.h"
#include "RZM_WeaponSystem.h"
//
#include "EngineUtils.h"
#include "GameFramework/GameStateBase.h"

ARZ_Weapon::ARZ_Weapon() :
	ItemState(ERZ_WeaponState::Ready)
{
	RootSceneCT = CreateDefaultSubobject<USceneComponent>(FName("RootSceneCT"));
	RootComponent = RootSceneCT;

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

void ARZ_Weapon::SetControllerTargetLocation(const FVector& NewPlayerTargetLocation)
{
	PlayerTargetLocation = NewPlayerTargetLocation;
}

void ARZ_Weapon::OnSelectionUpdated(bool bNewIsSelected)
{
	SetIsEquipped(bNewIsSelected);
}

void ARZ_Weapon::SetItemState(ERZ_WeaponState NewItemState)
{
	ItemState = NewItemState;
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

