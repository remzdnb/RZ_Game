/// RemzDNB

#include "RZM_Shared.h"

#define LOCTEXT_NAMESPACE "FRZM_SharedModule"

void FRZM_SharedModule::StartupModule()
{
}

void FRZM_SharedModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRZM_SharedModule, RZM_Shared)

URZ_SharedModuleSettings::URZ_SharedModuleSettings()
{
}

const FRZ_ItemSettings* URZ_SharedModuleSettings::GetItemSettingsFromTableRow(const FName& RowName) const
{
	if (InventoryItemSettingsDataTable == nullptr || RowName == "Empty")
		return nullptr;

	const FString ContextString;
	const FRZ_ItemSettings* ItemData = InventoryItemSettingsDataTable->FindRow<FRZ_ItemSettings>(RowName, ContextString);
	if (ItemData)
	{
		return ItemData;
	}

	UE_LOG(LogTemp, Error, TEXT("ARZ_ItemPluginDataManager::GetItemInfoFromRow - Row not found : %s"), *RowName.ToString());
	return nullptr;
}

void IRZ_ItemInterface::InitItemSettings(const UWorld* World, const FName& TableRowName)
{
	IRZ_SharedModuleInterface* SharedModuleInterface =
		Cast<IRZ_SharedModuleInterface>(World->GetGameInstance());

	if (!SharedModuleInterface) { return; }
	
	ItemSettings = *SharedModuleInterface->GetSharedModuleSettings()->GetItemSettingsFromTableRow(TableRowName);
}

void IRZ_ItemInterface::OnSelectionUpdated(bool bNewIsSelected)
{
}

void IRZ_ItemInterface::OnHoverStart()
{
}

void IRZ_ItemInterface::OnHoverEnd()
{
}

void IRZ_ItemInterface::EnableBuildMode(bool bNewIsEnabled)
{
}

void IRZ_ItemInterface::UpdateBuildModeLocation(const FVector& SpawnLocation, const FVector& LerpedItemLocation)
{
}

void IRZ_ItemInterface::SetBuildMeshVisibility(bool bNewIsVisible)
{
}

bool IRZ_ItemInterface::IsValidBuildLocation()
{
	return true; // Don't use super xD
}

void IRZ_ItemInterface::SetWantToUse(bool bNewWantTouse)
{
}

void IRZ_ItemInterface::SetWantToUse_Secondary(bool bNewWantToUse)
{
	
}
