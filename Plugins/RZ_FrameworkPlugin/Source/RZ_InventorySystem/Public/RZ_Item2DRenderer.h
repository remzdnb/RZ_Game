// RemzDNB

#pragma once

#include "RZ_BaseFramework.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "RZ_Item2DRenderer.generated.h"

UCLASS()
class RZ_INVENTORYSYSTEM_API ARZ_Item2DRenderer : public AActor
{
	GENERATED_BODY()
	
public:	

	ARZ_Item2DRenderer();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//

	UFUNCTION(BlueprintCallable)
	void SpawnNewActor(const FName& ActorName);

	UFUNCTION(BlueprintCallable)
	void StopActorRotation();

	UFUNCTION(BlueprintCallable)
	void SetSpawnedActorRelativeLocation(const FVector& NewLocation);
	
	UFUNCTION(BlueprintCallable)
	void SetActorLocalRotation(const FRotator& NewRotation);
	

private:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootSceneComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* SceneCaptureComp;
	
	//

	UPROPERTY()
	AActor* SpawnedActor;

	UPROPERTY()
	bool bIsAutoRotating;

};
