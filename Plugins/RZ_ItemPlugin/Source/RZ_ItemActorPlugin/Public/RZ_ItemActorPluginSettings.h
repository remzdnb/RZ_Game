// RemzDNB

#pragma once

#include "RZ_ItemActorPluginTypes.h"
//
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RZ_ItemActorPluginSettings.generated.h"

UCLASS()
class RZ_ITEMACTORPLUGIN_API URZ_ItemActorPluginSettings : public UDataAsset
{
	GENERATED_BODY()

public:

	URZ_ItemActorPluginSettings();
	
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	TArray<FRZ_ItemData*> GetItemDataArray() const;

	const FRZ_ItemData* const GetItemDataFromRow(FName RowName) const;
	const FRZ_ProjectileWeaponData* const GetProjectileWeaponDataFromRow(FName RowName) const;
	const FRZ_MeleeWeaponData* const GetMeleeWeaponDataFromRow(FName RowName) const;

	// ?? Pas mal
	TArray<FRZ_ItemData*> ItemDataArray;
	TArray<FRZ_ItemData*> WeaponDataArray;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* ItemDT;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* ProjectileWeaponDT;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDataTable* MeleeWeaponDT;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bDebugProjectileWeapon;
};
