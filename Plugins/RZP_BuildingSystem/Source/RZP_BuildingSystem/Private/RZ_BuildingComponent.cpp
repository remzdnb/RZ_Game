/// RemzDNB

#include "RZ_BuildingComponent.h"
//
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

URZ_BuildingComponent::URZ_BuildingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	bIsBuilding = false;
	bDebug = false;
}

void URZ_BuildingComponent::BeginPlay()
{
	Super::BeginPlay();

	IRZ_ActorInterface* OwnerActorInterface = Cast<IRZ_ActorInterface>(GetOwner());
	if (OwnerActorInterface)
	{
		BuildSize = OwnerActorInterface->GetActorSettings().NormalizedWorldSize;
	}
}

void URZ_BuildingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* TickFunction)
{
	Super::TickComponent(DeltaTime, TickType, TickFunction);

	if (bIsBuilding)
	{
		UpdateBuildableActorLocation(DeltaTime);
		UpdateBuildableActorCollision();
	}
}

void URZ_BuildingComponent::StartBuilding(AActor* NewBuildableActor)
{
	IRZ_BuildableInterface* BuildableInterface = Cast<IRZ_BuildableInterface>(NewBuildableActor);
	if (!BuildableInterface) { return; }

	BuildableInterface->OnBuildStart();
	BuildableActor = NewBuildableActor;
	bIsBuilding = true;
}

void URZ_BuildingComponent::StopBuilding()
{
	if (!BuildableActor.IsValid()) { return; }
	
	IRZ_BuildableInterface* PawnBuildableInterface = Cast<IRZ_BuildableInterface>(BuildableActor);
	if (!PawnBuildableInterface) { return; }

	PawnBuildableInterface->OnBuildStop();
	BuildableActor.Reset();
	bIsBuilding = false;
}

void URZ_BuildingComponent::EndBuilding()
{
	if (!BuildableActor.IsValid()) { return; }
	
	IRZ_BuildableInterface* PawnBuildableInterface = Cast<IRZ_BuildableInterface>(BuildableActor);
	if (!PawnBuildableInterface) { return; }

	PawnBuildableInterface->OnBuildEnd();
	BuildableActor.Reset();
	bIsBuilding = false;
}

void URZ_BuildingComponent::UpdateBuildableActorLocation(float DeltaTime)
{
	if (!BuildableActor.IsValid()) { return; }

	IRZ_BuildableInterface* PawnBuildableInterface = Cast<IRZ_BuildableInterface>(BuildableActor);
	if (!PawnBuildableInterface) { return; }

	FVector FinalBuildLocation;
	if (PawnBuildableInterface->GetGridSize() % 2 == 0) // Even GridSize
	{
		FinalBuildLocation = UKismetMathLibrary::Vector_SnappedToGrid(PlayerTargetLocation, 100.0f);
	}
	else
	{
		FinalBuildLocation = UKismetMathLibrary::Vector_SnappedToGrid(PlayerTargetLocation, 100.0f);
		FinalBuildLocation.X += 50.0f;
		FinalBuildLocation.Y += 50.0f;
	}
	
	BuildableActor->SetActorLocation(FinalBuildLocation);
	if (FinalBuildLocation != LastBuildActorLocation)
	{
		PawnBuildableInterface->OnBuildLocationUpdated(FinalBuildLocation);
		LastBuildActorLocation = FinalBuildLocation;
	}

	//RZ_UtilityLibrary::PrintVectorToScreen("URZ_BuildingComponent::Debug /// PlayerTargetLocation == ", PlayerTargetLocation, FColor::Red, -1, DeltaTime);
	//RZ_UtilityLibrary::PrintVectorToScreen("URZ_BuildingComponent::Debug /// FinalBuildLocation == ", FinalBuildLocation, FColor::Red, -1, DeltaTime);
}

void URZ_BuildingComponent::UpdateBuildableActorCollision()
{
	if (!BuildableActor.IsValid()) { return; }
	
	TArray<FHitResult> OutHits;
	const FVector SweepStart = GetOwner()->GetActorLocation();
	const FVector SweepEnd = GetOwner()->GetActorLocation();
	const FVector BoxExtent = BuildSize * RZ_GRIDTILESIZE / 2;
	const FCollisionShape CollisionBox = FCollisionShape::MakeBox(BoxExtent);
	
	GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_GameTraceChannel9, CollisionBox); // OverlapAll
	for (const auto& Hit : OutHits)
	{
		if (GetOwner() != Hit.Actor)
		{
			if (bDebug) { UE_LOG(LogTemp, Display, TEXT("URZ_BuildingComponent::UpdateBuildableActorCollision // Actor hit == %s"), *Hit.Actor->GetName()); }

			Cast<IRZ_BuildableInterface>(BuildableActor)->OnInvalidBuildLocation();
			return;
		}

		if (bDebug) { UE_LOG(LogTemp, Display, TEXT("URZ_BuildingComponent::UpdateBuildableActorCollision // Discarded actor hit == %s"), *Hit.Actor->GetName()); }
	}

	Cast<IRZ_BuildableInterface>(BuildableActor)->OnValidBuildLocation();
	
	if (bDebug)
	{
		DrawDebugBox(GetWorld(), GetOwner()->GetActorLocation(), BoxExtent, FColor::Orange, false, 0.5f, 0, 4.0f);
	}
}

void URZ_BuildingComponent::RotateBuildActor(bool bRotateRight) const
{
	if (!BuildableActor.IsValid()) { return; }
	
	float YawToAdd = 45.0f;
	if (bRotateRight) { YawToAdd *= -1; }
	BuildableActor->AddActorLocalRotation(FRotator(0.0f, YawToAdd, 0.0f));
}

bool URZ_BuildingComponent::IsValidBuildLocation()
{
	TArray<AActor*> ActorArray;
	//CollisionBoxCT->GetOverlappingActors(ActorArray);
	
	if (ActorArray.Num() == 0) { return true; }

	return false;
}

