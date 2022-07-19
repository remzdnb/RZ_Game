// RemzDNB

#include "RZ_Item2DRenderer.h"
//
#include "RZ_InventorySystem.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"

ARZ_Item2DRenderer::ARZ_Item2DRenderer()
{
	RootSceneComp = CreateDefaultSubobject<USceneComponent>(FName("RootSceneComp"));
	RootComponent = RootSceneComp;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootSceneComp);

	SceneCaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(FName("SceneCaptureComp"));
	SceneCaptureComp->SetupAttachment(SpringArmComp);
	
	PrimaryActorTick.bCanEverTick = true;

	bIsAutoRotating = true;
}

void ARZ_Item2DRenderer::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARZ_Item2DRenderer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpawnedActor && bIsAutoRotating)
	{
		SpawnedActor->AddActorLocalRotation(FRotator(0.0f, 50.0f * DeltaTime, 0.0f));
	}
}

void ARZ_Item2DRenderer::SpawnNewActor(const FName& ActorName)
{
	IRZ_InventorySystemInterface* InventorySystemInterface =
		Cast<IRZ_InventorySystemInterface>(GetWorld()->GetGameInstance());

	if (!InventorySystemInterface) { return; }
	if (!InventorySystemInterface->GetInventorySystemSettings()) { return; }
	if (!InventorySystemInterface->GetInventorySystemSettings()->GetItemSettingsFromTableRow(ActorName)) { return; }
	
	//

	if (SpawnedActor)
	{
		SpawnedActor->Destroy();
	}

	//

	AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(
		InventorySystemInterface->GetInventorySystemSettings()->GetItemSettingsFromTableRow(ActorName)->ActorClass,
		GetActorTransform(),
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);
	if (NewActor)
	{
		UGameplayStatics::FinishSpawningActor(NewActor, GetActorTransform());
		SpawnedActor = NewActor;
	}
}

void ARZ_Item2DRenderer::StopActorRotation()
{
	bIsAutoRotating = false;
}

void ARZ_Item2DRenderer::SetSpawnedActorRelativeLocation(const FVector& NewLocation)
{
	if (SpawnedActor)
	{
		SpawnedActor->SetActorRelativeLocation(NewLocation);
	}
}

void ARZ_Item2DRenderer::SetActorLocalRotation(const FRotator& NewRotation)
{
	if (SpawnedActor)
	{
		SpawnedActor->SetActorRelativeRotation(NewRotation);
	}
}

