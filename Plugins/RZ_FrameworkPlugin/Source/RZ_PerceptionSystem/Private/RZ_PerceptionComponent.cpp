/// RemzDNB

#include "RZ_PerceptionComponent.h"
//
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"

URZ_PerceptionComponent::URZ_PerceptionComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	
	VisionDistance = 20000.0f;
	VisionAngle = 180.0f;
}

void URZ_PerceptionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ARZ_PerceptionManager> NewPerceptionManager(GetWorld()); NewPerceptionManager; ++NewPerceptionManager)
	{
		PerceptionManager = *NewPerceptionManager;
	}

	if (!PerceptionManager)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		PerceptionManager = GetWorld()->SpawnActor<ARZ_PerceptionManager>(
			ARZ_PerceptionManager::StaticClass(),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParameters
		);
	}
}

void URZ_PerceptionComponent::UpdateSensedActors()
{
	if (!GetOwner()) { return; }
	if (!PerceptionManager) { return; }
	
	SensedActors.Empty();
	
	for (const auto& Pawn : PerceptionManager->GetRegisteredPawns())
	{
		if (Pawn == GetOwner()) { continue; }
		/** ThisCombatComp =
			Cast<URZ_AttributeComponent>(Pawn->GetComponentByClass(URZ_AttributeComponent::StaticClass()));
		if (!ThisCombatComp) { continue; }

		if (ThisCombatComp->GetTeamID() == OwnerCombatComp->GetTeamID()) { continue; }*/
		
		if (FVector::Dist(GetOwner()->GetActorLocation(), Pawn->GetActorLocation()) > VisionDistance)
			continue;

		// ToDo : check rotation

		const FVector Start = GetOwner()->GetActorLocation();
		const FVector End = Pawn->GetActorLocation();
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(GetOwner());
		TArray<FHitResult> HitResults;
		bool bIsPawnSensed = false;
		
		GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, ECC_GameTraceChannel5, TraceParams); // ECC_GameTraceChannel5 = PawnSight
		for (const auto& HitResult : HitResults)
		{
			if (HitResult.Actor == Pawn)
			{
				bIsPawnSensed = true;
				SensedActors.Add(Pawn);
				break;
			}
		}

		if (true)
		{
			if (bIsPawnSensed)
				DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, .3f, 0, 3.0f);
			else
				DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, .3f, 0, 3.0f);
		}
	}

	//Debug();
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
	//if (false) { return; }
	
	const FString PrefixString = GetOwner()->GetName() + " - " + this->GetName() + " /// Sensed Actors : ";
	FString SensedActorsString;

	for (const auto& SensedActor : SensedActors)
	{
		SensedActorsString = SensedActorsString + SensedActor->GetName() + " / ";
	}

	FString DebugString = PrefixString + SensedActorsString;

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, DebugString);
}

