///// RZ_MeleeWeapon.h - RemzDNB
/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Item.h"
#include "RZ_ItemActorPluginTypes.h"
//
#include "CoreMinimal.h"
#include "RZ_MeleeWeapon.generated.h"

UCLASS()
class RZ_ITEMACTORPLUGIN_API ARZ_MeleeWeapon : public ARZ_Item
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

	const FRZ_MeleeWeaponData* MeleeWeaponData;

	//

	UFUNCTION()
	void AttackOnce();

};
