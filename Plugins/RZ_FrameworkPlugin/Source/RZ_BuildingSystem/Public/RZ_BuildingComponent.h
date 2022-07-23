/// RemzDNB
///
///	RZ_BuildingComponent.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_BuildingSystem.h"
//
#include "Components/ActorComponent.h"
#include "RZ_BuildingComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RZ_BUILDINGSYSTEM_API URZ_BuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	URZ_BuildingComponent(); // SpawningComponent, SpawningSystem ?
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* TickFunction) override;

	//

	void Init(UStaticMeshComponent* StaticMeshComponent, USkeletalMeshComponent* SkeletalMeshComponent);

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRZ_BuildableActorSettings BuildableActorSettings;

	//

	const URZ_BuildingSystemSettings* BuildingSystemSettings;
	IRZ_BuildableActorInterface* OwnerBuildableActorInterface;
	FHitResult CursorToGroundHitResult;
	
	//

	UPROPERTY()
	bool bIsDemoBuilding;
	
	UPROPERTY()
	bool bIsBuilding;

	UPROPERTY()
	float BuildStartTime;

	UPROPERTY()
	FVector LastBuildActorLocation;

	UPROPERTY()
	UMaterialInstanceDynamic* BuildDynamicMaterial;

	UPROPERTY()
	USkeletalMeshComponent* OwnerSkeletalMeshComp;

	UPROPERTY()
	UStaticMeshComponent* OwnerStaticMeshComp;

public:
	
	UFUNCTION()
	void StartDemoBuild();

	UFUNCTION()
	void StopDemoBuild();
	
	UFUNCTION()
	void StartBuild();

	UFUNCTION()
	void StopBuild();
	
	UFUNCTION()
	void EndBuild();
	
	UFUNCTION()
	void RotateBuildActor(bool bRotateRight) const;

	//

	UFUNCTION()
	virtual bool IsValidBuildLocation();

	UFUNCTION()
	void SetAllMaterials(UMaterialInterface* MaterialInterface);
	
	//
	
	FORCEINLINE bool GetIsBuilding() const { return bIsBuilding; }
	FORCEINLINE float GetBuildStartTime() const { return BuildStartTime; }
	FORCEINLINE const FRZ_BuildableActorSettings& GetBuildableActorSettings() const { return BuildableActorSettings; }

private:
	
	UFUNCTION()
	void UpdateBuildableActorLocation(float DeltaTime);

	UFUNCTION()
	void UpdateBuildableActorCollision(float DeltaTime);

	//

	UFUNCTION()
	void Debug(float DeltaTime);
};
