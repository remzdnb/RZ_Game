// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RZM_ItemActor.h"

#define LOCTEXT_NAMESPACE "FRZ_ItemPluginModule"

void FRZM_ItemActorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FRZM_ItemActorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRZM_ItemActorModule, RZM_ItemActor)

//

URZ_ItemActorModuleSettings::URZ_ItemActorModuleSettings()
{
	
}

const FRZ_ItemInfo* URZ_ItemActorModuleSettings::GetItemInfoFromRow(const FName& RowName) const
{
	if (ItemInfoDT == nullptr || RowName == "Empty")
		return nullptr;

	const FString ContextString;
	const FRZ_ItemInfo* ItemData = ItemInfoDT->FindRow<FRZ_ItemInfo>(RowName, ContextString);
	if (ItemData)
	{
		return ItemData;
	}

	UE_LOG(LogTemp, Error, TEXT("ARZ_ItemPluginDataManager::GetItemInfoFromRow - Row not found : %s"), *RowName.ToString());
	return nullptr;
}


const FRZ_ProjectileWeaponData* URZ_ItemActorModuleSettings::GetProjectileWeaponInfoFromRow(const FName& RowName) const
{
	if (ProjectileWeaponInfoDT == nullptr || RowName == "Empty")
		return nullptr;

	const FString ContextString;
	const FRZ_ProjectileWeaponData* WeaponData = ProjectileWeaponInfoDT->FindRow<FRZ_ProjectileWeaponData>(RowName, ContextString);
	if (WeaponData)
	{
		return WeaponData;
	}

	UE_LOG(LogTemp, Error, TEXT("ARZ_ItemPluginDataManager::GetProjectileWeaponInfoFromRow - Row not found : %s"), *RowName.ToString());
	return nullptr;
}

const FRZ_MeleeWeaponData* URZ_ItemActorModuleSettings::GetMeleeWeaponInfoFromRow(const FName& RowName) const
{
	if (MeleeWeaponInfoDT == nullptr || RowName == "Empty")
		return nullptr;

	const FString ContextString;
	const FRZ_MeleeWeaponData* MeleeWeaponData = MeleeWeaponInfoDT->FindRow<FRZ_MeleeWeaponData>(RowName, ContextString);
	if (MeleeWeaponData)
	{
		return MeleeWeaponData;
	}

	UE_LOG(LogTemp, Error, TEXT("ARZ_ItemPluginDataManager::GetMeleeWeaponInfoFromRow - Row not found : %s"), *RowName.ToString());
	return nullptr;
}
