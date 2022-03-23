#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Modules/ModuleManager.h"
#include "RZM_UIManager.generated.h"

#pragma region +++++ Module ...
///
/// Module setup
///

class FRZM_UIManagerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

#pragma endregion

#pragma region +++++ Settings ...
/// 
///	Module settings
///

UCLASS()
class RZM_UIMANAGER_API URZ_UIManagerModuleSettings : public UDataAsset
{
	GENERATED_BODY()

public:

	URZ_UIManagerModuleSettings();

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class URZ_MenuLayoutWidget> MenuLayoutWBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> MenuLayoutButtonWBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> ExitWBP;
	
	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDebugUIManager;
};

#pragma endregion

#pragma region +++++ Interfaces ...
/// 
///	Module interfaces
///

/// Used to load editor data from a single DataAsset reference.
/// Must be implemented in project BaseGameState.

UINTERFACE(MinimalAPI)
class URZ_UIManagerPluginSettingsInterface : public UInterface
{
	GENERATED_BODY()
};

class RZM_UIMANAGER_API IRZ_UIManagerPluginSettingsInterface
{
	GENERATED_BODY()

public:

	virtual class URZ_UIManagerModuleSettings* GetUIManagerModuleSettings() = 0;
};

#pragma endregion