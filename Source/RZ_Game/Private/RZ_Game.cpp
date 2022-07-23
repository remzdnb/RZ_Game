/// RemzDNB

#include "RZ_Game.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, RZ_Game, "RZ_Game" );

IRZ_AIOwnerInterface::IRZ_AIOwnerInterface()
{
}

URZ_GameSettings::URZ_GameSettings()
{
}

const FRZ_DistanceWeaponSettings* URZ_GameSettings::GetProjectileWeaponSettingsFromTableRow(const FName& RowName) const
{
	if (ProjectileWeaponSettingsDataTable == nullptr)
		return nullptr;

	const FString ContextString;
	const FRZ_DistanceWeaponSettings* ItemData = ProjectileWeaponSettingsDataTable->FindRow<FRZ_DistanceWeaponSettings>(RowName, ContextString);
	if (ItemData)
	{
		return ItemData;
	}

	return nullptr;
}

IRZ_WorldInteractionInterface::IRZ_WorldInteractionInterface()
{
}
