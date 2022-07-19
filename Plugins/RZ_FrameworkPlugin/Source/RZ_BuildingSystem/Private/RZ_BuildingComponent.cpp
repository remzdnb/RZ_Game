/// RemzDNB

#include "RZ_BuildingComponent.h"
//
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

URZ_BuildingComponent::URZ_BuildingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	bIsDemoBuilding = false;
	bIsBuilding = false;
}

void URZ_BuildingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	BuildableActorInterface = Cast<IRZ_BuildableActorInterface>(GetOwner());
}

void URZ_BuildingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* TickFunction)
{
	Super::TickComponent(DeltaTime, TickType, TickFunction);

	if (bIsBuilding)
	{
		if (GetWorld()->GetTimeSeconds() - BuildingStartTime <= BuildableActorSettings.BuildTime)
		{
			EndBuilding();
		}
	}

	UpdateBuildableActorLocation(DeltaTime);
	UpdateBuildableActorCollision();
}

void URZ_BuildingComponent::StartDemoBuild()
{
	IRZ_BuildableActorInterface* BuildableInterface = Cast<IRZ_BuildableActorInterface>(GetOwner());
	if (!BuildableInterface) { return; }

	bIsDemoBuilding = true;
	BuildableInterface->OnDemoBuildStart();
}

void URZ_BuildingComponent::StopDemoBuild()
{
	IRZ_BuildableActorInterface* BuildableInterface = Cast<IRZ_BuildableActorInterface>(GetOwner());
	if (!BuildableInterface) { return; }

	bIsDemoBuilding = false;
	BuildableInterface->OnDemoBuildStop();
}

void URZ_BuildingComponent::StartBuilding()
{
	if (!BuildableActorInterface) { return; }

	BuildingStartTime = GetWorld()->GetTimeSeconds();

	BuildableActorInterface->OnBuildStart();
	bIsBuilding = true;
}

void URZ_BuildingComponent::StopBuilding()
{
	if (!BuildableActorInterface) { return; }
	
	BuildableActorInterface->OnBuildStop();
	bIsBuilding = false;
}

void URZ_BuildingComponent::EndBuilding()
{
	if (!BuildableActorInterface) { return; }

	BuildableActorInterface->OnBuildEnd();
	bIsBuilding = false;
}

void URZ_BuildingComponent::UpdateBuildableActorLocation(float DeltaTime)
{
	if (!BuildableActorInterface) { return; }

	FVector FinalBuildLocation;
	if (BuildableActorInterface->GetBuildingComponent()->GetBuildableActorSettings().NormalizedBuildSize % 2 == 0) // Even GridSize
	{
		FinalBuildLocation = UKismetMathLibrary::Vector_SnappedToGrid(PlayerTargetLocation, 100.0f);
	}
	else
	{
		FinalBuildLocation = UKismetMathLibrary::Vector_SnappedToGrid(PlayerTargetLocation, 100.0f);
		FinalBuildLocation.X += 50.0f;
		FinalBuildLocation.Y += 50.0f;
	}
	
	GetOwner()->SetActorLocation(FinalBuildLocation);
	if (FinalBuildLocation != LastBuildActorLocation)
	{
		BuildableActorInterface->OnBuildLocationUpdated(FinalBuildLocation);
		LastBuildActorLocation = FinalBuildLocation;
	}

	//RZ_UtilityLibrary::PrintVectorToScreen("URZ_BuildingComponent::Debug /// PlayerTargetLocation == ", PlayerTargetLocation, FColor::Red, -1, DeltaTime);
	//RZ_UtilityLibrary::PrintVectorToScreen("URZ_BuildingComponent::Debug /// FinalBuildLocation == ", FinalBuildLocation, FColor::Red, -1, DeltaTime);
}

void URZ_BuildingComponent::UpdateBuildableActorCollision()
{
	if (!BuildableActorInterface) { return; }
	
	TArray<FHitResult> OutHits;
	const FVector SweepStart = GetOwner()->GetActorLocation();
	const FVector SweepEnd = GetOwner()->GetActorLocation();
	const FVector BoxExtent = FVector(BuildableActorInterface->GetBuildingComponent()->GetBuildableActorSettings().NormalizedBuildSize * RZ_GRIDTILESIZE / 2);
	const FCollisionShape CollisionBox = FCollisionShape::MakeBox(BoxExtent);
	
	GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_GameTraceChannel9, CollisionBox); // OverlapAll
	for (const auto& Hit : OutHits)
	{
		if (GetOwner() != Hit.Actor)
		{
			if (true) { UE_LOG(LogTemp, Display, TEXT("URZ_BuildingComponent::UpdateBuildableActorCollision // Actor hit == %s"), *Hit.Actor->GetName()); }

			Cast<IRZ_BuildableActorInterface>(BuildableActorInterface)->OnInvalidBuildLocation();
			return;
		}

		if (true) { UE_LOG(LogTemp, Display, TEXT("URZ_BuildingComponent::UpdateBuildableActorCollision // Discarded actor hit == %s"), *Hit.Actor->GetName()); }
	}

	BuildableActorInterface->OnValidBuildLocation();
	
	if (true)
	{
		DrawDebugBox(GetWorld(), GetOwner()->GetActorLocation(), BoxExtent, FColor::Orange, false, 0.5f, 0, 4.0f);
	}
}

void URZ_BuildingComponent::RotateBuildActor(bool bRotateRight) const
{
	if (!BuildableActorInterface) { return; }
	
	float YawToAdd = 45.0f;
	if (bRotateRight) { YawToAdd *= -1; }
	//BuildableActorInterface->AddActorLocalRotation(FRotator(0.0f, YawToAdd, 0.0f));
}

bool URZ_BuildingComponent::IsValidBuildLocation()
{
	TArray<AActor*> ActorArray;
	//CollisionBoxCT->GetOverlappingActors(ActorArray);
	
	if (ActorArray.Num() == 0) { return true; }

	return false;
}

