/// RemzDNB
///
///	RZ_GameInstance.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_ItemActor.h"
#include "RZ_ItemManagerComponent.h"
#include "RZ_UIManagerPluginInterfaces.h"
///
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RZ_GameInstance.generated.h"

class URZ_GameSettings;
class URZ_ItemActorEditorSettings;
class URZ_ItemManagerEditorSettings;
class URZ_UIManagerPluginSettings;

UCLASS()
class RZ_GAME_API URZ_GameInstance : public UGameInstance,
                                     public IRZ_ItemActorEditorSettingsInterface,
                                     public IRZ_ItemManagerEditorSettingsInterface,
                                     public IRZ_UIManagerPluginSettingsInterface
{
	GENERATED_BODY()
	
public:

	virtual URZ_GameSettings* GetGameSettings() const { return GameSettingsDA; }
	virtual URZ_ItemActorEditorSettings* GetItemActorEditorSettings() override { return ItemActorModuleSettingsDA; }
	virtual URZ_ItemManagerEditorSettings* GetItemManagerEditorSettings() override { return ItemManagerPluginSettingsDA; }
	virtual URZ_UIManagerPluginSettings* GetUIManagerPluginSettings() override { return UIManagerPluginSettingsDA; }
	
private :

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	URZ_GameSettings* GameSettingsDA;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	URZ_ItemActorEditorSettings* ItemActorModuleSettingsDA;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	URZ_ItemManagerEditorSettings* ItemManagerPluginSettingsDA;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "URZ_GameInstance", meta = (AllowPrivateAccess = true))
	URZ_UIManagerPluginSettings* UIManagerPluginSettingsDA;
};
