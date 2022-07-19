#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RZ_AttributeSystem.generated.h"

class URZ_AttributeComponent;

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FRZ_AttributeSystemModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

USTRUCT()
struct RZ_ATTRIBUTESYSTEM_API FRZ_AttributeComponentSettings : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxArmor;
	
	FRZ_AttributeComponentSettings()
	{
		MaxHealth = 1000.0f;
		MaxArmor = 1000.0f;	
	}
};

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UINTERFACE(MinimalAPI)
class URZ_AttributeComponentOwnerInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_ATTRIBUTESYSTEM_API IRZ_AttributeComponentOwnerInterface
{
	GENERATED_BODY()

public:
	
	virtual URZ_AttributeComponent* GetAttributeComponent() const = 0;
};

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class RZ_ATTRIBUTESYSTEM_API URZ_AttributeSystemSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	
	URZ_AttributeSystemSettings();

	//
};

UINTERFACE(MinimalAPI)
class URZ_AttributeSystemInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_ATTRIBUTESYSTEM_API IRZ_AttributeSystemInterface
{
	GENERATED_BODY()

	// Used to load editor data from a single DataAsset reference. Must be implemented in projects GameInstance.

public:
	
	virtual const URZ_AttributeSystemSettings* GetCombatSystemSettings() const = 0;
};