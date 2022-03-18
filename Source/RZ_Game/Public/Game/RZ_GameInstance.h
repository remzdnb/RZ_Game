/// RemzDNB
///
///	RZ_GameInstance.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_ItemActorPluginInterfaces.h"
#include "RZ_ItemManagerPluginInterfaces.h"
#include "RZ_UIManagerPluginInterfaces.h"
///
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RZ_GameInstance.generated.h"

class URZ_GameSettings;
class URZ_ItemActorPluginSettings;
class URZ_ItemManagerPluginSettings;
class URZ_UIManagerPluginSettings;

UCLASS()
class RZ_GAME_API URZ_GameInstance : public UGameInstance,
                                     public IRZ_ItemActorPluginSettingsInterface,
                                     public IRZ_ItemManagerPluginSettingsInterface,
                                     public IRZ_UIManagerPluginSettingsInterface
{
	GENERATED_BODY()
	
public:

	virtual URZ_GameSettings* GetGameSettings() const { return GameSettingsDA; }
	virtual URZ_ItemActorPluginSettings* GetItemActorPluginSettings() override { return ItemActorPluginSettingsDA; }
	virtual URZ_ItemManagerPluginSettings* GetItemManagerPluginSettings() override { return ItemManagerPluginSettingsDA; }
	virtual URZ_UIManagerPluginSettings* GetUIManagerPluginSettings() override { return UIManagerPluginSettingsDA; }
	
private :

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	URZ_GameSettings* GameSettingsDA;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	URZ_ItemActorPluginSettings* ItemActorPluginSettingsDA;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	class URZ_ItemManagerPluginSettings* ItemManagerPluginSettingsDA;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	class URZ_UIManagerPluginSettings* UIManagerPluginSettingsDA;
};
