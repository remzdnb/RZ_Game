#include "RZ_WorldTile.h"
//
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ARZ_WorldTile::ARZ_WorldTile()
{
	/*RootCT = CreateDefaultSubobject<USceneComponent>(FName("RootCT"));
	RootComponent = RootCT;
	
	CollisionCT = CreateDefaultSubobject<UBoxComponent>(FName("CollisionCT"));
	CollisionCT->SetupAttachment(RootComponent);
	CollisionCT->SetCollisionProfileName("BlockAll");
	CollisionCT->SetGenerateOverlapEvents(true);
	CollisionCT->SetBoxExtent(FVector(TILESIZE / 2, TILESIZE / 2, 1.0f));

	OwnershipMeshCT = CreateDefaultSubobject<UStaticMeshComponent>(FName("OwnershipMeshCT"));
	OwnershipMeshCT->SetupAttachment(RootComponent);
	OwnershipMeshCT->SetCollisionProfileName("NoCollision");
	OwnershipMeshCT->SetGenerateOverlapEvents(false);
	
	SouthBorderParticleCT = CreateDefaultSubobject<UParticleSystemComponent>(FName("SouthBorderParticleCT"));
	SouthBorderParticleCT->SetupAttachment(RootComponent);

	NorthBorderParticleCT = CreateDefaultSubobject<UParticleSystemComponent>(FName("NorthBorderParticleCT"));
	NorthBorderParticleCT->SetupAttachment(RootComponent);

	WestBorderParticleCT = CreateDefaultSubobject<UParticleSystemComponent>(FName("WestBorderParticleCT"));
	WestBorderParticleCT->SetupAttachment(RootComponent);

	EastBorderParticleCT = CreateDefaultSubobject<UParticleSystemComponent>(FName("EastBorderParticleCT"));
	EastBorderParticleCT->SetupAttachment(RootComponent);*/
	
	PrimaryActorTick.bCanEverTick = false;
}
/*
void ARZ_WorldTile::Init(const FRZ_WorldTileData& NewTileData)
{
	TileData = NewTileData;
}

void ARZ_WorldTile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ARZ_WorldTile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GInstance = Cast<USS_GameInstance>(GetGameInstance());

	BaseDMaterial = UMaterialInstanceDynamic::Create(
		GInstance->GameSettings->TileBaseMaterial,
		this
	);
	OwnershipMeshCT->SetMaterial(0, BaseDMaterial);
}

void ARZ_WorldTile::BeginPlay()
{
	Super::BeginPlay();



	if (TileData.Type == ERZ_WorldTileType::Border)
	{
		OwnershipMeshCT->SetHiddenInGame(true);
		SouthBorderParticleCT->SetHiddenInGame(true);
		NorthBorderParticleCT->SetHiddenInGame(true);
		WestBorderParticleCT->SetHiddenInGame(true);
		EastBorderParticleCT->SetHiddenInGame(true);
	}

	DefaultSouthBorderColor = SavedSouthBorderColor = GInstance->GameSettings->DefaultTileBorderColor;
	DefaultNorthBorderColor = SavedNorthBorderColor = GInstance->GameSettings->DefaultTileBorderColor;
	DefaultWestBorderColor = SavedWestBorderColor = GInstance->GameSettings->DefaultTileBorderColor;
	DefaultEastBorderColor = SavedEastBorderColor = GInstance->GameSettings->DefaultTileBorderColor;


}

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

void ARZ_WorldTile::EnableDirectionalHighlight(ESS_Direction Direction, const FLinearColor& Color, bool bSaveColor)
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