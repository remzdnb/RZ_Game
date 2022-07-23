/// RemzDNB

#include "Pawn/RZ_Pawn.h"
#include "Core/RZ_GameInstance.h"
#include "Core/RZ_GameState.h"
#include "UI/RZ_OTM_Pawn_Widget.h"
// InventorySystem
#include "RZ_InventoryComponent.h"
// BuildingSystem
#include "RZ_BuildingComponent.h"
// PowerSystem
#include "RZ_PowerManager.h"
#include "RZ_PowerComponent.h"
//
#include "EngineUtils.h"
#include "Components/WidgetComponent.h"
#include "Character/RZ_Character.h"

#pragma region +++ ...

ARZ_Pawn::ARZ_Pawn()
{
	RootSceneComp = CreateDefaultSubobject<USceneComponent>(FName("RootSceneComp"));
	RootComponent = RootSceneComp;

	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("BaseMeshComp"));
	BaseMeshComp->SetupAttachment(RootComponent);
	BaseMeshComp->SetAbsolute(false);
	BaseMeshComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	BaseMeshComp->SetCollisionProfileName("Pawn");
	BaseMeshComp->SetGenerateOverlapEvents(true);
	BaseMeshComp->SetCustomDepthStencilValue(1);
	
	BuildMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("BuildMeshComp"));
	BuildMeshComp->SetupAttachment(RootComponent);
	BuildMeshComp->SetCollisionProfileName("IgnoreAll");
	BuildMeshComp->SetUsingAbsoluteRotation(true);
	//BuildSquareMeshComp->SetAbsolute(false, true, false);

	OTMWidgetComp = CreateDefaultSubobject<UWidgetComponent>(FName("OTMWidgetComp"));;
	OTMWidgetComp->SetupAttachment(RootComponent);
	OTMWidgetComp->SetCollisionProfileName("IgnoreAll");
	OTMWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	OTMWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	OTMWidgetComp->SetDrawSize(FVector2D(150.0f, 40.f));
	
	InventoryComp = CreateDefaultSubobject<URZ_InventoryComponent>("InventoryComp");
	BuildingComp = CreateDefaultSubobject<URZ_BuildingComponent>(FName("BuildingComp"));
	PowerComp = CreateDefaultSubobject<URZ_PowerComponent>(FName("PowerComp"));

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	bUseControllerRotationPitch = true; //?
	bUseControllerRotationYaw = true; //?
	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ARZ_Pawn::OnConstruction(const FTransform& InTransform)
{
	Super::OnConstruction(InTransform);
	
	// Align the collision box to the floor level. incroyable
	//const FVector BoxExtent = CollisionBoxCT->GetScaledBoxExtent();
	//CollisionBoxCT->SetRelativeLocation(FVector(0.0f, 0.0f, BoxExtent.Z));
}

void ARZ_Pawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!GetWorld()->IsGameWorld()) { return; }

	GameInstance = Cast<URZ_GameInstance>(GetGameInstance());
	GameSettings = Cast<URZ_GameInstance>(GetGameInstance())->GetGameSettings();
	GameState = Cast<ARZ_GameState>(GetWorld()->GetGameState());

	// Init ActorSettings.

	IRZ_InventorySystemInterface* InventorySystemInterface =
		Cast<IRZ_InventorySystemInterface>(GetWorld()->GetGameInstance());

	if (!InventorySystemInterface) { return; }
	if (!InventorySystemInterface->GetInventorySystemSettings()) { return; }
	if (!InventorySystemInterface->GetInventorySystemSettings()->GetItemSettingsFromTableRow(GetItemName())) { return; }

	ItemSettings = *InventorySystemInterface->GetInventorySystemSettings()->GetItemSettingsFromTableRow(GetItemName());

	//

	BuildingComp->Init(BaseMeshComp, nullptr);
}

void ARZ_Pawn::BeginPlay()
{
	Super::BeginPlay();
	
	//GameState->ReportPawnBeginPlay(this);
	
	//PawnCombatComp->OnHealthReachedZero.AddUniqueDynamic(this, &ARZ_Pawn::OnDestroyed);

	OTMWidget = Cast<URZ_OTM_Pawn_Widget>(OTMWidgetComp->GetWidget());
	if (OTMWidget)
		OTMWidget->Init(this);
}

void ARZ_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma endregion

#pragma region +++ InventorySystem ...

void ARZ_Pawn::OnAttachedToInventory()
{
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	//

	if (PowerComp && PowerComp->GetPowerManager())
	{
		if (PowerComp->GetPowerManager() &&
			PowerComp->GetPowerGridID() != 0)
		{
			PowerComp->GetPowerManager()->RemoveComponentFromGrid(PowerComp->GetPowerGridID(), PowerComp);
		}

		PowerComp->SetIsDisabled(true);
		for (const auto& Power : PowerComp->GetConnectedPowerComps())
		{
			Power->UpdateConnectedActors(); // OnPowerComponentMoved() in Manager.
		}
		PowerComp->GetPowerManager()->EvaluateGrids();
	}
}

void ARZ_Pawn::OnEquippedByInventory(bool bNewIsSelected)
{
	if (bNewIsSelected)
	{
		SetActorHiddenInGame(true);
		SetActorTickEnabled(false);
		BuildingComp->StartDemoBuild();
	}
	else
	{
		SetActorHiddenInGame(false);
		SetActorTickEnabled(false);
		BuildingComp->StopDemoBuild();
	}
}

void ARZ_Pawn::OnWantToBeUsedByInventory(bool bNewWantsTouse)
{
	if (bNewWantsTouse)
	{
		if (BuildingComp)
		{
			BuildingComp->StartBuild();
		}
	}
}

#pragma endregion

#pragma region +++ BuildingSystem ...

void ARZ_Pawn::OnDemoBuildStart()
{
	IRZ_InventoryOwnerInterface* InventoryOwnerInterface = Cast<IRZ_InventoryOwnerInterface>(GetOwner());
	if (InventoryOwnerInterface && InventoryOwnerInterface->GetInventoryComponent())
	{
		//InventoryOwnerInterface->GetInventoryComponent()->RemoveItemQuantityFromStorage(ItemName, 1);	
	}
	
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(false);

	IRZ_BuildingSystemInterface* BuildingSystemInterface =
		Cast<IRZ_BuildingSystemInterface>(GetWorld()->GetGameInstance());
	
// in component
	BaseMeshComp->SetMaterial(0, BuildingSystemInterface->GetBuildingSystemSettings()->BuildingMaterial_Invalid);
}

void ARZ_Pawn::OnDemoBuildStop()
{
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	//BaseMeshComp->SetMaterial(0, BaseMeshDefaultMaterial);
}

void ARZ_Pawn::OnBuildStart()
{
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(false);

	//BaseMeshComp->SetMaterial(0, BuildingManager->BuildingMaterial_Valid);
	//BuildMeshComp->SetVisibility(true);
	//OTMWidgetComp->SetVisibility(false);

	//Detach from inventory comp MDRRRRRRRRR.......>>>>
	
	//RZ_CommonLibrary::PrintStringToScreen("ARZ_Pawn::OnBuildStart", "", FColor::Green, -1, 3.0f);

	/*if (!GridManager) { return; }
	if (!PowerComp) { return; }
	if (!PowerComp->GetPowerManager()) { return; }

	GridManager->ClearActiveTiles();

	GridManager->AddSelection(
		GetActorLocation(),
		FIntPoint(PowerComp->GetPoweredAreaSize().X,
				  PowerComp->GetPoweredAreaSize().Y)
	);

	for (const auto& Grid : PowerComp->GetPowerManager()->GetPowerGrids())
	{
		for (const auto& AttachedPowerComponent : Grid.AttachedPowerComponents)
		{
			GridManager->AddSelection(
				AttachedPowerComponent->GetOwner()->GetActorLocation(),
				FIntPoint(AttachedPowerComponent->GetPoweredAreaSize().X,
						  AttachedPowerComponent->GetPoweredAreaSize().Y)
			);
		}
	}
	*/
}

void ARZ_Pawn::OnBuildStop()
{
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	//BaseMeshComp->SetMaterial(0, BaseMeshDefaultMaterial);
	BuildMeshComp->SetVisibility(false);

	//RZ_CommonLibrary::PrintStringToScreen("ARZ_Pawn::OnBuildStop", "", FColor::Green, -1, 3.0f);
}

void ARZ_Pawn::OnBuildEnd()
{
	if (!BuildingComp->GetIsBuilding()) { return; }
		
	SetActorTickEnabled(true);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	BuildMeshComp->SetVisibility(false);
	OTMWidgetComp->SetVisibility(true);

	UE_LOG(LogTemp, Display, TEXT("ARZ_Pawn::OnBuildEnd 0"));

	if (GetOwnerInventory())
	{
		UE_LOG(LogTemp, Display, TEXT("ARZ_Pawn::OnBuildEnd 1"));
		//GetOwnerInventory()->DropStorageItem();
	}
	
	/*ARZ_Character* CharacterOwner = Cast<ARZ_Character>(GetOwner());
	if (CharacterOwner)
	{
		UE_LOG(LogTemp, Display, TEXT("ARZ_Pawn::OnBuildEnd 1"));
		URZ_InventoryComponent* InvComp = CharacterOwner->GetInventoryComponent();
		if (InvComp)
		{
			UE_LOG(LogTemp, Display, TEXT("ARZ_Pawn::OnBuildEnd 2"));
			InvComp->DropSelectedSlot();
		}
	}*/
	
	//const FDetachmentTransformRules TransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);
	//DetachFromActor(TransformRules);
	
	/*PowerComp->SetIsDisabled(false);
	PowerComp->UpdateConnectedActors();
	for (const auto& Power : PowerComp->GetConnectedPowerComps())
	{
		Power->UpdateConnectedActors(); // OnPowerComponentMoved() in Manager.
	}
	PowerComp->GetPowerManager()->EvaluateGrids();*/
	
	//RZ_CommonLibrary::PrintStringToScreen("ARZ_Pawn::OnBuildEnd", "", FColor::Green, -1, 3.0f);
}

void ARZ_Pawn::OnValidBuildLocation()
{
	IRZ_BuildingSystemInterface* BuildingSystemInterface =
		Cast<IRZ_BuildingSystemInterface>(GetWorld()->GetGameInstance());
	
	BaseMeshComp->SetMaterial(0, BuildingSystemInterface->GetBuildingSystemSettings()->BuildingMaterial_Valid);
	BuildMeshComp->SetVisibility(true);
}

void ARZ_Pawn::OnInvalidBuildLocation()
{
	IRZ_BuildingSystemInterface* BuildingSystemInterface =
		Cast<IRZ_BuildingSystemInterface>(GetWorld()->GetGameInstance());
	
	BaseMeshComp->SetMaterial(0, BuildingSystemInterface->GetBuildingSystemSettings()->BuildingMaterial_Invalid);
	BuildMeshComp->SetVisibility(false);
}

#pragma endregion

void ARZ_Pawn::OnPowerStatusUpdated(bool bNewIsPowered)
{
	/*const ERZ_ActorMode NewActorMode = bNewIsPowered ? ERZ_ActorMode::Visible_Default : ERZ_ActorMode::Visible_Build;
	
	for (const auto& AttachedSlot : InventoryComp->GetAttachedSlots())
	{
		//Cast<IRZ_WorldInteractionInterface>(AttachedSlot.SpawnedActor)->SetActorMode(NewActorMode);
	}*/
}
/*
void ARZ_Pawn::OnHoverStart()
{
	BaseMeshComp->SetRenderCustomDepth(true);

	if (OTMWidget)
	{
		OTMWidget->ToggleBuildingOTM_BPI();
	}

	/*	if (!GridManager) { return; }
	if (!PowerComp) { return; }
	if (!PowerComp->GetPowerManager()) { return; }

	GridManager->ClearActiveTiles();

	for (const auto& Grid : PowerComp->GetPowerManager()->GetPowerGrids())
	{
		if (Grid.GridID == PowerComp->GetPowerGridID())
		{
			for (const auto& AttachedPowerComponent : Grid.AttachedPowerComponents)
			{
				GridManager->AddSelection(
					AttachedPowerComponent->GetOwner()->GetActorLocation(),
					FIntPoint(AttachedPowerComponent->GetPoweredAreaSize().X,
							  AttachedPowerComponent->GetPoweredAreaSize().Y)
				);
			}
		}
	}
}

void ARZ_Pawn::OnHoverEnd()
{
	BaseMeshComp->SetRenderCustomDepth(false);

	if (OTMWidget)
	{
		OTMWidget->ToggleBuildingOTM_BPI();
	}
}*/

/*

void AS2D_Pawn::OnBuildLocationUpdated(const FVector& NewBuildLocation)
{
Super::OnBuildLocationUpdated(NewBuildLocation);

GridManager->ClearActiveTiles();

GridManager->AddSelection(
GetActorLocation(),
FIntPoint(PowerComp->GetPoweredAreaSize().X,
PowerComp->GetPoweredAreaSize().Y)
);

for (const auto& Grid : PowerComp->GetPowerManager()->GetPowerGrids())
{
for (const auto& AttachedPowerComponent : Grid.AttachedPowerComponents)
{
GridManager->AddSelection(
AttachedPowerComponent->GetOwner()->GetActorLocation(),
FIntPoint(AttachedPowerComponent->GetPoweredAreaSize().X,
AttachedPowerComponent->GetPoweredAreaSize().Y)
);
}
}
}*/