#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RZ_ItemManagerPluginTypes.generated.h"

USTRUCT(BlueprintType)
struct RZ_ITEMMANAGERPLUGIN_API FRZ_SlotConfig : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 SlotID;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> AllowedCategories;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class ARZ_Item* AttachedItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName DefaultItemName;

	
	FRZ_SlotConfig()
	{
		SlotID = 0;
		SlotName = "DefaultSlotName";
		AttachedItem = nullptr;
		DefaultItemName = "DefaultItemName";
	}
};
