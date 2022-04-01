// Copyright Epic Games, Inc. All Rights Reserved.

#include "RZM_WeaponSystem.h"

#define LOCTEXT_NAMESPACE "FRZM_WeaponSystemModule"

void FRZM_WeaponSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FRZM_WeaponSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRZM_WeaponSystemModule, RZM_WeaponSystem)

URZ_WeaponSystemModuleSettings::URZ_WeaponSystemModuleSettings()
{
	
}

const FRZ_ProjectileWeaponSettings* URZ_WeaponSystemModuleSettings::GetProjectileWeaponInfoFromRow(const FName& RowName) const
{
	if (ProjectileWeaponInfoDT == nullptr || RowName == "Empty")
		return nullptr;

	const FString ContextString;
	const FRZ_ProjectileWeaponSettings* WeaponData = ProjectileWeaponInfoDT->FindRow<FRZ_ProjectileWeaponSettings>(RowName, ContextString);
	if (WeaponData)
	{
		return WeaponData;
	}

	UE_LOG(LogTemp, Error, TEXT("ARZ_WeaponPluginDataManager::GetProjectileWeaponInfoFromRow - Row not found : %s"), *RowName.ToString());
	return nullptr;
}

const FRZ_MeleeWeaponSettings* URZ_WeaponSystemModuleSettings::GetMeleeWeaponInfoFromRow(const FName& RowName) const
{
	if (MeleeWeaponInfoDT == nullptr || RowName == "Empty")
		return nullptr;

	const FString ContextString;
	const FRZ_MeleeWeaponSettings* MeleeWeaponData = MeleeWeaponInfoDT->FindRow<FRZ_MeleeWeaponSettings>(RowName, ContextString);
	if (MeleeWeaponData)
	{
		return MeleeWeaponData;
	}

	UE_LOG(LogTemp, Error, TEXT("ARZ_WeaponPluginDataManager::GetMeleeWeaponInfoFromRow - Row not found : %s"), *RowName.ToString());
	return nullptr;
}