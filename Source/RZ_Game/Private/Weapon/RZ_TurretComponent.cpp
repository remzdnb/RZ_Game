/// RemzDNB

#include "Weapon/RZ_TurretComponent.h"
#include "RZ_SensingComponent.h"
#include "Kismet/KismetMathLibrary.h"

URZ_TurretComponent::URZ_TurretComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
	
	TimerDelay = 0.5f;
	bIsAIEnabled = false;
}

void URZ_TurretComponent::BeginPlay()
{
	Super::BeginPlay();

	PerceptionComp = NewObject<URZ_SensingComponent>(GetOwner(), FName("PerceptionComp")); // oof
	if (PerceptionComp)
	{
		PerceptionComp->RegisterComponent();
	}
}

void URZ_TurretComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* TickFunc)
{
	Super::TickComponent(DeltaTime, TickType, TickFunc);
	
	if (!bIsAIEnabled) { return; }
	if (!CompToRotate) { return; }
	
	if (PerceptionComp->GetSensedActors().Num() != 0)
	{
		const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
			GetOwner()->GetActorLocation(),
			PerceptionComp->GetSensedActors()[0]->GetActorLocation()
		);
		
		CompToRotate->SetWorldRotation(FRotator(
			0.0f,
			TargetRotation.Yaw - 90.0f,
			0.0f
		));

		OnTargetLocked.Broadcast(true);
	}
	else
	{
		CompToRotate->SetWorldRotation(FRotator(
			0.0f,
			GetOwner()->GetActorRotation().Yaw - 90.0f,
			0.0f
		));

		OnTargetLocked.Broadcast(false);
	}
}

void URZ_TurretComponent::Init(USceneComponent* ComponentToRotate)
{
	CompToRotate = ComponentToRotate;
}

void URZ_TurretComponent::ToggleTurretAI(bool bNewIsEnabled)
{
	bIsAIEnabled = bNewIsEnabled;
	
	if (bNewIsEnabled)
	{
		GetWorld()->GetTimerManager().SetTimer(
			PerceptionUpdateTimer,
			this,
			&URZ_TurretComponent::UpdateSensedActors,
			0.1f,
			true,
			0.1f
		);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(PerceptionUpdateTimer);
	}
}

void URZ_TurretComponent::UpdateSensedActors()
{
	if (PerceptionComp)
	{
		PerceptionComp->UpdateSensedActors();
	}
}
