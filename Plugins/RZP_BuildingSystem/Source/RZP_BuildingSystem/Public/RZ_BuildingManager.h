// RemzDNB

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RZ_BuildingManager.generated.h"

UCLASS()
class RZM_BUILDINGSYSTEM_API ARZ_BuildingManager : public AActor
{
	GENERATED_BODY()
	
public:	

	ARZ_BuildingManager();

	virtual void BeginPlay() override;

	//
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_BuildingManager | Settings")
	UMaterialInterface* BuildingMaterial_Valid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ARZ_BuildingManager | Settings")
	UMaterialInterface* BuildingMaterial_Invalid;
};
