/// RemzDNB

#include "S2D_Pawn.h"
#include "Core/RZ_GameSettings.h"
#include "RZ_PowerManager.h"
#include "RZ_PowerComponent.h"
#include "RZ_InventoryComponent.h"
//
#include "EngineUtils.h"
#include "S2D_GridManager.h"

AS2D_Pawn::AS2D_Pawn()
{
	BuildSquareMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("BuildSquareMeshComp"));
	BuildSquareMeshComp->SetupAttachment(RootComponent);
	BuildSquareMeshComp->SetCollisionProfileName("IgnoreAll");
	//BuildSquareMeshComp->SetAbsolute(false, true, false);
	
	PowerComp = CreateDefaultSubobject<URZ_PowerComponent>(FName("PowerComp"));
}

void AS2D_Pawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!GetWorld()->IsGameWorld()) { return; }

	BaseMeshDefaultMaterial = BaseMeshComp->GetMaterial(0);

	for (TActorIterator<AS2D_GridManager> GridManagerItr(GetWorld()); GridManagerItr; ++GridManagerItr)
	{
		GridManager = *GridManagerItr;
	}
}

void AS2D_Pawn::BeginPlay()
{
	Super::BeginPlay();
}

void AS2D_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AS2D_Pawn::OnBuildStart()
{
	//IS2D_BuildableInterface::OnBuildStart();
	
	BaseMeshComp->SetMaterial(0, GameSettings->ItemSpawnMaterial_Valid);
}

void AS2D_Pawn::OnBuildStop()
{
	//IS2D_BuildableInterface::OnBuildStop();
	
	BaseMeshComp->SetMaterial(0, BaseMeshDefaultMaterial);
}

void AS2D_Pawn::OnBuildEnd()
{
	//IS2D_BuildableInterface::OnBuildEnd();
	
	BaseMeshComp->SetMaterial(0, BaseMeshDefaultMaterial);
	
	const FDetachmentTransformRules TransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);
	DetachFromActor(TransformRules);

	SetItemMode(ERZ_ItemMode::Visible);

	URZ_InventoryComponent* InvComp = Cast<URZ_InventoryComponent>(OwnerInventory);
	if (InvComp)
	{
		InvComp->DropSelectedSlot();
	}
}

void AS2D_Pawn::OnHoverStart()
{
	Super::OnHoverStart();

	if (BuildSquareMeshComp)
	{
		BuildSquareMeshComp->SetVisibility(true);
	}
	
	if (!GridManager) { return; }
	if (!PowerComp) { return; }
	if (!PowerComp->PowerManager) { return; }

	GridManager->ShowPowerGrid(PowerComp->PowerManager->GetComponentsFromGrid(PowerComp->PowerGridID));
}

void AS2D_Pawn::OnHoverEnd()
{
	Super::OnHoverEnd();

	if (BuildSquareMeshComp)
	{
		BuildSquareMeshComp->SetVisibility(false);
	}

	if (!GridManager) { return; }

	GridManager->HidePowerGrid();
}



	