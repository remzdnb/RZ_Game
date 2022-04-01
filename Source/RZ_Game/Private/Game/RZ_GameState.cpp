/// RemzDNB

#include "Game/RZ_GameState.h"
#include "Game/RZ_GameInstance.h"
#include "Game/RZ_GameSettings.h"
#include "Player/RZ_PlayerController.h"

ARZ_GameState::ARZ_GameState()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ARZ_GameState::BeginPlay()
{
	Super::BeginPlay();

	GameSettings = Cast<URZ_GameInstance>(GetGameInstance())->GetGameSettings();
}

void ARZ_GameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Debug(DeltaTime);
}

void ARZ_GameState::ReportPawnBeginPlay(APawn* NewPawn)
{
	AlivePawns.Add(NewPawn);
}

void ARZ_GameState::ReportPawnEndPlay(APawn* InPawn)
{
	AlivePawns.Remove(InPawn);
}

void ARZ_GameState::ReportPawnDamage(const FRZ_DamageInfo& DamageInfo)
{
	ARZ_PlayerController* InstigatorPlayerController = Cast<ARZ_PlayerController>(DamageInfo.InstigatorController);
	if (InstigatorPlayerController)
	{
		InstigatorPlayerController->OnDamageDealt_Client(DamageInfo.Amount, DamageInfo.Location);
	}
}

TArray<APawn*> ARZ_GameState::GetAlivePawns() const
{
	TArray<APawn*> PawnArray;
	for (const auto& Pawn : AlivePawns)
	{
		PawnArray.Add(Pawn.Get());
	}

	return PawnArray;
}

void ARZ_GameState::Debug(float DeltaTime)
{
	if (!GameSettings) { return; }
	if (!GameSettings->bDebugGameState) { return; }

	const FString PrefixString = "ARZ_GameState /// AlivePawns : ";
	
	FString AlivePawnsString;
	for (const auto& AlivePawn : AlivePawns)
	{
		AlivePawnsString = AlivePawnsString + AlivePawn->GetName() + " / ";
	}

	const FString DebugString = PrefixString + AlivePawnsString;

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, DebugString);
}
