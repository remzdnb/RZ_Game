/// RemzDNB

#pragma once

#include "CoreMinimal.h"
#include "Components/ChildActorComponent.h"
#include "RZ_AttachedSlotComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RZM_INVENTORYSYSTEM_API URZ_AttachedSlotComponent : public UChildActorComponent
{
	GENERATED_BODY()

public:

	URZ_AttachedSlotComponent();

	virtual void InitializeComponent() override;

	//

	FORCEINLINE const FName& GetDefaultActorTableRowName() const { return DefaultActorTableRowName; }

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName DefaultActorTableRowName;

	//
	
	UPROPERTY()
	AActor* AttachedActor;
};
