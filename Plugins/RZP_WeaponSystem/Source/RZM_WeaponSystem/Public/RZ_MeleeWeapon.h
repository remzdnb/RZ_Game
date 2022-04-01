///// RZ_MeleeWeapon.h - RemzDNB
/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_WeaponSystem.h"
#include "RZ_Weapon.h"
//
#include "CoreMinimal.h"
#include "RZ_MeleeWeapon.generated.h"

UCLASS()
class RZM_WEAPONSYSTEM_API ARZ_MeleeWeapon : public ARZ_Weapon
{
	GENERATED_BODY()

public:

	ARZ_MeleeWeapon();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:

	const FRZ_MeleeWeaponSettings* MeleeWeaponData;

	//

	UFUNCTION()
	void AttackOnce();

};
