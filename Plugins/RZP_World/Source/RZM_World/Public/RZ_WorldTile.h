#pragma once

#include "GameFramework/Actor.h"
#include "RZ_WorldTile.generated.h"

UCLASS()
class RZM_WORLD_API ARZ_WorldTile : public AActor
{
	GENERATED_BODY()

public:

	ARZ_WorldTile();

	/*void Init(const FRZ_WorldTileData& NewTileData);
	
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	
	//
	
	UFUNCTION() class UBoxComponent* GetCollisionBox() const { return CollisionCT; }
	UFUNCTION() const FRZ_WorldTileData& GetTileData() const { return TileData; }
	
	UFUNCTION() void SetOwningTeam(ESS_Team Team);

private:

	USS_GameInstance* GInstance;

	//
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* OwnershipMeshCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* SouthBorderParticleCT;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* NorthBorderParticleCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* WestBorderParticleCT;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* EastBorderParticleCT;

	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRZ_WorldTileData TileData;
	
	UPROPERTY()
	ESS_Team OwningTeam;
	
	///// Pawn
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UFUNCTION(BlueprintNativeEvent)
	void RegisterPawnBPI(ASS_Pawn* PawnToRegister);

	UFUNCTION(BlueprintNativeEvent)
	void UnregisterPawnBPI();

	//

	FORCEINLINE UFUNCTION() TWeakObjectPtr<ASS_Pawn> GetRegisteredPawn() const { return RegisteredPawn; }
	
private:

	TWeakObjectPtr<ASS_Pawn> RegisteredPawn;

	///// Rendering
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:

	UFUNCTION()
	void EnableHighlight(const FLinearColor& Color);
	
	UFUNCTION()
	void EnableDirectionalHighlight(ESS_Direction Direction, const FLinearColor& Color, bool bSaveColor = false);
	
	UFUNCTION()
	void DisableHighlight(bool bResetToDefault = false);

private:

	UPROPERTY()
	UMaterialInstanceDynamic* BaseDMaterial;
	
	FLinearColor DefaultSouthBorderColor;
	FLinearColor DefaultNorthBorderColor;
	FLinearColor DefaultWestBorderColor;
	FLinearColor DefaultEastBorderColor;
	FLinearColor SavedSouthBorderColor;
	FLinearColor SavedNorthBorderColor;
	FLinearColor SavedWestBorderColor;
	FLinearColor SavedEastBorderColor;*/
};