/// RemzDNB
///
///	RZ_GameState.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
//
#include "GameFramework/GameStateBase.h"
#include "RZ_GameState.generated.h"

UCLASS()
class RZ_GAME_API ARZ_GameState : public AGameStateBase

{
	GENERATED_BODY()

public:

	ARZ_GameState();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//

	UFUNCTION()
	void ReportPawnBeginPlay(APawn* NewPawn);

	UFUNCTION()
	void ReportPawnEndPlay(APawn* InPawn);

	/*UFUNCTION() // Server only
	void ReportPawnDamage(const FRZ_DamageInfo& DamageInfo);*/
	
	//

	FORCEINLINE TArray<APawn*> GetAlivePawns() const;
	FORCEINLINE URZ_GameSettings* GetGameSettings() const { return GameSettings; }

private:

	UFUNCTION()
	void Debug(float DeltaTime);

	//

	URZ_GameSettings* GameSettings;

	UPROPERTY()
	TArray<TWeakObjectPtr<APawn>> AlivePawns;
};
