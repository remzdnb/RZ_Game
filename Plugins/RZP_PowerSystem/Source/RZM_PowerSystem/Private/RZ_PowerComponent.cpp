/// RemzDNB

#include "RZ_PowerComponent.h"
#include "RZ_PowerManager.h"
#include "RZ_PowerComponentInterface.h"
//
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"

URZ_PowerComponent::URZ_PowerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	PowerGridID = -1;
	bIsPowered = false;
	bIsDisabled = true;
	SpawnSize = FVector(1.0f);
}

void URZ_PowerComponent::BeginPlay()
{
	Super::BeginPlay();

	const IRZ_PowerSystemSettingsInterface* PowerSystemSettingsInterface = Cast<IRZ_PowerSystemSettingsInterface>(
		GetWorld()->GetGameInstance()
	);
	ensureAlways(PowerSystemSettingsInterface);
	PowerSystemSettings = PowerSystemSettingsInterface->GetPowerSystemSettings();
	ensureAlways(PowerSystemSettings);
	
	for (TActorIterator<ARZ_PowerManager> FoundPowerManager(GetWorld()); FoundPowerManager; ++FoundPowerManager)
	{
		PowerManager = *FoundPowerManager;
	}

	if (PowerManager)
	{
		PowerManager->AddPowerComponentRef(this);
	}

	IRZ_ActorInterface* ItemInterface = Cast<IRZ_ActorInterface>(GetOwner());
	if (ItemInterface)
	{
		SpawnSize = ItemInterface->GetActorSettings().NormalizedWorldSize;
	}
	
	CollisionComp = NewObject<UBoxComponent>(this, FName("PowerCollisionComp"));
	if (CollisionComp)
	{
		CollisionComp->AttachToComponent(
			GetOwner()->GetRootComponent(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale
		);
		CollisionComp->SetBoxExtent(
			SpawnSize * RZ_GRIDTILESIZE / 2 + PowerCompSettings.PowerRange * RZ_GRIDTILESIZE / 2
		);
		CollisionComp->SetCollisionProfileName("Power");
		CollisionComp->SetHiddenInGame(true);
		CollisionComp->RegisterComponent();
		CollisionComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &URZ_PowerComponent::OnCollisionCompBeginOverlap);
		CollisionComp->OnComponentEndOverlap.AddUniqueDynamic(this, &URZ_PowerComponent::OnCollisionCompEndOverlap);
	}	
}

void URZ_PowerComponent::UpdateConnectedActors() //
{
	/*for (const auto& ConnectedPowerComp : ConnectedPowerComps)
	{
		ConnectedPowerComp->UpdateConnectedActors();
	}*/ //stack overflow kek
	
	ConnectedPowerComps.Empty();

	//
	
	TArray<FHitResult> OutHits;
	const FVector SweepStart = GetOwner()->GetActorLocation();
	const FVector SweepEnd = GetOwner()->GetActorLocation();
	const FVector BoxExtent = FVector(
		SpawnSize.X * RZ_GRIDTILESIZE / 2 + PowerCompSettings.PowerRange * RZ_GRIDTILESIZE / 2
	);
	const FCollisionShape CollisionBox = FCollisionShape::MakeBox(BoxExtent);
	
	GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_GameTraceChannel12, CollisionBox);
	for (const auto& Hit : OutHits)
	{
		IRZ_PowerComponentInterface* PowerUserInterface = Cast<IRZ_PowerComponentInterface>(Hit.Actor);
		if (GetOwner() != Hit.Actor &&
			PowerUserInterface &&
			PowerUserInterface->GetPowerComponent() &&
			PowerUserInterface->GetPowerComponent()->GetIsDisabled() == false)
		{
			ConnectedPowerComps.Add(PowerUserInterface->GetPowerComponent());

			if (PowerSystemSettings->bDebugPowerComponent)
				UE_LOG(LogTemp, Display, TEXT("URZ_PowerComponent::UpdateConnectedActors // PowerUser hit == %s"), *Hit.Actor->GetName());
		}
		else
		{
			if (PowerSystemSettings->bDebugPowerComponent)
				UE_LOG(LogTemp, Display, TEXT("URZ_PowerComponent::UpdateConnectedActors // Discarded actor hit == %s"), *Hit.Actor->GetName());
		}
		// screen log information on what was hit

		// uncommnet to see more info on sweeped actor
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("All Hit Information: %s"), *Hit.ToString()));
	}

	//PowerManager->ReevaluteGrids();
	
	if (PowerSystemSettings->bDebugPowerComponent)
		DrawDebugBox(GetWorld(), GetOwner()->GetActorLocation(), BoxExtent, FColor::Blue, false, 3.0f, 0, 5.0f);
}

void URZ_PowerComponent::OnCollisionCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void URZ_PowerComponent::OnCollisionCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
