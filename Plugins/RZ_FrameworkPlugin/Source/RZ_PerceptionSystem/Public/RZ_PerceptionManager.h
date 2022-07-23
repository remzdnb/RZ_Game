/// RemzDNB
///
///	RZ_PerceptionManager.h
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_CommonTags.h"
#include "GameFramework/Actor.h"
#include "RZ_PerceptionManager.generated.h"

UCLASS()
class RZ_PERCEPTIONSYSTEM_API ARZ_PerceptionManager : public AActor
{
	GENERATED_BODY()
	
public:	

	ARZ_PerceptionManager();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	//
	
	FORCEINLINE const TArray<TWeakObjectPtr<APawn>>& GetRegisteredPawns() const { return RegisteredPawns; }

private:

	UPROPERTY()
	TArray<TWeakObjectPtr<APawn>> RegisteredPawns;

	//

	UFUNCTION()
	void Debug(float DeltaTime);
};
