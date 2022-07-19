// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "UObject/Interface.h"
#include "RZ_AnimationSystem.generated.h"

class FRZ_AnimationSystemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

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
	Default,
	Pistol,
	Rifle,
	Sword
};

USTRUCT(BlueprintType)
struct RZ_ANIMATIONSYSTEM_API FRZ_CharacterAnimData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator ViewRotation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ERZ_LowerBodyAnimStance LowerBodyAnimStance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ERZ_UpperBodyAnimStance UpperBodyAnimStance;

	FRZ_CharacterAnimData()
	{
		LowerBodyAnimStance = ERZ_LowerBodyAnimStance::Walk;
		UpperBodyAnimStance = ERZ_UpperBodyAnimStance::Default;
	}
};

/// Load anim data from character.
/// Implemented in character owner class.

UINTERFACE(MinimalAPI)
class URZ_CharacterAnimInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_ANIMATIONSYSTEM_API IRZ_CharacterAnimInterface
{
	GENERATED_BODY()

public:
	
	const FRZ_CharacterAnimData& GetCharacterAnimData()  { return AnimData; }
	void SetAnimData(const FRZ_CharacterAnimData& NewAnimData) { AnimData = NewAnimData; }

private:

	FRZ_CharacterAnimData AnimData;

};
