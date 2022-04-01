#pragma once

#include "CoreMinimal.h"
#include "RZ_CharacterAnimTypes.generated.h"

UENUM(BlueprintType)
enum class ERZ_LowerBodyAnimStance : uint8
{
	Walk,
	Run,
	Crouch
};

UENUM(BlueprintType)
enum class ERZ_UpperBodyAnimStance : uint8
{
	Pistol,
	Rifle,
	Sword
};

USTRUCT(BlueprintType)
struct RZM_CHARACTERANIM_API FRZ_CharacterAnimData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ERZ_LowerBodyAnimStance LowerBodyAnimStance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ERZ_UpperBodyAnimStance UpperBodyAnimStance;

	FRZ_CharacterAnimData()
	{
		LowerBodyAnimStance = ERZ_LowerBodyAnimStance::Walk;
		UpperBodyAnimStance = ERZ_UpperBodyAnimStance::Pistol;
	}
};
