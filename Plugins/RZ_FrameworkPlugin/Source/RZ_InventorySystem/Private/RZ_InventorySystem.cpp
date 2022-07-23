// Copyright Epic Games, Inc. All Rights Reserved.

#include "RZ_InventorySystem.h"

#define LOCTEXT_NAMESPACE "FRZP_InventorySystemModule"

void FRZ_InventorySystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FRZ_InventorySystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRZ_InventorySystemModule, RZ_InventorySystem)

URZ_InventorySystemSettings::URZ_InventorySystemSettings()
{
	
}

const FRZ_ItemSettings* URZ_InventorySystemSettings::GetItemSettingsFromTableRow(const FName& RowName) const
{
	if (ItemSettingsDataTable == nullptr)
		return nullptr;

	const FString ContextString;
	const FRZ_ItemSettings* ItemData = ItemSettingsDataTable->FindRow<FRZ_ItemSettings>(RowName, ContextString);
	if (ItemData)
	{
		return ItemData;
	}

	UE_LOG(LogTemp, Error, TEXT("URZ_SharedModuleSettings::GetActorSettingsFromTableRow - Row not found : %s"), *RowName.ToString());
	return nullptr;
}


