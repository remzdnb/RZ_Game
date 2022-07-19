/// RemzDNB
///
/// RZ_PowerComponent.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_BaseFramework.h"
#include "RZ_PowerSystem.h"
#include "RZ_PowerComponentInterface.h"
#include "Components/ActorComponent.h"
#include "RZ_PowerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPowerComponentUpdated);

class UBoxComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RZ_POWERSYSTEM_API URZ_PowerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	URZ_PowerComponent();
	
	virtual void BeginPlay() override;
	
	//
	
	FPowerComponentUpdated OnPowerComponentUpdated;

	//

	const FRZ_PowerComponentSettings& GetPowerComponentSettings() const { return PowerCompSettings; }
	TArray<URZ_PowerComponent*> GetConnectedPowerComps() const { return ConnectedPowerComps; };
	FORCEINLINE FVector GetPoweredAreaSize() const { return SpawnSize + PowerCompSettings.PowerRange; }
	
	//
	
	FORCEINLINE int32 GetPowerGridID() const { return PowerGridID; }
	FORCEINLINE void SetPowerGridID(int32 NewPowerGridID)
	{
		if (PowerGridID != NewPowerGridID)
		{
			PowerGridID = NewPowerGridID;
			OnPowerComponentUpdated.Broadcast();
		}
	}
	FORCEINLINE bool GetIsPowered() const { return bIsPowered;}
	FORCEINLINE void SetIsPowered(bool bNewIsPowered)
	{
		if (bIsPowered != bNewIsPowered)
		{
			bIsPowered = bNewIsPowered;
			Cast<IRZ_PowerComponentInterface>(GetOwner())->OnPowerStatusUpdated(bIsPowered);
			OnPowerComponentUpdated.Broadcast();
		}
	}
	FORCEINLINE bool GetIsDisabled() const { return bIsDisabled; }
	FORCEINLINE void SetIsDisabled(bool bNewIsDisabled)
	{
		if (bIsDisabled != bNewIsDisabled)
		{
			bIsDisabled = bNewIsDisabled;
			OnPowerComponentUpdated.Broadcast();
		}
	}

	FORCEINLINE ARZ_PowerManager* GetPowerManager() const { return PowerManager; }
	
	//
	
	UFUNCTION()
	void UpdateConnectedActors();

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRZ_PowerComponentSettings PowerCompSettings;

	//

	const URZ_PowerSystemSettings* PowerSystemSettings;
	ARZ_PowerManager* PowerManager;
	
	//

	UPROPERTY()
	UBoxComponent* CollisionComp;
	
	UPROPERTY() // -1 == Not part of any grid.
	int32 PowerGridID;

	UPROPERTY()
	bool bIsPowered;

	UPROPERTY()
	bool bIsDisabled;

	UPROPERTY() // Components inside PoweredArea.
	TArray<URZ_PowerComponent*> ConnectedPowerComps;

	UPROPERTY() // ?
	FVector SpawnSize;

	//

	UFUNCTION()
	void OnCollisionCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                 const FHitResult& SweepResult);

	UFUNCTION()
	void OnCollisionCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
 