/// RemzDNB
///
///	RZ_GameInstance.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_Shared.h"
#include "RZM_WeaponSystem.h"
#include "RZM_UISystem.h"
#include "RZM_InventorySystem.h"
///
#include "Engine/GameInstance.h"
#include "RZ_GameInstance.generated.h"

class URZ_GameSettings;
class URZ_UISystemModuleSettings;

UCLASS()
class RZ_GAME_API URZ_GameInstance : public UGameInstance,
                                     public IRZ_SharedModuleInterface,
                                     public IRZ_WeaponSystemModuleInterface,
                                     public IRZ_InventorySystemModuleInterface,
                                     public IRZ_UISystemPluginSettingsInterface
{
	GENERATED_BODY()
	
public:

	virtual URZ_GameSettings* GetGameSettings() const { return GameSettingsDataAsset; }
	virtual const URZ_SharedModuleSettings* GetSharedModuleSettings() override { return SharedModuleSettingsDataAsset; }
	virtual const URZ_WeaponSystemModuleSettings* GetWeaponSystemModuleSettings() override { return WeaponSystemModuleSettingsDataAsset; }
	virtual URZ_InventorySystemModuleSettings* GetInventorySystemModuleSettings() override { return InventorySystemModuleSettingsDataAsset; }
	virtual URZ_UISystemModuleSettings* GetUIManagerModuleSettings() override { return UIManagerModuleSettingsDataAsset; }

	const UDataAsset* GetS2DGameSettings() const { return S2DGameSettings; }
	const UDataAsset* GetS3DGameSettings() const { return S3DGameSettings; }
	
private :

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	URZ_GameSettings* GameSettingsDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	UDataAsset* S2DGameSettings;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	UDataAsset* S3DGameSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	URZ_SharedModuleSettings* SharedModuleSettingsDataAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	URZ_WeaponSystemModuleSettings* WeaponSystemModuleSettingsDataAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	URZ_InventorySystemModuleSettings* InventorySystemModuleSettingsDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	URZ_UISystemModuleSettings* UIManagerModuleSettingsDataAsset;
};
