// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RZM_ItemManager.h"

#define LOCTEXT_NAMESPACE "FRZM_ItemManagerModule"

void FRZM_ItemManagerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FRZM_ItemManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRZM_ItemManagerModule, RZM_ItemManager)

//

URZ_ItemManagerEditorSettings::URZ_ItemManagerEditorSettings()
{
	
}

const FRZ_ItemSlotSettings* URZ_ItemManagerEditorSettings::GetAttachedItemSlotConfigFromRow(
	const FName& RowName) const
{
	if (AttachedItemSlotsConfigsDT == nullptr)
		return nullptr;

	if (RowName == "Empty")
		return nullptr;

	FString ContextString;
	FRZ_ItemSlotSettings* AttachedItemSlotConfig =
		AttachedItemSlotsConfigsDT->FindRow<FRZ_ItemSlotSettings>(RowName, ContextString);
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