/// RemzDNB
///
///	RZ_GameInstance.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_InventorySystem.h"
#include "RZ_BuildingSystem.h"
#include "RZ_PowerSystem.h"
#include "RZ_UISystem.h"
///
#include "Engine/GameInstance.h"
#include "RZ_GameInstance.generated.h"

class URZ_GameSettings;
class URZ_UISystemSettings;

UCLASS()
class RZ_GAME_API URZ_GameInstance : public UGameInstance,
                                     public IRZ_InventorySystemInterface,
                                     public IRZ_BuildingSystemInterface,
                                     public IRZ_UISystemPluginSettingsInterface,
                                     public IRZ_PowerSystemSettingsInterface
{
	GENERATED_BODY()

public:

	virtual const URZ_UISystemSettings* GetUIManagerModuleSettings() const override { return UISystemSettingsDataAsset; }
	virtual const URZ_InventorySystemSettings* GetInventorySystemSettings() const override { return InventorySystemSettingsDataAsset; }
	virtual const URZ_BuildingSystemSettings* GetBuildingSystemSettings() const override { return BuidlingSystemSettingsDataAsset; }
	virtual const URZ_PowerSystemSettings* GetPowerSystemSettings() const override { return PowerSystemSettingsDataAsset; }
	
	virtual URZ_GameSettings* GetGameSettings() const { return GameSettingsDataAsset; }
	const UDataAsset* GetS2DGameSettings() const { return S2DGameSettings; }
	const UDataAsset* GetS3DGameSettings() const { return S3DGameSettings; }
	
private :

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	URZ_UISystemSettings* UISystemSettingsDataAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	URZ_InventorySystemSettings* InventorySystemSettingsDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	URZ_BuildingSystemSettings* BuidlingSystemSettingsDataAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	URZ_PowerSystemSettings* PowerSystemSettingsDataAsset;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	URZ_GameSettings* GameSettingsDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UDataAsset* S2DGameSettings;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UDataAsset* S3DGameSettings;
};
