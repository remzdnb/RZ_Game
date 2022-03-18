// RemzDNB

#include "RZ_ItemActorPluginSettings.h"

URZ_ItemActorPluginSettings::URZ_ItemActorPluginSettings()
{
	
}

TArray<FRZ_ItemData*> URZ_ItemActorPluginSettings::GetItemDataArray() const
{
	if (ItemDT == nullptr)
		return TArray<FRZ_ItemData*>();

	return TArray<FRZ_ItemData*>();
	//return ItemDT->GetAllRows()
	//return TArray<FRZ_ItemData*>();
}

const FRZ_ItemData* const URZ_ItemActorPluginSettings::GetItemDataFromRow(FName RowName) const
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


const FRZ_ProjectileWeaponData* const URZ_ItemActorPluginSettings::GetProjectileWeaponDataFromRow(FName RowName) const
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

const FRZ_MeleeWeaponData* const URZ_ItemActorPluginSettings::GetMeleeWeaponDataFromRow(FName RowName) const
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
