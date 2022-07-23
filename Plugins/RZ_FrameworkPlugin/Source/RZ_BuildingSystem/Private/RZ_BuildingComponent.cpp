/// RemzDNB

#include "RZ_BuildingComponent.h"
//
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameSession.h"
#include "Kismet/KismetMathLibrary.h"
#include "LauncherServices/Public/ILauncherProfile.h"

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

	IRZ_BuildingSystemInterface* BuildingSystemInterface = Cast<IRZ_BuildingSystemInterface>(GetWorld()->GetGameInstance());
	if (!BuildingSystemInterface) { return; }

	BuildingSystemSettings = BuildingSystemInterface->GetBuildingSystemSettings();
	
	OwnerBuildableActorInterface = Cast<IRZ_BuildableActorInterface>(GetOwner());
}

void URZ_BuildingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
						   FActorComponentTickFunction* TickFunction)
{
	Super::TickComponent(DeltaTime, TickType, TickFunction);

	Debug(DeltaTime);
	
	if (bIsBuilding)
	{
		if (GetWorld()->GetTimeSeconds() - BuildStartTime >= BuildableActorSettings.BuildTime)
		{
			EndBuild();
		}

		BuildDynamicMaterial->SetScalarParameterValue("Appearance",
		                                              (GetWorld()->GetTimeSeconds() - BuildStartTime) /
		                                              BuildableActorSettings.BuildTime);
	}

	UpdateBuildableActorLocation(DeltaTime);
	UpdateBuildableActorCollision(DeltaTime);
}

void URZ_BuildingComponent::Init(UStaticMeshComponent* StaticMeshComponent, USkeletalMeshComponent* SkeletalMeshComponent)
{
	if (StaticMeshComponent)
	{
		OwnerStaticMeshComp = StaticMeshComponent;
	
		BuildDynamicMaterial = UMaterialInstanceDynamic::Create(StaticMeshComponent->GetMaterial(0), this);
		BuildDynamicMaterial->SetScalarParameterValue("Appearance", 1.0f);
		
		int32 Index = 0;
		for (const auto& MaterialInterface : StaticMeshComponent->GetMaterials())
		{
			StaticMeshComponent->SetMaterial(Index, BuildDynamicMaterial);
			Index++;
		}
	}

	if (SkeletalMeshComponent)
	{
		OwnerSkeletalMeshComp = SkeletalMeshComponent;
		
		BuildDynamicMaterial = UMaterialInstanceDynamic::Create(SkeletalMeshComponent->GetMaterial(0), this);

		int32 Index = 0;
		for (const auto& MaterialInterface : SkeletalMeshComponent->GetMaterials())
		{
			SkeletalMeshComponent->SetMaterial(Index, BuildDynamicMaterial);
			Index++;
		}
	}
}

void URZ_BuildingComponent::StartDemoBuild()
{
	IRZ_BuildableActorInterface* BuildableInterface = Cast<IRZ_BuildableActorInterface>(GetOwner());
	if (!BuildableInterface) { return; }

	if (OwnerSkeletalMeshComp) { OwnerSkeletalMeshComp->SetMaterial(0, BuildingSystemSettings->BuildingMaterial_Valid); }
	if (OwnerStaticMeshComp) { OwnerStaticMeshComp->SetMaterial(0, BuildingSystemSettings->BuildingMaterial_Valid); }
	
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

void URZ_BuildingComponent::StartBuild()
{
	if (!OwnerBuildableActorInterface) { return; }

	if (bIsDemoBuilding)
	{
		OwnerBuildableActorInterface->OnDemoBuildStop();
		bIsDemoBuilding = false;
	}

	SetAllMaterials(BuildDynamicMaterial);

	BuildStartTime = GetWorld()->GetTimeSeconds();

	OwnerBuildableActorInterface->OnBuildStart();
	bIsBuilding = true;
}

void URZ_BuildingComponent::StopBuild()
{
	if (!OwnerBuildableActorInterface) { return; }
	
	OwnerBuildableActorInterface->OnBuildStop();
	bIsBuilding = false;
}

void URZ_BuildingComponent::EndBuild()
{
	if (!OwnerBuildableActorInterface) { return; }


	
	OwnerBuildableActorInterface->OnBuildEnd();
	bIsBuilding = false;
}

void URZ_BuildingComponent::UpdateBuildableActorLocation(float DeltaTime)
{
	if (!bIsDemoBuilding) { return; }
	
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	Controller->GetHitResultUnderCursorByChannel(
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel6),
		false,
		CursorToGroundHitResult
	);

	// big hack kek
	if (!Cast<ACharacter>(GetOwner()))
	{
		CursorToGroundHitResult.Location = GetOwner()->GetActorLocation();
	}
	//
	
	if (!OwnerBuildableActorInterface) { return; }

	FVector FinalBuildLocation;
	if (BuildableActorSettings.NormalizedBuildSize % 2 == 0) // Even GridSize
	{
		FinalBuildLocation = UKismetMathLibrary::Vector_SnappedToGrid(CursorToGroundHitResult.Location, 100.0f);
	}
	else
	{
		FinalBuildLocation = UKismetMathLibrary::Vector_SnappedToGrid(CursorToGroundHitResult.Location, 100.0f);
		FinalBuildLocation.X += 50.0f;
		FinalBuildLocation.Y += 50.0f;
	}
	
	GetOwner()->SetActorLocation(FinalBuildLocation);
	if (FinalBuildLocation != LastBuildActorLocation)
	{
		OwnerBuildableActorInterface->OnBuildLocationUpdated(FinalBuildLocation);
		LastBuildActorLocation = FinalBuildLocation;
	}

	//RZ_CommonLibrary::PrintVectorToScreen("URZ_BuildingComponent::Debug /// CursorToGroundHitResult == ", CursorToGroundHitResult.Location, FColor::Red, -1, DeltaTime);
	//RZ_CommonLibrary::PrintVectorToScreen("URZ_BuildingComponent::Debug /// FinalBuildLocation == ", FinalBuildLocation, FColor::Red, -1, DeltaTime);
}

void URZ_BuildingComponent::UpdateBuildableActorCollision(float DeltaTime)
{
	if (!BuildingSystemSettings) { return; }
	if (!OwnerBuildableActorInterface) { return; }
	if (!OwnerBuildableActorInterface->GetBuildingComponent()) { return; }

	if (!bIsDemoBuilding) { return; } // ?
	
	TArray<FHitResult> OutHits;
	const FVector SweepStart = GetOwner()->GetActorLocation();
	const FVector SweepEnd = GetOwner()->GetActorLocation();
	const FVector BoxExtent = FVector(OwnerBuildableActorInterface->GetBuildingComponent()->GetBuildableActorSettings().NormalizedBuildSize * 100.0f/*GRIDTILESIZE*/ / 2);
	const FCollisionShape CollisionBox = FCollisionShape::MakeBox(BoxExtent);
	
	GetWorld()->SweepMultiByChannel(OutHits, SweepStart, SweepEnd, FQuat::Identity, ECC_GameTraceChannel9, CollisionBox); // OverlapAll
	for (const auto& Hit : OutHits)
	{
		if (GetOwner() != Hit.Actor)
		{
			Cast<IRZ_BuildableActorInterface>(OwnerBuildableActorInterface)->OnInvalidBuildLocation();
			
			if (!BuildingSystemSettings->bDebugBuildComponent)
			{
				DrawDebugBox(GetWorld(), GetOwner()->GetActorLocation(), BoxExtent, FColor::Red, false, DeltaTime, 0, 4.0f);
				//UE_LOG(LogTemp, Display, TEXT("URZ_BuildingComponent::CollidingActor // Actor hit == %s"), *Hit.Actor->GetName());
			}
			
			return;
		}
	}

	OwnerBuildableActorInterface->OnValidBuildLocation();
	
	if (true)
	{
		DrawDebugBox(GetWorld(), GetOwner()->GetActorLocation(), BoxExtent, FColor::Green, false, DeltaTime, 0, 4.0f);
	}
}

void URZ_BuildingComponent::RotateBuildActor(bool bRotateRight) const
{
	if (!OwnerBuildableActorInterface) { return; }
	
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

void URZ_BuildingComponent::SetAllMaterials(UMaterialInterface* MaterialInterface)
{
	if (OwnerStaticMeshComp)
	{
		int32 Index = 0;
		for (const auto& SMaterial : OwnerStaticMeshComp->GetMaterials())
		{
			OwnerStaticMeshComp->SetMaterial(Index, MaterialInterface);
			Index++;
		}
	}

	if (OwnerSkeletalMeshComp)
	{
		int32 Index = 0;
		for (const auto& MatInt : OwnerSkeletalMeshComp->GetMaterials())
		{
			OwnerSkeletalMeshComp->SetMaterial(Index, MaterialInterface);
			Index++;
		}
	}
}

void URZ_BuildingComponent::Debug(float DeltaTime)
{
	if (!BuildingSystemSettings) { return; }
	if (!BuildingSystemSettings->bDebugBuildComponent) { return; }
	if (!bIsDemoBuilding && !bIsBuilding) { return; }
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, " ");

	const FString IsDemoBuildingString = "bIsDemoBuilding == " + FString::FromInt(bIsDemoBuilding);
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, IsDemoBuildingString);
	const FString IsBuildingString = "bIsBuilding == " + FString::FromInt(bIsBuilding);
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, IsBuildingString);
	const FString IsValidBuildLocationString = "IsValidBuildLocation == " + FString::FromInt(IsValidBuildLocation());
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, IsValidBuildLocationString);
	
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, " ");
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, "--------------------------------------------------");
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, this->GetName());
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, "--------------------------------------------------");
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Cyan, " ");
}
