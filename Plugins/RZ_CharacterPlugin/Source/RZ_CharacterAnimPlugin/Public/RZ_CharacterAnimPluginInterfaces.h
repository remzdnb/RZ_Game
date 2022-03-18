/// RemzDNB
///
///	RZ_AnimPluginInterfaces.cpp
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_CharacterAnimPluginTypes.h"
///
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RZ_CharacterAnimPluginInterfaces.generated.h"

/// Load anim data from character.
/// Implemented in character owner class.

UINTERFACE(MinimalAPI)
class URZ_CharacterAnimInterface : public UInterface
{
	GENERATED_BODY()
};

class RZ_CHARACTERANIMPLUGIN_API IRZ_CharacterAnimInterface
{
	GENERATED_BODY()

public:
	
	virtual const FRZ_CharacterAnimData& GetCharacterAnimData() = 0;

};
