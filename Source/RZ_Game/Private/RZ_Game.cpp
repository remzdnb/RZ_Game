/// RemzDNB

#include "RZ_Game.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, RZ_Game, "RZ_Game" );

IRZ_PawnInterface::IRZ_PawnInterface()
{
}

void IRZ_PawnInterface::SetAssignedTarget(AActor* NewAssignedTarget)
{
	AssignedTarget = NewAssignedTarget;
}

URZ_GameSettings::URZ_GameSettings()
{
}

const FRZ_ProjectileWeaponSettings* URZ_GameSettings::GetProjectileWeaponSettingsFromTableRow(const FName& RowName) const
{
	if (ProjectileWeaponSettingsDataTable == nullptr)
		return nullptr;

	const FString ContextString;
	const FRZ_ProjectileWeaponSettings* ItemData = ProjectileWeaponSettingsDataTable->FindRow<FRZ_ProjectileWeaponSettings>(RowName, ContextString);
	if (ItemData)
	{
		return ItemData;
	}

	return nullptr;
}

