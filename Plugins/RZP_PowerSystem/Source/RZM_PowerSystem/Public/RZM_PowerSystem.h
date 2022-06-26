/// RemzDNB
///
/// RZM_PowerSystem.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

#pragma once

#include "CoreMinimal.h"
#include "RZM_PowerSystem.generated.h"

class ARZ_PowerManager;
class URZ_PowerComponent;
class URZ_PowerWidget;

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FRZM_PowerSystemModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct RZM_POWERSYSTEM_API FRZ_PowerComponentSettings
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsProducingPower;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxProducedPower;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsConsumingPower;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxConsumedPower;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PowerRange;
	
	FRZ_PowerComponentSettings()
	{
		bIsProducingPower = false;
		MaxProducedPower = 100.0f;
		bIsConsumingPower = false;
		MaxConsumedPower = 10.0f;
		PowerRange = 1000.0f;
	}
};

USTRUCT(BlueprintType)
struct RZM_POWERSYSTEM_API FRZ_PowerGridInfo
{
	GENERATED_USTRUCT_BODY()
	
public:
	
	FRZ_PowerGridInfo();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 GridID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ProducedPower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ConsumedPower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<URZ_PowerComponent*> AttachedPowerComponents;

	void UpdateTotalGridPower();
	
	/*FRZ_PowerGridInfo()
	{
		GridID = 0;
		ProducedPower = 0.0f;
		ConsumedPower = 0.0f;
	}*/
};

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UINTERFACE(MinimalAPI)
class URZ_PowerUserInterface : public UInterface
{
	GENERATED_BODY()
};

class RZM_POWERSYSTEM_API IRZ_PowerUserInterface
{
	GENERATED_BODY()

public:

	// Allow PowerComponents to easily identify each others from any Actor class.
	virtual URZ_PowerComponent* GetPowerComponent() = 0;
};
