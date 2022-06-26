/// RemzDNB
///
/// RZ_PowerComponent.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZM_PowerSystem.h"
#include "RZM_Shared.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "RZ_PowerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPowerComponentUpdated);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RZM_POWERSYSTEM_API URZ_PowerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	URZ_PowerComponent();
	
	virtual void BeginPlay() override;
	
	//

	const FRZ_PowerComponentSettings& GetPowerComponentSettings() const { return PowerCompSettings; }
	TArray<URZ_PowerComponent*> GetConnectedPowerComps() const { return ConnectedPowerComps; };

	//
	
	FORCEINLINE uint8 GetPowerGridID() const { return PowerGridID; }
	FORCEINLINE void SetPowerGridID(uint8 NewPowerGridID)
	{
		PowerGridID = NewPowerGridID;
		OnPowerComponentUpdated.Broadcast();
	}
	FORCEINLINE FVector GetPoweredAreaSize() const { return SpawnSize + PowerCompSettings.PowerRange; }
	
	//
	
	UFUNCTION()
	void UpdateConnectedActors();
	
	//

	UPROPERTY()
	UBoxComponent* CollisionComp;

	FPowerComponentUpdated OnPowerComponentUpdated;

	ARZ_PowerManager* PowerManager;

	//

	UPROPERTY()
	bool bIsPowered;

	UPROPERTY()
	bool bIsDisabled;

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRZ_PowerComponentSettings PowerCompSettings;

	//

	UPROPERTY() // 0 == Not connected to any grid
	uint8 PowerGridID;

	TArray<URZ_PowerComponent*> ConnectedPowerComps;
	FVector SpawnSize;

	//

	UFUNCTION()
	void OnCollisionCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                 const FHitResult& SweepResult);

	UFUNCTION()
	void OnCollisionCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//
	
	bool bDebug;
};
 