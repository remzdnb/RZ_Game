#include "RZ_ProjectileWeapon.h"
#include "RZ_Projectile.h"
#include "RZ_Attachment.h"
#include "RZM_WeaponSystem.h"
#include "RZM_WeaponSystem.h"
// Engine
#include "Components/SkeletalMeshComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "Net/UnrealNetwork.h"

#pragma region +++++ ...

ARZ_ProjectileWeapon::ARZ_ProjectileWeapon()
{
	BaseSkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(FName("BaseSkeletalMeshComp"));
	BaseSkeletalMeshComp->SetCollisionProfileName("IgnoreAll");
	BaseSkeletalMeshComp->SetCustomDepthStencilValue(1);
	BaseSkeletalMeshComp->SetupAttachment(RootComponent);

	MagStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("MagStaticMeshComp"));
	MagStaticMeshComp->SetCollisionProfileName("IgnoreAll");
	MagStaticMeshComp->SetupAttachment(BaseSkeletalMeshComp);
	BarrelStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("BarrelStaticMeshComp"));
	BarrelStaticMeshComp->SetCollisionProfileName("IgnoreAll");
	BarrelStaticMeshComp->SetupAttachment(BaseSkeletalMeshComp);
	ScopeStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("ScopeStaticMeshComp"));
	ScopeStaticMeshComp->SetCollisionProfileName("IgnoreAll");
	ScopeStaticMeshComp->SetupAttachment(BaseSkeletalMeshComp);
	MuzzleTipSceneComp = CreateDefaultSubobject<USceneComponent>(FName("MuzzleTipSceneComp"));
	MuzzleTipSceneComp->SetupAttachment(BaseSkeletalMeshComp);
	//MuzzleTipSceneComp->SetRelativeLocation(FVector(-20.0f, 0.0f, 0.0f));

	PrimaryActorTick.bCanEverTick = true;
}

void ARZ_ProjectileWeapon::BeginPlay()
{
	Super::BeginPlay();

	/*MagDemoCT->DestroyComponent();
	BarrelDemoCT->DestroyComponent();
	ScopeDemoCT->DestroyComponent();*/

	//ProjectileWeaponSettings = *ItemActorPluginSettings->GetProjectileWeaponInfoFromRow(DataTableRowName);
	ClipAmmo = ProjectileWeaponSettings.MaxClipAmmo;
	StockAmmo = ProjectileWeaponSettings.MaxStockAmmo;
	BaseSkeletalMeshComp->SetRelativeScale3D(ProjectileWeaponSettings.MeshScale);

	/*ReloadCurve = DataManager->GlobalSettings.Linear1SCurve_0to1;
	if (ReloadCurve)
	{
		FOnTimelineFloat ReloadTimelineProgress;
		FOnTimelineEventStatic ReloadTimelineFinishedCallback;
		ReloadTimelineProgress.BindUFunction(this, FName("ReloadTimelineProgress"));
		ReloadTimelineFinishedCallback.BindUFunction(this, FName("ReloadTimelineEnd"));
		ReloadTimeline.AddInterpFloat(ReloadCurve, ReloadTimelineProgress);
		ReloadTimeline.SetTimelineFinishedFunc(ReloadTimelineFinishedCallback);
		ReloadTimeline.SetLooping(false);
		ReloadTimeline.SetPlayRate(1 / WeaponData.ReloadTime);
	}*/
}

void ARZ_ProjectileWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() == ROLE_Authority)
	{
		if (ClipAmmo == 0 && ItemState == ERZ_ItemState::Ready)
			StartReloading();
		else if (ProjectileWeaponSettings.bIsAutoFire)
			FireTick();
	}
	
	//

	ReloadTimeline.TickTimeline(DeltaTime);

	//

	/*if (ItemState != ERZ_ItemState::Reloading &&
		ItemState != ERZ_ItemState::Firing &&
		ItemState != ERZ_ItemState::Equipping)
	{
		const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), OwnerTargetLocation); // target location
		const FRotator LerpedRotation = FMath::Lerp(GetActorRotation(), TargetRotation, 1.0f);
		SetActorRotation(FRotator(0.0f, LerpedRotation.Yaw, 0.0f));
	}*/

	//
	
	Debug(DeltaTime);
}

/*void ARZ_ProjectileWeapon::UpdateItemState(ERZ_ItemState NewItemState)
{
	ItemState = NewItemState;
}*/

#pragma endregion

#pragma region +++++ Fire ...

void ARZ_ProjectileWeapon::FireTick()
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	if (ItemState == ERZ_ItemState::Firing && (CurrentTime - LastUseTime) >= ProjectileWeaponSettings.FireTime)
	{
		ItemState = ERZ_ItemState::Ready;
	}
		
	if (bWantsToUse && ItemState == ERZ_ItemState::Ready && (CurrentTime - LastUseTime) >= ProjectileWeaponSettings.FireTime)
	{
		ItemState = ERZ_ItemState::Firing;
		LastUseTime = CurrentTime;
		FireOnce();
	}
}

void ARZ_ProjectileWeapon::FireOnce()
{
	/*if (OwnerInventoryCT == nullptr)
		return;

	if (MagAttachment == nullptr)
		return;

	if (MagAttachment->GetQuantity() <= 0)
		return;
		*/
	ItemState = ERZ_ItemState::Firing;
	//MagAttachment->SetQuantity(MagAttachment->GetQuantity() -1);

	SpawnProjectile();
	SpawnFireFXMulticast();

	/*if (MagAttachment->GetQuantity() <= 0)
	{
		MagAttachment->Destroy();
		MagAttachment = nullptr;
		StartReloading();
	}

	if (OwnerInventoryCT.IsValid())
	{
		OwnerInventoryCT->InventoryUpdatedEvent.Broadcast(true);
		OwnerInventoryCT->GetOwnerInventoryInterface()->OnItemUse(WeaponData.FireTime, ItemData.AnimType);
	}*/

	//FireEvent.Broadcast(0.0f);*/
}

void ARZ_ProjectileWeapon::SpawnProjectile()
{
	const FVector SpawnLocation = MuzzleTipSceneComp->GetComponentLocation();
	const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, OwnerTargetLocation);
	const FTransform SpawnTransform(SpawnRotation, SpawnLocation, FVector(1.0f));
	const FActorSpawnParameters SpawnParameters;

	ARZ_Projectile* const Projectile = GetWorld()->SpawnActorDeferred<ARZ_Projectile>(
		ProjectileWeaponSettings.ProjectileBP,
		SpawnTransform,
		this,
		Cast<APawn>(GetOwner())
	);
	if (Projectile)
	{
		//Projectile->Init(ItemData->pr, DataRowName);
		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);

		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Projectile spawned !"));;
	}
	
	//UKismetSystemLibrary::DrawDebugLine(GetWorld(), SpawnLocation, TargetLocation, FColor::Yellow, 2.0f, 2.0f);
	//VCUtilityLibrary::PrintFloatToScreen("Fire Once : ", SpawnLocation.X, FColor::Purple, -1, 2.0f);
}

void ARZ_ProjectileWeapon::SpawnFireFXMulticast_Implementation()
{
	if (ProjectileWeaponSettings.MuzzleParticle)
	{
		UGameplayStatics::SpawnEmitterAttached(
			ProjectileWeaponSettings.MuzzleParticle,
			MuzzleTipSceneComp,
			"",
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTargetIncludingScale
		);

		OnItemUsed.Broadcast(this);
		//UE_LOG(LogTemp, Display, TEXT("ARZ_ProjectileWeapon::SpawnFireFXMulticast - 2"), *this->GetName());
	}

	/*if (WeaponData.FireSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponData.FireSound, GetActorLocation());
	}*/
}

#pragma endregion

#pragma region +++++ Reload ...

void ARZ_ProjectileWeapon::SetWantsToReload(bool bNewWantsToReload)
{
	bWantsToReload = bNewWantsToReload;

	if (GetLocalRole() < ROLE_Authority)
		SetWantsToReloadServer(bNewWantsToReload);
}

void ARZ_ProjectileWeapon::SetWantsToReloadServer_Implementation(bool bNewWantsToReload)
{
	SetWantsToReload(bNewWantsToReload);
}

void ARZ_ProjectileWeapon::StartReloading()
{	
	/*if (OwnerInventoryCT.IsValid() == false)
		return;

	if (ItemState == ERZ_ItemState::Ready)
	{
		if (MagAttachment)
			MagAttachment->Destroy();

		//OwnerInventoryCT->RefillEquippedItem();
		if (MagAttachment)
		{
			ReloadTimeline.PlayFromStart();
			ItemState = ERZ_ItemState::Reloading;
			//ReloadEvent.Broadcast(WeaponData.ReloadTime);

			//OwnerInventoryCT->InventoryUpdatedEvent.Broadcast(true);
			//OwnerInventoryCT->GetOwnerInventoryInterface()->OnItemReload(WeaponData.ReloadTime, ItemData.AnimType);
		}
	}*/
}

void ARZ_ProjectileWeapon::StopReloading()
{
	if (ItemState == ERZ_ItemState::Reloading)
	{
		ReloadTimeline.Stop();
		ItemState = ERZ_ItemState::Ready;
	}
}

void ARZ_ProjectileWeapon::ReloadTimelineProgress(float Value)
{
	ReloadTimelineProgressValue = Value;
}

void ARZ_ProjectileWeapon::ReloadTimelineEnd()
{
	if (ItemState == ERZ_ItemState::Reloading && ReloadTimeline.GetPlaybackPosition() == ReloadTimeline.GetTimelineLength())
	{
		// clip

		ItemState = ERZ_ItemState::Ready;
	}
}

#pragma endregion

#pragma region +++++ Aim spline mesh ...

void ARZ_ProjectileWeapon::SetupAimSplineMesh()
{
	/*AimSplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	AimSplineMesh->RegisterComponentWithWorld(GetWorld());
	AimSplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	AimSplineMesh->SetMobility(EComponentMobility::Movable);
	AimSplineMesh->SetStaticMesh(GameSettings->EngineCylinderMesh);
	AimSplineMesh->SetMaterial(0, GameSettings->TargetSplineMeshMaterial);
	AimSplineMesh->SetWorldScale3D(FVector(1.0f, 0.05f, 0.05f));
	AimSplineMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	AimSplineMesh->SetForwardAxis(ESplineMeshAxis::Z, true);*/
}

void ARZ_ProjectileWeapon::UpdateAimSplineMesh()
{
	/*if (IsLocallyControlled() == false)
		return;
	
	if (AimSplineMesh == nullptr)
		return;
	
	if (ItemManager && ItemManager->GetEquippedItem() != nullptr)
	{
		/// Snap mesh to projectile plane.

		AimSplineMesh->SetWorldLocation(FVector(
			GetMesh()->GetSocketLocation("hand_rSocket").X,
			GetMesh()->GetSocketLocation("hand_rSocket").Y,
			TOPDOWNPROJECTILEPLANEHEIGHT
		));

		const float Distance = FVector::Dist(AimSplineMesh->GetComponentLocation(), TargetLocation);

		AimSplineMesh->SetEndPosition(FVector(Distance, 0.0f, 0.0f), true);
		AimSplineMesh->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(AimSplineMesh->GetComponentLocation(), TargetLocation));

		AimSplineMesh->SetVisibility(true);
	}
	else
	{
		AimSplineMesh->SetVisibility(false);
	}*/
}

#pragma endregion

#pragma region +++++ Network / Debug ...

void ARZ_ProjectileWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARZ_ProjectileWeapon, ClipAmmo);
	DOREPLIFETIME(ARZ_ProjectileWeapon, StockAmmo);
	DOREPLIFETIME(ARZ_ProjectileWeapon, MagAttachment);
	DOREPLIFETIME(ARZ_ProjectileWeapon, BarrelAttachment);
	DOREPLIFETIME(ARZ_ProjectileWeapon, ScopeAttachment);
}

void ARZ_ProjectileWeapon::Debug(float DeltaTime)
{
	if (ItemActorPluginSettings == nullptr)
		return;
	
	if (ItemState == ERZ_ItemState::Holstered && ItemActorPluginSettings->bDebugHolsteredItems == false)
		return;

	FString RoleString = "None";
	if (GetLocalRole() == ROLE_Authority)
		RoleString = "Authority";
	if (GetLocalRole() == ROLE_AutonomousProxy)
		RoleString = "AutonomousProxy";
	if (GetLocalRole() == ROLE_SimulatedProxy)
		RoleString = "SimulatedProxy";

	FString StateString;
	if (ItemState == ERZ_ItemState::Ready)
		StateString = "Ready";
	else if (ItemState == ERZ_ItemState::Firing)
		StateString = "Firing";
	else
		StateString = "Unknown";

	FColor Color;
	if (GetLocalRole() == ROLE_Authority)
		Color = FColor::Green;
	else
		Color = FColor::White;

	const FString StringToPrint =
		this->GetName() +
		" // bWantsToUse : " + (bWantsToUse ? "True" : "False") +
		" // State : " + StateString;
		

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, Color, FString::Printf(TEXT("%s"), *StringToPrint));;
}

#pragma endregion