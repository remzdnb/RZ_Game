// RemzDNB

#pragma once

#include "Components/SkeletalMeshComponent.h"
#include "RZM_WeaponSystem.h"
#include "RZ_TurretComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RZM_WEAPONSYSTEM_API URZ_TurretComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:

	URZ_TurretComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
};
