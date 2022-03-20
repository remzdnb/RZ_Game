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
	LastUsedTime = 0.0f;
}

void ARZ_Item::BeginPlay()
{
	Super::BeginPlay();
	
	ItemActorPluginSettings = Cast<IRZ_ItemActorEditorSettingsInterface>(GetGameInstance())->GetItemActorEditorSettings();
	
	const FRZ_ItemData* const NewItemData = ItemActorPluginSettings->GetItemDataFromRow(DataRowName);
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

