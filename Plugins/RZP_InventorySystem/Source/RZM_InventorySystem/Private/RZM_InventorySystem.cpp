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

void IRZ_InventoryActorInterface::OnAttachedToInventory(URZ_InventoryComponent* InventoryCompRef)
{
	OwnerInventory = InventoryCompRef;
}
