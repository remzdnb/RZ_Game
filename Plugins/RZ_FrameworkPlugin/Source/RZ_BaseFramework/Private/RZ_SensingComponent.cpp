/// RemzDNB

#include "RZ_SensingComponent.h"
//
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"

URZ_SensingComponent::URZ_SensingComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	
	VisionDistance = 20000.0f;
	VisionAngle = 180.0f;
}

void URZ_SensingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//GameState = Cast<ARZ_GameState>(GetWorld()->GetGameState());
	//GameSettings = Cast<URZ_GameInstance>(GetOwner()->GetGameInstance())->GetGameSettings();
}

void URZ_SensingComponent::UpdateSensedActors()
{
	//if (!GameSettings.IsValid()) { return; }
	if (!GetOwner()) { return; }

	/*URZ_AttributeComponent* OwnerCombatComp =
		Cast<URZ_AttributeComponent>(GetOwner()->GetComponentByClass(URZ_AttributeComponent::StaticClass()));
	if (!OwnerCombatComp) { return; }
	
	SensedActors.Empty();
	
	//for (const auto& Pawn : GameState->GetAlivePawns())
	for (TActorIterator<APawn> Pawn(GetWorld(), APawn::StaticClass()); Pawn; ++Pawn) // Costly ?
	{
		URZ_AttributeComponent* ThisCombatComp =
			Cast<URZ_AttributeComponent>(Pawn->GetComponentByClass(URZ_AttributeComponent::StaticClass()));
		if (!ThisCombatComp) { continue; }

		if (ThisCombatComp->GetTeamID() == OwnerCombatComp->GetTeamID()) { continue; }
		
		//
		
		
		IRZ_ActorInterface* ActorInterface = Cast<IRZ_ActorInterface>(*Pawn);
		
		if (!ActorInterface) { continue; }
		//if (!(ActorInterface->GetActorMode() == ERZ_ActorMode::Visible_Enabled)) { return; }
		
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
				if (HitResult.Actor == *Pawn)
				{
					bIsPawnSensed = true;
					SensedActors.Add(*Pawn);
					break;
				}
			}
			else
			{
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
	}*/

	//Debug();
}

TArray<AActor*> URZ_SensingComponent::GetActorsInRange() const
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

void URZ_SensingComponent::Debug()
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

