// RemzDNB

#include "RZ_ItemManagerPluginSettings.h"

URZ_ItemManagerPluginSettings::URZ_ItemManagerPluginSettings()
{
	
}

const FRZ_SlotConfig* URZ_ItemManagerPluginSettings::GetAttachedItemSlotConfigFromRow(
	const FName& RowName) const
{
	if (AttachedItemSlotsConfigsDT == nullptr)
		return nullptr;

	if (RowName == "Empty")
		return nullptr;

	FString ContextString;
	FRZ_SlotConfig* AttachedItemSlotConfig =
		AttachedItemSlotsConfigsDT->FindRow<FRZ_SlotConfig>(RowName, ContextString);
	if (AttachedItemSlotConfig)
	{
		return AttachedItemSlotConfig;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ARZ_ItemPluginDataManager::GetItemDataFromRow - Row not found : %s"), *RowName.ToString());
	}

	return nullptr;
}