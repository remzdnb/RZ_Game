// RemzDNB

#include "RZ_Actor2DRenderer.h"
//
#include "RZ_BuildingSystem.h"
#include "RZ_InventorySystem.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"

ARZ_Actor2DRenderer::ARZ_Actor2DRenderer()
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

void ARZ_Actor2DRenderer::BeginPlay()
{
	Super::BeginPlay();

	//SpawnNewActor("Character_01");
}

void ARZ_Actor2DRenderer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpawnedActor && bIsAutoRotating)
	{
		SpawnedActor->AddActorLocalRotation(FRotator(0.0f, 50.0f * DeltaTime, 0.0f));
	}
}

void ARZ_Actor2DRenderer::SpawnNewActor(const FName& ActorName)
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
		APawn* ThisPawn = Cast<APawn>(NewActor);
		if (ThisPawn)
		{
			ThisPawn->AutoPossessAI = EAutoPossessAI::Disabled;
			ThisPawn->Tags.Add("Demo");
		}
		UGameplayStatics::FinishSpawningActor(NewActor, GetActorTransform());
		SpawnedActor = NewActor;

		//IRZ_BuildableActorInterface* Buildable = Cast<IRZ_BuildableActorInterface>(NewActor);
		//Buildable->OnBuildEnd();

		UE_LOG(LogTemp, Display, TEXT("ARZ_Actor2DRenderer::SpawnNewActor success - %s"), *ActorName.ToString());
	}
}

void ARZ_Actor2DRenderer::StopActorRotation()
{
	bIsAutoRotating = false;
}

void ARZ_Actor2DRenderer::SetSpawnedActorRelativeLocation(const FVector& NewLocation)
{
	if (SpawnedActor)
	{
		SpawnedActor->SetActorRelativeLocation(NewLocation);
	}
}

void ARZ_Actor2DRenderer::SetActorLocalRotation(const FRotator& NewRotation)
{
	if (SpawnedActor)
	{
		SpawnedActor->SetActorRelativeRotation(NewRotation);
	}
}

