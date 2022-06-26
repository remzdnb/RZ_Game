// RemzDNB


#include "S2D_GridTile.h"
#include "S2D_GridManager.h"
//
#include "EngineUtils.h"
#include "RZM_Shared.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

AS2D_GridTile::AS2D_GridTile()
{
	RootSceneComp = CreateDefaultSubobject<USceneComponent>(FName("RootSceneComp"));
	RootSceneComp->SetMobility(EComponentMobility::Movable);
	RootComponent = RootSceneComp;

	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("BaseMeshComp"));
	BaseMeshComp->SetupAttachment(RootComponent);
	BaseMeshComp->SetMobility(EComponentMobility::Movable);
	BaseMeshComp->SetCollisionProfileName("IgnoreAll");
	BaseMeshComp->SetGenerateOverlapEvents(false);
	
	SouthBorderMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("SouthBorderMeshComp"));
	SouthBorderMeshComp->SetupAttachment(RootComponent);
	SouthBorderMeshComp->SetCollisionProfileName("IgnoreAll");

	NorthBorderMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("NorthBorderMeshComp"));
	NorthBorderMeshComp->SetupAttachment(RootComponent);
	NorthBorderMeshComp->SetCollisionProfileName("IgnoreAll");

	WestBorderMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("WestBorderMeshComp"));
	WestBorderMeshComp->SetupAttachment(RootComponent);
	WestBorderMeshComp->SetCollisionProfileName("IgnoreAll");
	
	EastBorderMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("EastBorderMeshComp"));
	EastBorderMeshComp->SetupAttachment(RootComponent);
	EastBorderMeshComp->SetCollisionProfileName("IgnoreAll");
	
	PrimaryActorTick.bCanEverTick = false;
}

void AS2D_GridTile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AS2D_GridTile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	/*BaseDMaterial = UMaterialInstanceDynamic::Create(
		GInstance->GameSettings->TileBaseMaterial,
		this
	);
	OwnershipMeshCT->SetMaterial(0, BaseDMaterial);*/
}

void AS2D_GridTile::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AS2D_GridManager> NewGridManager(GetWorld()); NewGridManager; ++NewGridManager)
	{
		if (NewGridManager)
		{
			GridManagerRef = *NewGridManager;
			return;
		}
	}

	SetActorHiddenInGame(false);

	NorthBorderMeshComp->SetHiddenInGame(true);
	SouthBorderMeshComp->SetHiddenInGame(true);
	EastBorderMeshComp->SetHiddenInGame(true);
	WestBorderMeshComp->SetHiddenInGame(true);

	//DefaultSouthBorderColor = SavedSouthBorderColor = GInstance->GameSettings->DefaultTileBorderColor;
	//DefaultNorthBorderColor = SavedNorthBorderColor = GInstance->GameSettings->DefaultTileBorderColor;
	//DefaultWestBorderColor = SavedWestBorderColor = GInstance->GameSettings->DefaultTileBorderColor;
	//DefaultEastBorderColor = SavedEastBorderColor = GInstance->GameSettings->DefaultTileBorderColor;
}

void AS2D_GridTile::Enable(const FS2D_GridTileData& NewTileData)
{
	TileData = NewTileData;

	SetActorHiddenInGame(false);
}

void AS2D_GridTile::Disable()
{
	SetActorHiddenInGame(true);
}

void AS2D_GridTile::UpdateBorders()
{
	if (!GridManagerRef) { return; }

	NorthBorderMeshComp->SetHiddenInGame(true);
	SouthBorderMeshComp->SetHiddenInGame(true);
	EastBorderMeshComp->SetHiddenInGame(true);
	WestBorderMeshComp->SetHiddenInGame(true);

	const FVector NorthTileLocation = GetActorLocation() + FVector(RZ_GRIDTILESIZE, 0.0f, 0.0f);
	const FVector SouthTileLocation = GetActorLocation() + FVector(RZ_GRIDTILESIZE * -1, 0.0f, 0.0f);
	const FVector EastTileLocation = GetActorLocation() + FVector(0.0f, RZ_GRIDTILESIZE, 0.0f);
	const FVector WestTileLocation = GetActorLocation() + FVector(0.0f, RZ_GRIDTILESIZE * -1, 0.0f);
	
	if (!GridManagerRef->IsActiveTileLocation(NorthTileLocation))
	{
		NorthBorderMeshComp->SetHiddenInGame(false);
	}

	if (!GridManagerRef->IsActiveTileLocation(SouthTileLocation))
	{
		SouthBorderMeshComp->SetHiddenInGame(false);
	}

	if (!GridManagerRef->IsActiveTileLocation(EastTileLocation))
	{
		EastBorderMeshComp->SetHiddenInGame(false);
	}

	if (!GridManagerRef->IsActiveTileLocation(WestTileLocation))
	{
		WestBorderMeshComp->SetHiddenInGame(false);
	}
}

/*
void ARZ_WorldTile::SetOwningTeam(ESS_Team Team)
{
	OwningTeam = Team;

	if (BaseDMaterial == nullptr)
		return;
	
	if (OwningTeam == ESS_Team::North)
	{
		BaseDMaterial->SetVectorParameterValue("Color", FLinearColor::Green);
	}

	if (OwningTeam == ESS_Team::South)
	{
		BaseDMaterial->SetVectorParameterValue("Color", FLinearColor::Red);
	}

	if (OwningTeam == ESS_Team::Neutral)
	{
		BaseDMaterial->SetVectorParameterValue("Color", FLinearColor::White);
	}
}

void ARZ_WorldTile::RegisterPawnBPI_Implementation(ASS_Pawn* PawnToRegister)
{
	RegisteredPawn = PawnToRegister;
}

void ARZ_WorldTile::UnregisterPawnBPI_Implementation()
{
	RegisteredPawn.Reset();
}

void ARZ_WorldTile::EnableHighlight(const FLinearColor& Color)
{
	SouthBorderParticleCT->SetColorParameter("Color", Color);
	SouthBorderParticleCT->SetFloatParameter("Opacity", Color.A);
	NorthBorderParticleCT->SetColorParameter("Color", Color);
	NorthBorderParticleCT->SetFloatParameter("Opacity", Color.A);
	WestBorderParticleCT->SetColorParameter("Color", Color);
	WestBorderParticleCT->SetFloatParameter("Opacity", Color.A);
	EastBorderParticleCT->SetColorParameter("Color", Color);
	EastBorderParticleCT->SetFloatParameter("Opacity", Color.A);
	
}

void ARZ_WorldTile::EnableDirectionalHighlight(ESS_Direction Direction, const FLinearColor& Color, bool bSaveColor) // show directional border
{
	switch (Direction)
	{
	case ESS_Direction::South:
		{
			SouthBorderParticleCT->SetColorParameter("Color", Color);
			SouthBorderParticleCT->SetFloatParameter("Opacity", Color.A);

			if (bSaveColor)
				SavedSouthBorderColor = Color;
		}
		break;
	case ESS_Direction::North:
		{
			NorthBorderParticleCT->SetColorParameter("Color", Color);
			NorthBorderParticleCT->SetFloatParameter("Opacity", Color.A);

			if (bSaveColor)
				SavedNorthBorderColor = Color;
		}
		break;
	case ESS_Direction::West:
		{
			WestBorderParticleCT->SetColorParameter("Color", Color);
			WestBorderParticleCT->SetFloatParameter("Opacity", Color.A);

			if (bSaveColor)
				SavedWestBorderColor = Color;
		}
		break;
	case ESS_Direction::East:
		{
			EastBorderParticleCT->SetColorParameter("Color", Color);
			EastBorderParticleCT->SetFloatParameter("Opacity", Color.A);

			if (bSaveColor)
				SavedEastBorderColor = Color;
		}
		break;
	}
}

void ARZ_WorldTile::DisableHighlight(bool bResetToDefault)
{
	if (bResetToDefault)
	{
		SavedSouthBorderColor = DefaultSouthBorderColor;
		SavedNorthBorderColor = DefaultNorthBorderColor;
		SavedWestBorderColor = DefaultWestBorderColor;
		SavedEastBorderColor = DefaultEastBorderColor;
	}

	SouthBorderParticleCT->SetColorParameter("Color", SavedSouthBorderColor);
	SouthBorderParticleCT->SetFloatParameter("Opacity", SavedSouthBorderColor.A);
	NorthBorderParticleCT->SetColorParameter("Color", SavedNorthBorderColor);
	NorthBorderParticleCT->SetFloatParameter("Opacity", SavedNorthBorderColor.A);
	WestBorderParticleCT->SetColorParameter("Color", SavedWestBorderColor);
	WestBorderParticleCT->SetFloatParameter("Opacity", SavedWestBorderColor.A);
	EastBorderParticleCT->SetColorParameter("Color", SavedEastBorderColor);
	EastBorderParticleCT->SetFloatParameter("Opacity", SavedEastBorderColor.A);
}*/
