#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "RZM_ItemUI.generated.h"

#pragma region +++++ Module ...
///
/// Module setup
///


class FRZM_ItemUIModule : public IModuleInterface
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
class RZM_ITEMUI_API URZ_ItemUIModuleSettings : public UDataAsset
{
	GENERATED_BODY()

public:

	URZ_ItemUIModuleSettings();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> ItemSlotWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> ItemActorMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> ItemActorHUDWidgetClass;
};

#pragma endregion

#pragma region +++++ Interfaces ...
/// 
///	Module interfaces
///

// Allows to load editor data from a single DataAsset reference, from any module class.
// Must be implemented in projects GameInstance.

UINTERFACE(MinimalAPI)
class URZ_ItemUIModuleInterface : public UInterface
{
	GENERATED_BODY()
};

class RZM_ITEMUI_API IRZ_ItemUIModuleInterface
{
	GENERATED_BODY()

public:

	// Used to load editor data from a single DataAsset reference. Must be implemented in projects GameInstance.
	virtual URZ_ItemUIModuleSettings* GetItemUIModuleSettings() = 0;
};

#pragma endregion
