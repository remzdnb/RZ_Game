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

TArray<FRZ_ItemData*> URZ_ItemActorModuleSettings::GetItemDataArray() const
{
	if (ItemDT == nullptr)
		return TArray<FRZ_ItemData*>();

	return TArray<FRZ_ItemData*>();
	//return ItemDT->GetAllRows()
	//return TArray<FRZ_ItemData*>();
}

const FRZ_ItemData* const URZ_ItemActorModuleSettings::GetItemDataFromRow(FName RowName) const
{
	if (ItemDT == nullptr)
		return nullptr;

	if (RowName == "Empty")
		return nullptr;

	FString ContextString;
	FRZ_ItemData* ItemData = ItemDT->FindRow<FRZ_ItemData>(RowName, ContextString);
	if (ItemData)
	{
		return ItemData;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ARZ_ItemPluginDataManager::GetItemDataFromRow - Row not found : %s"), *RowName.ToString());
	}

	return nullptr;
}


const FRZ_ProjectileWeaponData* const URZ_ItemActorModuleSettings::GetProjectileWeaponDataFromRow(FName RowName) const
{
	if (ProjectileWeaponDT == nullptr)
		return nullptr;

	FString ContextString;
	FRZ_ProjectileWeaponData* WeaponData = ProjectileWeaponDT->FindRow<FRZ_ProjectileWeaponData>(RowName, ContextString);
	if (WeaponData)
	{
		return WeaponData;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ARZ_ItemPluginDataManager::GetWeaponDataFromRow - Row not found : %s"), *RowName.ToString());
		return nullptr;
	}
}

const FRZ_MeleeWeaponData* const URZ_ItemActorModuleSettings::GetMeleeWeaponDataFromRow(FName RowName) const
{
	if (MeleeWeaponDT == nullptr)
		return nullptr;

	FString ContextString;
	FRZ_MeleeWeaponData* MeleeWeaponData = MeleeWeaponDT->FindRow<FRZ_MeleeWeaponData>(RowName, ContextString);
	if (MeleeWeaponData)
	{
		return MeleeWeaponData;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ARZ_ItemPluginDataManager::GetMeleeWeaponDataFromRow - Row not found : %s"), *RowName.ToString());
		return nullptr;
	}
}
