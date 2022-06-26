// RemzDNB

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RZ_InventoryManager.generated.h"

UCLASS()
class RZM_INVENTORYSYSTEM_API ARZ_InventoryManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARZ_InventoryManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
