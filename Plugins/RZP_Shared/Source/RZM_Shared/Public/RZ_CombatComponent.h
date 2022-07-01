/// RemzDNB
///
///	RZ_CombatComponent.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Components/ActorComponent.h"
#include "RZ_CombatComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RZM_SHARED_API URZ_CombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	URZ_CombatComponent();

	virtual void BeginPlay() override;

	//

	FORCEINLINE void SetTeamID(uint8 NewTeamID) { TeamID = NewTeamID; }
	FORCEINLINE uint8 GetTeamID() const { return TeamID; }

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	uint8 TeamID;
};
