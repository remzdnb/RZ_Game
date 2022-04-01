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

void ARZ_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
//	ItemActorPluginSettings = Cast<IRZ_WeaponSystemModuleInterface>(GetGameInstance())->GetWeaponSystemModuleSettings();
	
	/*const FRZ_ItemInfo* NewItemData = ItemActorPluginSettings->GetItemInfoFromRow(DataTableRowName);
	if (NewItemData)
	{
		ItemData = NewItemData;
	}*/

	//OwnerPawnInterface = Cast<IRZ_PawnItemInterface>(GetOwner());
}

void ARZ_Weapon::SetWantsToUse(bool bNewWantsToUse)
{
	bWantsToUse = bNewWantsToUse;
}

void ARZ_Weapon::SetItemState(ERZ_ItemState NewItemState)
{
	ItemState = NewItemState;
}


