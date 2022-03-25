#include "RZ_Item.h"
#include "RZM_ItemActor.h"
//
#include "EngineUtils.h"
#include "GameFramework/GameStateBase.h"

ARZ_Item::ARZ_Item() :
	ItemState(ERZ_ItemState::Holstered)
{
	RootSceneCT = CreateDefaultSubobject<USceneComponent>(FName("RootSceneCT"));
	RootComponent = RootSceneCT;

	PrimaryActorTick.bCanEverTick = true;

	bWantsToUse = false;
	LastUseTime = 0.0f;
}

void ARZ_Item::BeginPlay()
{
	Super::BeginPlay();
	
	ItemActorPluginSettings = Cast<IRZ_ItemActorModuleInterface>(GetGameInstance())->GetItemActorModuleSettings();
	
	const FRZ_ItemInfo* NewItemData = ItemActorPluginSettings->GetItemInfoFromRow(DataTableRowName);
	if (NewItemData)
	{
		ItemData = NewItemData;
	}

	//OwnerPawnInterface = Cast<IRZ_PawnItemInterface>(GetOwner());
}

void ARZ_Item::SetWantsToUse(bool bNewWantsToUse)
{
	bWantsToUse = bNewWantsToUse;
}

void ARZ_Item::SetItemState(ERZ_ItemState NewItemState)
{
	ItemState = NewItemState;
}


