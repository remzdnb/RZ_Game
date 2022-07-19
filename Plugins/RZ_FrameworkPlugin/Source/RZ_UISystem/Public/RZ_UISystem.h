#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Modules/ModuleManager.h"
#include "RZ_UISystem.generated.h"

class FRZ_UISystemModule : public IModuleInterface
{
	
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

UCLASS()
class RZ_UISYSTEM_API URZ_UISystemSettings : public UDataAsset
{
	GENERATED_BODY()

public:

	URZ_UISystemSettings();

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



UINTERFACE(MinimalAPI)
class URZ_UISystemPluginSettingsInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_UISYSTEM_API IRZ_UISystemPluginSettingsInterface
{
	GENERATED_BODY()

public:

	// Used to load editor data from a single DataAsset reference. Must be implemented in projects GameInstance.

	virtual const URZ_UISystemSettings* GetUIManagerModuleSettings() const = 0;
};