///// RZ_Consumable.h - RemzDNB
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Item.h"
//
#include "CoreMinimal.h"
#include "RZ_Consumable.generated.h"

UCLASS()
class RZM_ITEMACTOR_API ARZ_Consumable : public ARZ_Item
{
	GENERATED_BODY()

public:

	ARZ_Consumable();

protected:

	virtual void BeginPlay() override;

/////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
};