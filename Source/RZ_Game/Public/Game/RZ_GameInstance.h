/// RemzDNB
///
///	RZ_GameInstance.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_ItemActor.h"
#include "RZM_ItemManager.h"
#include "RZM_ItemUI.h"
#include "RZM_UIManager.h"
///
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RZ_GameInstance.generated.h"

class URZ_GameSettings;
class URZ_ItemActorModuleSettings;
class URZ_ItemManagerModuleSettings;
class URZ_ItemUIModuleSettings;
class URZ_UIManagerModuleSettings;

UCLASS()
class RZ_GAME_API URZ_GameInstance : public UGameInstance,
                                     public IRZ_ItemActorModuleInterface,
                                     public IRZ_ItemManagerModuleInterface,
                                     public IRZ_ItemUIModuleInterface,
                                     public IRZ_UIManagerPluginSettingsInterface
{
	GENERATED_BODY()
	
public:

	virtual URZ_GameSettings* GetGameSettings() const { return GameSettingsDA; }
	virtual URZ_ItemActorModuleSettings* GetItemActorModuleSettings() override { return ItemActorModuleSettingsDA; }
	virtual URZ_ItemManagerModuleSettings* GetItemManagerModuleSettings() override { return ItemManagerModuleSettingsDA; }
	virtual URZ_ItemUIModuleSettings* GetItemUIModuleSettings() override { return ItemUIModuleSettingsDA; }
	virtual URZ_UIManagerModuleSettings* GetUIManagerModuleSettings() override { return UIManagerModuleSettingsDA; }
	
private :

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	URZ_GameSettings* GameSettingsDA;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	URZ_ItemActorModuleSettings* ItemActorModuleSettingsDA;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	URZ_ItemManagerModuleSettings* ItemManagerModuleSettingsDA;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	URZ_ItemUIModuleSettings* ItemUIModuleSettingsDA;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	URZ_UIManagerModuleSettings* UIManagerModuleSettingsDA;
};
