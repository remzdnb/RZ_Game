#pragma once

#include "CoreMinimal.h"
//#include "RZ_AttributeSystem.generated.h"

class FRZ_AttributeSystemModule : public IModuleInterface
{
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class URZ_AttributeSet;
class URZ_GameplayAbility;