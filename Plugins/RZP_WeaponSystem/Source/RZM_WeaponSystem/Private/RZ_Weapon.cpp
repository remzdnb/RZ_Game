#include "RZ_Weapon.h"
#include "RZM_WeaponSystem.h"
//
#include "EngineUtils.h"
#include "GameFramework/GameStateBase.h"

ARZ_Weapon::ARZ_Weapon() :
	ItemState(ERZ_ItemState::Holstered)
{
	RootSceneComp = CreateDefaultSubobject<USceneComponent>(FName("RootSceneCT"));
	RootComponent = RootSceneComp;

	PrimaryActorTick.bCanEverTick = true;

	bWantsToUse = false;
	LastUseTime = 0.0f;
}

void ARZ_Weapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InitItemSettings(GetWorld(), DataTableRowName);
}

void ARZ_Weapon::BeginPlay()
{
	Super::BeginPlay();

	//OwnerPawnInterface = Cast<IRZ_PawnItemInterface>(GetOwner());
}

void ARZ_Weapon::SetWantToUse(bool bNewWantsToUse)
{
	bWantsToUse = bNewWantsToUse;
}

void ARZ_Weapon::SetItemState(ERZ_ItemState NewItemState)
{
	ItemState = NewItemState;
}


