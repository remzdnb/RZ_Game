///	RemzDNB

#include "Pawn/RZ_PawnStart.h"
//
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

ARZ_PawnStart::ARZ_PawnStart() :
	bIsEnabled(true),
	bIsAvailable(true)
{
	RootComp = CreateDefaultSubobject<USceneComponent>(FName("RootComp"));
	RootComponent = RootComp;
	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(FName("CapsuleComp"));
	CapsuleComp->SetCollisionProfileName("OverlapOnlyPawn");
	CapsuleComp->SetupAttachment(RootComp);

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(FName("ArrowComp"));
	ArrowComp->SetupAttachment(CapsuleComp);

	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void ARZ_PawnStart::OnConstruction(const FTransform& InTransform)
{
	Super::OnConstruction(InTransform);

	// Set the correct capsule Z relative location, relative to its height.
	
	float CapsuleRadius = 0.0f;
	float CapsuleHalfHeight = 0.0f;
	CapsuleComp->GetScaledCapsuleSize(CapsuleRadius, CapsuleHalfHeight);
	CapsuleComp->SetRelativeLocation(FVector(0.0f, 0.0f, CapsuleHalfHeight));

	//

	if (Ownership == ERZ_PawnOwnership::Player)
		ArrowComp->SetArrowColor(FLinearColor::Green);
	else if (Ownership == ERZ_PawnOwnership::WaveAI)
		ArrowComp->SetArrowColor(FLinearColor::Red);
	else if (Ownership == ERZ_PawnOwnership::FreeAI)
		ArrowComp->SetArrowColor(FLinearColor::Blue);
}

void ARZ_PawnStart::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (GetLocalRole() == ROLE_Authority)
	{
		CapsuleComp->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARZ_PawnStart::OnBeginOverlap);
		CapsuleComp->OnComponentEndOverlap.AddUniqueDynamic(this, &ARZ_PawnStart::OnEndOverlap);
	}
}

void ARZ_PawnStart::BeginPlay()
{
	Super::BeginPlay();
}

FTransform ARZ_PawnStart::GetStartTransform() const
{
	return CapsuleComp->GetComponentTransform();
}

void ARZ_PawnStart::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetLocalRole() != ROLE_Authority)
		return;
	
	if (Cast<APawn>(OtherActor) == nullptr)
		return;

	OverlappedPawns.Add(OtherActor);
	
	UpdateAvailability_Server(false);
}

void ARZ_PawnStart::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetLocalRole() != ROLE_Authority)
		return;
	
	if (Cast<APawn>(OtherActor) == nullptr)
		return;

	OverlappedPawns.Remove(OtherActor);

	if (OverlappedPawns.Num() == 0)
	{
		bIsAvailable = true;
		UpdateAvailability_Server(true);
	}
	else
	{
		bIsAvailable = false;
		UpdateAvailability_Server(false);
	}
}

void ARZ_PawnStart::UpdateAvailability_Server_Implementation(bool bNewIsAvailable)
{
	bIsAvailable = bNewIsAvailable;
	OnRep_IsAvailable(); // Call manually because OnRep isn't called server side.
}

void ARZ_PawnStart::OnRep_IsAvailable()
{
	UpdateAvailability_BPI(bIsAvailable);
}

void ARZ_PawnStart::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARZ_PawnStart, bIsAvailable);
}

