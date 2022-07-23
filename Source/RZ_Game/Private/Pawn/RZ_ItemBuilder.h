// RemzDNB

#pragma once

#include "CoreMinimal.h"
#include "Pawn/RZ_Pawn.h"
#include "RZ_ItemBuilder.generated.h"

UCLASS()
class ARZ_ItemBuilder : public ARZ_Pawn
{
	GENERATED_BODY()

public:

	ARZ_ItemBuilder();

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName ItemToBuildName;

	//

	UFUNCTION()
	void BuildItem();
};
