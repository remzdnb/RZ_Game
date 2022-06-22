/// RemzDNB

#include "S2D_BuildComponent.h"
#include "S2D_BuildableInterface.h"
//
#include "RZM_Shared.h"
//
#include "Kismet/KismetMathLibrary.h"

US2D_BuildComponent::US2D_BuildComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void US2D_BuildComponent::BeginPlay()
{
	Super::BeginPlay();
}

void US2D_BuildComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateBuildingPawnLocation(DeltaTime);
}

void US2D_BuildComponent::StartBuilding(AActor* NewBuildableActor)
{
	IS2D_BuildableInterface* PawnBuildableInterface = Cast<IS2D_BuildableInterface>(NewBuildableActor);
	if (!PawnBuildableInterface) { return; }

	PawnBuildableInterface->OnBuildStart();
	BuildableActor = NewBuildableActor;
	
	RZ_UtilityLibrary::PrintStringToScreen("US2D_BuildComponent::StartBuilding", "", FColor::Red, -1, 3.0f);
}

void US2D_BuildComponent::StopBuilding()
{
	if (!BuildableActor.IsValid()) { return; }
	
	IS2D_BuildableInterface* PawnBuildableInterface = Cast<IS2D_BuildableInterface>(BuildableActor);
	if (!PawnBuildableInterface) { return; }

	PawnBuildableInterface->OnBuildStop();
	BuildableActor.Reset();

	RZ_UtilityLibrary::PrintStringToScreen("US2D_BuildComponent::StopBuilding", "", FColor::Red, -1, 3.0f);
}

void US2D_BuildComponent::EndBuilding()
{
	if (!BuildableActor.IsValid()) { return; }
	
	IS2D_BuildableInterface* PawnBuildableInterface = Cast<IS2D_BuildableInterface>(BuildableActor);
	if (!PawnBuildableInterface) { return; }

	PawnBuildableInterface->OnBuildEnd();
	BuildableActor.Reset();


	
	RZ_UtilityLibrary::PrintStringToScreen("US2D_BuildComponent::EndBuilding", "", FColor::Red, -1, 3.0f);
	
}

void US2D_BuildComponent::UpdateBuildingPawnLocation(float DeltaTime) const
{
	if (!BuildableActor.IsValid()) { return; }

	IS2D_BuildableInterface* PawnBuildableInterface = Cast<IS2D_BuildableInterface>(BuildableActor);
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

	RZ_UtilityLibrary::PrintVectorToScreen("US2D_BuildComponent::Debug /// PlayerTargetLocation == ", PlayerTargetLocation, FColor::Red, -1, DeltaTime);
	RZ_UtilityLibrary::PrintVectorToScreen("US2D_BuildComponent::Debug /// FinalBuildLocation == ", FinalBuildLocation, FColor::Red, -1, DeltaTime);
}

void US2D_BuildComponent::RotateBuildActor(bool bRotateRight) const
{
	if (!BuildableActor.IsValid()) { return; }
	
	float YawToAdd = 45.0f;
	if (bRotateRight) { YawToAdd *= -1; }
	BuildableActor->AddActorLocalRotation(FRotator(0.0f, YawToAdd, 0.0f));
}

bool US2D_BuildComponent::IsValidBuildLocation()
{
	TArray<AActor*> ActorArray;
	//CollisionBoxCT->GetOverlappingActors(ActorArray);
	
	if (ActorArray.Num() == 0) { return true; }

	return false;
}

