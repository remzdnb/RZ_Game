/// RemzDNB
///
///	RZ_SweepTraceWeapon.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "RZ_Weapon.h"
#include "RZ_SweepTraceWeapon.generated.h"


UCLASS()
class RZM_WEAPONSYSTEM_API ARZ_SweepTraceWeapon : public ARZ_Weapon
{
	GENERATED_BODY()

public:

	ARZ_SweepTraceWeapon();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//

	//virtual void SetWantToUse(bool bNewWantToUse) override;

private:

	UFUNCTION()
	void EnableSweep();

	UFUNCTION()
	void DisableSweep();
	
	UFUNCTION()
	void FireOnce();

	//

	bool bIsSweepEnabled;
	FTimerHandle SweepTraceTimer;
	float SweepMaxDuration;

	TArray<TWeakObjectPtr<AActor>> DamagedActors;

};
