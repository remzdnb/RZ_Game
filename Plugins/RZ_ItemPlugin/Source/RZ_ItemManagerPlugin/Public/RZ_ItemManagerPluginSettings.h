// RemzDNB

#pragma once

#include "RZ_ItemActorPluginTypes.h"
#include "RZ_ItemManagerPluginTypes.h"
//
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RZ_ItemManagerPluginSettings.generated.h"

UCLASS()
class RZ_ITEMMANAGERPLUGIN_API URZ_ItemManagerPluginSettings : public UDataAsset
{
	GENERATED_BODY()

public:

	URZ_ItemManagerPluginSettings();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	class UDataTable* AttachedItemSlotsConfigsDT;

	//
	
	const FRZ_SlotConfig* GetAttachedItemSlotConfigFromRow(const FName& RowName) const;
};
