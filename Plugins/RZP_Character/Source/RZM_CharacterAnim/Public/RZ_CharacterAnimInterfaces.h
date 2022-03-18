/// RemzDNB
///
///	RZ_AnimPluginInterfaces.cpp
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_CharacterAnimTypes.h"
///
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RZ_CharacterAnimInterfaces.generated.h"

/// Load anim data from character.
/// Implemented in character owner class.

UINTERFACE(MinimalAPI)
class URZ_CharacterAnimInterface : public UInterface
{
	GENERATED_BODY()
};

class RZM_CHARACTERANIM_API IRZ_CharacterAnimInterface
{
	GENERATED_BODY()

public:
	
	virtual const FRZ_CharacterAnimData& GetCharacterAnimData() = 0;

};
