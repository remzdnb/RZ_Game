/// RemzDNB

#include "AI/RZ_PerceptionComponent.h"
#include "Game/RZ_GameInstance.h"
#include "Game/RZ_GameState.h"
#include "Game/RZ_GameSettings.h"
//
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

URZ_PerceptionComponent::URZ_PerceptionComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	
	VisionDistance = 2000.0f;
	VisionAngle = 180.0f;
}

void URZ_PerceptionComponent::BeginPlay()
{
	Super::BeginPlay();

	GameState = Cast<ARZ_GameState>(GetWorld()->GetGameState());
	GameSettings = Cast<URZ_GameInstance>(GetOwner()->GetGameInstance())->GetGameSettings();
}

void URZ_PerceptionComponent::UpdateSensedActors()
{
	if (!GameSettings.IsValid()) { return; }
	if (!GetOwner()) { return; }

	SensedActors.Empty();
	
	for (const auto& Pawn : GameState->GetAlivePawns())
	{
		if (!Pawn) { break; }
		
		if (FVector::Dist(GetOwner()->GetActorLocation(), Pawn->GetActorLocation()) > VisionDistance)
			break;

		// ToDo : check rotation

		const FVector Start = FVector(
			GetOwner()->GetActorLocation().X,
			GetOwner()->GetActorLocation().Y,
			BASEVIEWHEIGHT
		);
		const FVector End = FVector(
			Pawn->GetActorLocation().X,
			Pawn->GetActorLocation().Y,
			BASEVIEWHEIGHT
		);
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(GetOwner());
		TArray<FHitResult> HitResults;
		bool bIsPawnSensed = false;
		
		GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECC_GameTraceChannel5, TraceParams); // ECC_GameTraceChannel5 = PawnSight
		for (const auto& HitResult : HitResults)
		{
			if (Cast<APawn>(HitResult.Actor))//HitResult.Actor == Pawn
			{
				if (HitResult.Actor == Pawn)
				{
					bIsPawnSensed = true;
					SensedActors.Add(Pawn);
					break;
				}
			}
			else
			{
				break;
			}
		}

		if (GameSettings->bDebugPerceptionComponent)
		{
			if (bIsPawnSensed)
				DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, .3f, 0, 3.0f);
			else
				DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, .3f, 0, 3.0f);
		}
	}

	Debug();
}

TArray<AActor*> URZ_PerceptionComponent::GetActorsInRange() const
{
	TArray<FHitResult> SphereHits;
	
	FVector SweepStart = GetOwner()->GetActorLocation();
	FVector SweepEnd = GetOwner()->GetActorLocation();
	
	const FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(500.0f);
	
	DrawDebugSphere(GetWorld(), SweepStart, VisionDistance, 50, FColor::Purple, true);

	TArray<AActor*> ActorsInRange;
	GetWorld()->SweepMultiByChannel(SphereHits, SweepStart, SweepEnd, FQuat::Identity, ECC_GameTraceChannel8, CollisionSphere);
	for (const auto& Hit : SphereHits)
	{
		ActorsInRange.Add(Hit.Actor.Get());
		if (GEngine)
		{
			// screen log information on what was hit
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
			                                 FString::Printf(TEXT("Hit Result: %s"), *Hit.Actor->GetName()));
			// uncommnet to see more info on sweeped actor
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("All Hit Information: %s"), *Hit.ToString()));
		}
	}

	return ActorsInRange;
}

void URZ_PerceptionComponent::Debug()
{
	if (!GameSettings->bDebugPerceptionComponent) { return; }
	
	const FString PrefixString = GetOwner()->GetName() + " - " + this->GetName() + " /// Sensed Actors : ";
	FString SensedActorsString;

	for (const auto& SensedActor : SensedActors)
	{
		SensedActorsString = SensedActorsString + SensedActor->GetName() + " / ";
	}

	FString DebugString = PrefixString + SensedActorsString;

	GEngine->AddOnScreenDebugMessage(-1, .3f, FColor::Orange, DebugString);
}

