/// RemzDNB
///
///	Reference location where to spawn a character.
///	The capsule collision is used to know if the location is valid.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
//
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RZ_PawnStart.generated.h"

UCLASS()
class RZ_GAME_API ARZ_PawnStart : public AActor
{
	GENERATED_BODY()
	
public:	

	ARZ_PawnStart();

	virtual void OnConstruction(const FTransform& InTransform) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	//

	UFUNCTION(Server, Reliable)
	void UpdateAvailability_Server(bool bNewIsAvailable);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAvailability_BPI(bool bNewIsAvailable);

	FORCEINLINE UFUNCTION(BlueprintCallable) bool GetIsEnabled() const { return bIsEnabled; }
	FORCEINLINE UFUNCTION(BlueprintCallable) bool GetIsAvailable() const { return bIsAvailable; }
	FORCEINLINE UFUNCTION(BlueprintCallable) uint8 GetTeamID() const { return TeamID; }
	//UFUNCTION(BlueprintCallable) ERZ_PawnOwnership GetOwnership() const { return Ownership; }
	
	UFUNCTION(BlueprintCallable) FTransform GetStartTransform() const;

private:

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION()
	void OnRep_IsAvailable();

	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ArrowComp;

	//

	//UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "ARZ_PawnStart")
	//ERZ_PawnOwnership Ownership;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true")) 
	uint8 TeamID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsEnabled;

	UPROPERTY(ReplicatedUsing = OnRep_IsAvailable)
	bool bIsAvailable;

	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> OverlappedPawns;

};
