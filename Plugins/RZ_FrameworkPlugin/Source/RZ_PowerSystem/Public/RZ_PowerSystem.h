/// RemzDNB
///
/// RZ_PowerSystem.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

#pragma once

#include "CoreMinimal.h"
#include "RZ_PowerSystem.generated.h"

#define POWERSYSTEM_GRIDSIZE 100.0f;

class ARZ_PowerManager;
class URZ_PowerComponent;

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FRZ_PowerSystemModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

/// Module settings.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class RZ_POWERSYSTEM_API URZ_PowerSystemSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	
	URZ_PowerSystemSettings() {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bDebugPowerManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bDebugPowerComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> PowerComponentTextWidgetClass;
};

UINTERFACE(MinimalAPI)
class URZ_PowerSystemSettingsInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_POWERSYSTEM_API IRZ_PowerSystemSettingsInterface
{
	GENERATED_BODY()

	// Implemented in GameInstance.

public:
	
	virtual const URZ_PowerSystemSettings* GetPowerSystemSettings() const = 0;
};


///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct RZ_POWERSYSTEM_API FRZ_PowerComponentSettings
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PowerDelta;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PowerRange;
	
	FRZ_PowerComponentSettings()
	{
		PowerDelta = 0.0f;
		PowerRange = 1000.0f;
	}
};

USTRUCT(BlueprintType)
struct RZ_POWERSYSTEM_API FRZ_PowerGridInfo
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 GridID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ProducedPower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ConsumedPower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<URZ_PowerComponent*> AttachedPowerComponents;
	
	FRZ_PowerGridInfo()
	{
		GridID = -1;
		ProducedPower = 0.0f;
		ConsumedPower = 0.0f;
	}
};

