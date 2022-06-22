/// RemzDNB
///
/// RZ_PowerComponent.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_PowerSystem.h"
#include "Components/ActorComponent.h"
#include "RZ_PowerComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RZM_POWERSYSTEM_API URZ_PowerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	URZ_PowerComponent();
	
	virtual void BeginPlay() override;
	
	//

	const FRZ_PowerComponentSettings& GetPowerComponentSettings() const { return PowerCompSettings; }
	TArray<URZ_PowerComponent*> GetComponentsInRange() const;

	//

	ARZ_PowerManager* PowerManager;

	//

	UPROPERTY() // 0 == Not connected to any grid
	uint8 PowerGridID;

	UPROPERTY()
	bool bIsPowered;

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRZ_PowerComponentSettings PowerCompSettings;
};
