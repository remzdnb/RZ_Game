// Copyright Epic Games, Inc. All Rights Reserved.

#include "RZM_InventorySystem.h"

#define LOCTEXT_NAMESPACE "FRZP_InventorySystemModule"

void FRZM_InventorySystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FRZM_InventorySystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRZM_InventorySystemModule, RZM_InventorySystem)

URZ_InventorySystemModuleSettings::URZ_InventorySystemModuleSettings()
{
	
}

const FRZ_InventoryItemSettings* URZ_InventorySystemModuleSettings::GetInventoryItemSettingsFromDataTable(const FName& RowName) const
{
	if (InventoryItemSettingsDataTable == nullptr || RowName == "Empty")
		return nullptr;

	const FString ContextString;
	const FRZ_InventoryItemSettings* ItemData = InventoryItemSettingsDataTable->FindRow<FRZ_InventoryItemSettings>(RowName, ContextString);
	if (ItemData)
	{
		return ItemData;
	}

	UE_LOG(LogTemp, Error, TEXT("ARZ_ItemPluginDataManager::GetItemInfoFromRow - Row not found : %s"), *RowName.ToString());
	return nullptr;
}