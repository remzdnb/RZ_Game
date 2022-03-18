#include "Character/RZ_CharacterStart.h"
//
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"

ARZ_CharacterStart::ARZ_CharacterStart()
{
	CapsuleCT = CreateDefaultSubobject<UCapsuleComponent>(FName("CapsuleCT"));
	CapsuleCT->SetCollisionProfileName("OverlapOnlyPawn");
	RootComponent = CapsuleCT;

	ArrowCT = CreateDefaultSubobject<UArrowComponent>(FName("ArrowCT"));
	ArrowCT->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	/*static ConstructorHelpers::FObjectFinder<URZ_PawnManagerPluginSettings> Settings(TEXT(
		"RZ_PawnManagerPluginSettings'/RZ_PawnManagerPlugin/Data/RZ_PawnManagerPluginSettings_DA.RZ_PawnManagerPluginSettings_DA'"
	));
	if (Settings.Succeeded())
	{
		PawnManagerPluginSettings = Settings.Object;
	}*/
}

void ARZ_CharacterStart::OnConstruction(const FTransform& transform)
{
	Super::OnConstruction(transform);

	/*if (PawnManagerPluginSettings)
	{
		CapsuleCT->InitCapsuleSize(
			PawnManagerPluginSettings->PawnStartDefaultCapsuleSize.X,
			PawnManagerPluginSettings->PawnStartDefaultCapsuleSize.Y
		);
	}*/
}

void ARZ_CharacterStart::BeginPlay()
{
	Super::BeginPlay();
}

FTransform ARZ_CharacterStart::GetStartTransform()
{
	return CapsuleCT->GetComponentTransform();
}

void ARZ_CharacterStart::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APawn>(OtherActor) == nullptr)
		return;

	OverlappedActors.Add(OtherActor);

	bIsAvailable = false;
	//UpdateAvailability_Multicast(false);
}

void ARZ_CharacterStart::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APawn>(OtherActor) == nullptr)
		return;

	OverlappedActors.Remove(OtherActor);

	if (OverlappedActors.Num() == 0)
	{
		bIsAvailable = true;
		//UpdateAvailability_Multicast(true);
	}
	else
	{
		bIsAvailable = false;
		//UpdateAvailability_Multicast(false);
	}
}
