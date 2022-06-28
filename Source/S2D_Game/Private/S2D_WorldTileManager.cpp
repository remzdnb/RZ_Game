/// RemzDNB

#include "S2D_WorldTileManager.h"
#include "S2D_WorldTile.h"
#include "S2D_GameState.h"
//
#include "Core/RZ_GameInstance.h"
#include "Core/RZ_GameSettings.h"
#include "RZ_PowerComponent.h"
//
#include "AITypes.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "RZ_PowerManager.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

AS2D_WorldTileManager::AS2D_WorldTileManager()
{
	RootSceneComp = CreateDefaultSubobject<USceneComponent>(FName("RootSceneComp"));
	RootSceneComp->SetMobility(EComponentMobility::Static);
	RootComponent = RootSceneComp;

	DirectionArrowComp = CreateDefaultSubobject<UArrowComponent>(FName("DirectionArrowComp"));
	DirectionArrowComp->SetMobility(EComponentMobility::Static);
	DirectionArrowComp->ArrowSize = 5.0f;
	DirectionArrowComp->SetupAttachment(RootComponent);

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	bDebug = false;
}

void AS2D_WorldTileManager::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AS2D_WorldTileManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	S2DGameSettings = GetWorld()->GetGameState<AS2D_GameState>()->GetS2DGameSettings();
}

void AS2D_WorldTileManager::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ARZ_PowerManager> NewPowerManager(GetWorld()); NewPowerManager; ++NewPowerManager)
	{
		if (NewPowerManager)
		{
			PowerManagerRef = *NewPowerManager;
			break;
		}
	}

	// Spawn pooled tiles.
	
	for (int32 Index = 0; Index < S2DGameSettings->MaxGridTiles; Index++)
	{
		AS2D_WorldTile* NewTile = GetWorld()->SpawnActorDeferred<AS2D_WorldTile>(
			S2DGameSettings->GridTileClass,
			FTransform(),
			this,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
		if (NewTile)
		{
			UGameplayStatics::FinishSpawningActor(NewTile, FTransform());
			
			PooledTiles.Add(NewTile);
		}
	}
}

void AS2D_WorldTileManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Debug(DeltaTime);
}

void AS2D_WorldTileManager::AddSelection(const FVector& Location, FIntPoint Size)
{
	if (!PooledTiles.IsValidIndex(0))
	{
		UE_LOG(LogTemp, Error, TEXT("AS2D_WorldTileManager::EnableTile : Out of pooled tiles !"));
		return;
	}

	//

	FIntPoint FinalSize = Size;
	
	const bool bHasOddSize = Size.X % 2 == 0 ? false : true;
	if (!bHasOddSize)
	{
		FinalSize.X++;
		FinalSize.Y++;
	}
	
	// Even // Get Size Rect from pawn.
	for (int32 XIndex = FinalSize.X / 2 * -1; XIndex <= FinalSize.X / 2; XIndex++)
	{
		for (int32 YIndex = FinalSize.Y / 2 * -1; YIndex <= FinalSize.Y / 2; YIndex++)
		{
			if ((YIndex == 0 || XIndex == 0) && !bHasOddSize)
			{
				
			}
			else
			{
				if (!PooledTiles.IsValidIndex(0))
				{
					UE_LOG(LogTemp, Error, TEXT("AS2D_WorldTileManager::EnableTile : Out of pooled tiles !"));
					return;
				}
				
				if (bHasOddSize)
				{
					const FVector TargetTileLocation = FVector(
						Location.X + XIndex * RZ_GRIDTILESIZE,
						Location.Y + YIndex * RZ_GRIDTILESIZE,
						0.0f
					);

					if (IsValidNewTileLocation(TargetTileLocation))
					{
						PooledTiles[0]->SetActorLocation(TargetTileLocation);
						PooledTiles[0]->Enable(FS2D_WorldTileData());
						ActiveTiles.Add(PooledTiles[0]);
						PooledTiles.RemoveAt(0);
					}
				}
				else
				{
					const FVector TargetTileLocation = FVector(
						Location.X + XIndex * RZ_GRIDTILESIZE,
						Location.Y + YIndex * RZ_GRIDTILESIZE,
						0.0f
					);
					
					const int32 PositiveX = XIndex > 0 ? 1 : -1;
					const int32 PositiveY = YIndex > 0 ? 1 : -1;

					if (IsValidNewTileLocation(TargetTileLocation))
					{
						PooledTiles[0]->SetActorLocation(FVector(
							Location.X + XIndex * RZ_GRIDTILESIZE - RZ_GRIDTILESIZE / 2 * PositiveX,
							Location.Y + YIndex * RZ_GRIDTILESIZE - RZ_GRIDTILESIZE / 2 * PositiveY,
							PooledTiles[0]->GetActorLocation().Z
						));

						PooledTiles[0]->Enable(FS2D_WorldTileData());
						ActiveTiles.Add(PooledTiles[0]);
						PooledTiles.RemoveAt(0);
					}
				}
			}
		}
	}

	for (const auto& ActiveTile : ActiveTiles)
	{
		ActiveTile->UpdateBorders();
	}
	
	/*FRZ_IntRect IntRect;
	IntRect.Min.X = OriginLocation.X - Size.X * RZ_GRIDTILESIZE;
	IntRect.Max.X = OriginLocation.X + Size.X * RZ_GRIDTILESIZE;
	IntRect.Min.Y = OriginLocation.Y - Size.Y * RZ_GRIDTILESIZE;
	IntRect.Max.Y = OriginLocation.Y + Size.Y * RZ_GRIDTILESIZE;

	for (int32 XIndex = IntRect.Min.X; XIndex < IntRect.Max.X; XIndex++)
	{
		for (int32 YIndex = IntRect.Min.Y; YIndex < IntRect.Min.Y; YIndex++)
		{
			DisabledTiles[0]->SetActorLocation(FVector(OriginLocation.X, OriginLocation.Y, 1.0f));
			DisabledTiles[0]->Enable(FS2D_WorldTileData());
			EnabledTiles.Add(DisabledTiles[0]);
			DisabledTiles.RemoveAt(0);
			//SpawnLocation = FVector(XIndex * WORLDTILESIZE, YIndex * WORLDTILESIZE, GetActorLocation().Z);
			//SpawnTransform = FTransform(GetActorRotation(), SpawnLocation, FVector(1.0f));
			//SpawnTile(SpawnTransform, XIndex, YIndex, WORLDTILESIZE / 2, WORLDTILESIZE / 2, ES2D_WorldTileType::Grid);
		}
	}*/

	/*DisabledTiles[0]->SetActorLocation(FVector(OriginLocation.X, OriginLocation.Y, 1.0f));
	DisabledTiles[0]->Enable(FS2D_WorldTileData());
	EnabledTiles.Add(DisabledTiles[0]);
	DisabledTiles.RemoveAt(0);*/

	if (bDebug)
	{
		UE_LOG(LogTemp, Display, TEXT("AS2D_WorldTileManager::AddRectSelection"));
	}
}

void AS2D_WorldTileManager::EnableTile(FIntPoint Position)
{
	if (!PooledTiles.IsValidIndex(0))
	{
		UE_LOG(LogTemp, Error, TEXT("AS2D_WorldTileManager::EnableTile : Out of pooled tiles !"));
		return;
	}

	PooledTiles[0]->SetActorLocation(FVector(Position.X, Position.Y, 1.0f));
	PooledTiles[0]->Enable(FS2D_WorldTileData());
	ActiveTiles.Add(PooledTiles[0]);
	PooledTiles.RemoveAt(0);

	if (bDebug)
	{
		UE_LOG(LogTemp, Display, TEXT("AS2D_WorldTileManager::EnableTile"));
	}
}

void AS2D_WorldTileManager::AddRectSelection(FRZ_IntRect Rect)
{
	FIntPoint Index;
	
	for (Index.X = Rect.Min.X; Index.X <= Rect.Max.X; Index.X++)
	{
		for (Index.Y = Rect.Min.Y; Index.Y <= Rect.Max.Y; Index.Y++)
		{
			EnableTile(Index);
		}
	}
}

void AS2D_WorldTileManager::ClearActiveTiles()
{
	for (const auto& EnabledTile : ActiveTiles)
	{
		EnabledTile->Disable();
		PooledTiles.Add(EnabledTile);
	}

	ActiveTiles.Empty();
}

bool AS2D_WorldTileManager::IsValidNewTileLocation(const FVector& TargetLocation) const
{
	if (IsActiveTileLocation(TargetLocation) == true) { return false; }

	//

	FCollisionQueryParams CollisionParams;
	FHitResult OutHit;
	FVector StartTraceLocation = TargetLocation;
	FVector EndTraceLocation = TargetLocation;
	StartTraceLocation.Z = 200.0f;
	EndTraceLocation.Z = -50.0f;
	if(GetWorld()->LineTraceSingleByChannel(OutHit, StartTraceLocation, EndTraceLocation, ECC_GameTraceChannel10, CollisionParams)) //OverlapPawnBlockWorld
	{
		if (bDebug)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Impact Point: %s"), *OutHit.ImpactPoint.ToString()));
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Normal Point: %s"), *OutHit.ImpactNormal.ToString()));
		}
		
		if (OutHit.bBlockingHit)
		{
			return false;
		}
	}

	if (bDebug) { DrawDebugLine(GetWorld(), StartTraceLocation, EndTraceLocation, FColor::Green, false, 1, 0, 1); }
	
	return true;
}


bool AS2D_WorldTileManager::IsActiveTileLocation(const FVector& TargetLocation) const
{
	for (const auto& ActiveTile : ActiveTiles)
	{
		if (ActiveTile->GetActorLocation().X == TargetLocation.X &&
			ActiveTile->GetActorLocation().Y == TargetLocation.Y)
		{
			return true;
		}
	}

	return false;
}

void AS2D_WorldTileManager::Debug(float DeltaTime)
{
	if (!bDebug) { return; }

	const FString SeparatorString = " ------------------------------------------------------------- ";
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Purple, SeparatorString);;
	
	for (const auto& EnabledTile : ActiveTiles)
	{
		FString GridString =
			"Enabled Tile - X == " +
			FString::FromInt(EnabledTile->GetActorLocation().X) +
			" // Y == " +
			FString::FromInt(EnabledTile->GetActorLocation().Y);

		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Purple, GridString);;
	}
	
	const FString FinalString = " ------------------- AS2D_WorldTileManager::Debug ------------------- ";
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Purple, FinalString);;
}

/*
void AS2D_WorldTileManager::SpawnTile(const FTransform& SpawnTransform, int32 PositionX,
						 int32 PositionY, float SizeX, float SizeY, ES2D_WorldTileType TileType)
{

	// Grid tiles
	
	for (int32 XIndex = 0; XIndex < GridSize.X; XIndex++)
	{
		for (int32 YIndex = 0; YIndex < GridSize.Y; YIndex++)
		{
			//SpawnLocation = FVector(XIndex * WORLDTILESIZE, YIndex * WORLDTILESIZE, GetActorLocation().Z);
			SpawnTransform = FTransform(GetActorRotation(), SpawnLocation, FVector(1.0f));
			//SpawnTile(SpawnTransform, XIndex, YIndex, WORLDTILESIZE / 2, WORLDTILESIZE / 2, ES2D_WorldTileType::Grid);
		}
	}
	
	AS2D_WorldTile* NewTile = GetWorld()->SpawnActorDeferred<AS2D_WorldTile>(
		TileClass,
		SpawnTransform,
		this,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);
	if (NewTile)
	{
		FS2D_WorldTileData NewTileData;
		NewTileData.Position.X = PositionX;
		NewTileData.Position.Y = PositionY;
		NewTileData.Type = TileType;
		//NewTile->Init(NewTileData);
		//NewTile->GetCollisionBox()->SetBoxExtent(FVector(SizeX, SizeY, 1.0f));
		UGameplayStatics::FinishSpawningActor(NewTile, SpawnTransform);
	}

	if (TileType == ES2D_WorldTileType::Grid)
		DisabledTiles.Add(NewTile);
}
*/
/*

void ARZ_WorldGrid::OnGamePhaseUpdated(ESS_GamePhase NewGamePhase)
{
	if (NewGamePhase == ESS_GamePhase::Strategic)
	{
		UpdateTilesOwnership();
	}
}

#pragma region +++++ Helpers ...

ARZ_WorldTile* ARZ_WorldGrid::ConvertBorderToGridTile(const ARZ_WorldTile* BorderTile) const
{
	FIntPoint ResultPosition;

	if (BorderTile->GetTileData().Position.X == -1)
	{
		ResultPosition.X = 0;
	}
	else if (BorderTile->GetTileData().Position.X >= GridSize.X)
	{
		ResultPosition.X = GridSize.X - 1;
	}
	else
	{
		ResultPosition.X = BorderTile->GetTileData().Position.X;
	}
	
	if (BorderTile->GetTileData().Position.Y == -1)
	{
		ResultPosition.Y = 0;
	}
	else if (BorderTile->GetTileData().Position.Y >= GridSize.Y)
	{
		ResultPosition.Y = GridSize.Y -1;
	}
	else
	{
		ResultPosition.Y = BorderTile->GetTileData().Position.Y;
	}
	
	return GetTileFromPosition(ResultPosition);
}

ARZ_WorldTile* ARZ_WorldGrid::GetTileFromPosition(const FIntPoint& Position) const
{
	if (Position.X >= GridSize.X || Position.Y >= GridSize.Y || Position.X < 0 || Position.Y < 0)
		return nullptr;
	
	const int32 TileIndex = GridSize.Y * Position.X + Position.Y;
	if (GridTiles.IsValidIndex(TileIndex))
	{
		if (GridTiles[TileIndex]->GetTileData().Type == ERZ_WorldTileType::Border) // maybe not necessary
			return nullptr;
		
		return GridTiles[TileIndex];	
	}

	return nullptr;	
}

TArray<ARZ_WorldTile*> ARZ_WorldGrid::GetTilesFromRect(const FRZ_IntRect& Rect) const
{
	TArray<ARZ_WorldTile*> ResultArray;

	FIntPoint Index;
	for (Index.X = Rect.Min.X; Index.X <= Rect.Max.X; Index.X++)
	{
		for (Index.Y = Rect.Min.Y; Index.Y <= Rect.Max.Y; Index.Y++)
		{
			ResultArray.Add(GetTileFromPosition(Index));
		}
	}

	return ResultArray;
}

bool ARZ_WorldGrid::IsValidRect(const FRZ_IntRect& Rect, const ASS_Pawn* PawnToIgnore) const
{
	TArray<ARZ_WorldTile*> Tiles = GetTilesFromRect(Rect);
	for (const auto& Tile : Tiles)
	{
		if (Tile == nullptr)
		{
			return false;
		}

		if (Tile->GetRegisteredPawn().IsValid() && Tile->GetRegisteredPawn() != PawnToIgnore)
		{
			return false;
		}
	}
	
	return true;
}

bool ARZ_WorldGrid::GetSpawnRect(FRZ_IntRect& RectResult, const ARZ_WorldTile* FromTile, int32 GroupSize, ESS_Team Team) const
{
	TArray<FRZ_IntRect> RectsToCheck = RZ_IntRect::GetAllSquaresIncludingPositionFromTopLeft(
		FromTile->GetTileData().Position,
		GroupSize
	);
	
	for (const auto& Rect : RectsToCheck)
	{
		if (IsValidRect(Rect))
		{
			RectResult = Rect;
			return true;
		}
	}

	return false;
}

TArray<ASS_Pawn*> ARZ_WorldGrid::GetAllPawnsFromRect(const FRZ_IntRect& Rect)
{
	TArray<ASS_Pawn*> PawnArrayResult;

	FIntPoint Index;
	for (Index.X = Rect.Min.X; Index.X <= Rect.Max.X; Index.X++)
	{
		for (Index.Y = Rect.Min.Y; Index.Y <= Rect.Max.Y; Index.Y++)
		{
			ASS_Pawn* RegisteredPawn = GetTileFromPosition(Index)->GetRegisteredPawn().Get();
			if (RegisteredPawn && PawnArrayResult.Contains(RegisteredPawn) == false)
			{
				PawnArrayResult.Add(RegisteredPawn);
			}
		}
	}

	return PawnArrayResult;
}


ARZ_WorldTile* ARZ_WorldGrid::GetClosestTile(const ARZ_WorldTile* TargetTile, TArray<ARZ_WorldTile*> TilesToCheck) const
{
	if (TilesToCheck.Num() == 0)
		return nullptr;

	ARZ_WorldTile* ResultTile = TilesToCheck[0];
	float MinDistance = FVector::Dist(TilesToCheck[0]->GetActorLocation(), TargetTile->GetActorLocation());
	
	for (const auto& Tile : TilesToCheck)
	{
		const float NewDistance = FVector::Dist(Tile->GetActorLocation(), TargetTile->GetActorLocation());
		if (NewDistance < MinDistance)
		{
			MinDistance = NewDistance;
			ResultTile = Tile;
		}
	}
	
	return ResultTile;
}

bool ARZ_WorldGrid::GetMoveVector(FIntPoint& MoveVectorResult, const ARZ_WorldTile* StartTile, const ARZ_WorldTile* TargetTile) const
{
	if (StartTile == nullptr || TargetTile == nullptr)
		return false; // crash ?
	
	if ((StartTile->GetTileData().Position.X == TargetTile->GetTileData().Position.X &&
			StartTile->GetTileData().Position.Y == TargetTile->GetTileData().Position.Y) ||
		(StartTile->GetTileData().Position.X != TargetTile->GetTileData().Position.X &&
			StartTile->GetTileData().Position.Y != TargetTile->GetTileData().Position.Y))
	{
		return false;
	}

	MoveVectorResult.X = TargetTile->GetTileData().Position.X - StartTile->GetTileData().Position.X;
	MoveVectorResult.Y = TargetTile->GetTileData().Position.Y - StartTile->GetTileData().Position.Y;

	return true;
}

#pragma endregion

#pragma region +++++ Rendering ...

void ARZ_WorldGrid::HighlightLine(int32 PositionX, const FLinearColor& HighlightColor)
{
	for (const auto& Tile : GridTiles)
	{
		if (Tile->GetTileData().Position.X == PositionX)
		{
			Tile->EnableDirectionalHighlight(ESS_Direction::South, HighlightColor);
			Tile->EnableDirectionalHighlight(ESS_Direction::North, HighlightColor);
		}
	}
}

void ARZ_WorldGrid::HighlightColumn(int32 PositionY, const FLinearColor& HighlightColor)
{
	for (const auto& Tile : GridTiles)
	{
		if (Tile->GetTileData().Position.Y == PositionY)
		{
			Tile->EnableDirectionalHighlight(ESS_Direction::West, HighlightColor);
			Tile->EnableDirectionalHighlight(ESS_Direction::East, HighlightColor);
		}
	}
}

void ARZ_WorldGrid::HighlightRect(const FRZ_IntRect& Rect, const FLinearColor& HighlightColor, bool bSaveChanges)
{
	TArray<ARZ_WorldTile*> Tiles = GetTilesFromRect(Rect);
	for (const auto& Tile : Tiles)
	{
		if (Tile->GetTileData().Position.X == Rect.Min.X)
			Tile->EnableDirectionalHighlight(ESS_Direction::South, HighlightColor, bSaveChanges);
		if (Tile->GetTileData().Position.X == Rect.Max.X)
			Tile->EnableDirectionalHighlight(ESS_Direction::North, HighlightColor, bSaveChanges);
		if (Tile->GetTileData().Position.Y == Rect.Min.Y)
			Tile->EnableDirectionalHighlight(ESS_Direction::West, HighlightColor, bSaveChanges);
		if (Tile->GetTileData().Position.Y == Rect.Max.Y)
			Tile->EnableDirectionalHighlight(ESS_Direction::East, HighlightColor, bSaveChanges);
	}
}

void ARZ_WorldGrid::DisableAllHighlight(bool bResetToDefault)
{
	for (const auto& Tile : GridTiles)
	{
		Tile->DisableHighlight(bResetToDefault);
	}
}

#pragma endregion

#pragma region +++++ Debug ...

void ARZ_WorldGrid::DebugPoint(const FIntPoint& Point)
{
	const FString StringToPrint = "ARZ_WorldGrid::DebugIntPoint // X = " + FString::FromInt(Point.X) + " // Y = " +
		FString::FromInt(Point.Y);

	UE_LOG(LogTemp, Display, TEXT("%s"), *StringToPrint);
}

void ARZ_WorldGrid::DebugRect(const FRZ_IntRect& Rect)
{
	UE_LOG(LogTemp, Display, TEXT("-------------------------------------------------------------"));

	UE_LOG(LogTemp, Display, TEXT("IntRect : MinX = %s // MinY = %s // MaxX = %s // MaxY = %s"),
		*FString::FromInt(Rect.Min.X),
		*FString::FromInt(Rect.Min.Y),
		*FString::FromInt(Rect.Max.X),
		*FString::FromInt(Rect.Max.Y)
	);
	
	UE_LOG(LogTemp, Display, TEXT("-------------------------------------------------------------"));
}

void ARZ_WorldGrid::DebugTile(const ARZ_WorldTile* TileToDebug)
{
	FString StringToPrint = "ARZ_WorldGrid::DebugTile // " + TileToDebug->GetName() + " // ";

	if (TileToDebug == nullptr)
		StringToPrint += "nullptr";
	else
	{
		StringToPrint += "PositionX = " + FString::FromInt(TileToDebug->GetTileData().Position.X) + " // PositionY = " +
			FString::FromInt(TileToDebug->GetTileData().Position.Y);
	}

	UE_LOG(LogTemp, Display, TEXT("%s"), *StringToPrint);
}

#pragma endregion

/*
TArray<ARZ_WorldTile*> ARZ_WorldGrid::GetTilesFromSquare(const ARZ_WorldTile* FirstCornerTile, const ARZ_WorldTile* SecondCornerTile) const
{
	TArray<ARZ_WorldTile*> ResultArray;
	
	int32 XPositionMin;
	int32 XPositionMax;
	int32 YPositionMin;
	int32 YPositionMax;

	FirstCornerTile->GetTileData().PositionY <= SecondCornerTile->GetTileData().PositionY
		? YPositionMin = FirstCornerTile->GetTileData().PositionY
		: YPositionMin = SecondCornerTile->GetTileData().PositionY;

	FirstCornerTile->GetTileData().PositionY >= SecondCornerTile->GetTileData().PositionY
		? YPositionMax = FirstCornerTile->GetTileData().PositionY
		: YPositionMax = SecondCornerTile->GetTileData().PositionY;

	FirstCornerTile->GetTileData().PositionX <= SecondCornerTile->GetTileData().PositionX
		? XPositionMin = FirstCornerTile->GetTileData().PositionX
		: XPositionMin = SecondCornerTile->GetTileData().PositionX;

	FirstCornerTile->GetTileData().PositionX >= SecondCornerTile->GetTileData().PositionX
		? XPositionMax = FirstCornerTile->GetTileData().PositionX
		: XPositionMax = SecondCornerTile->GetTileData().PositionX;

	for (int32 YPosition = YPositionMin; YPosition <= YPositionMax; YPosition++)
	{
		for (int32 XPosition = XPositionMin; XPosition <= XPositionMax; XPosition++)
		{
			ResultArray.Add(GetTileFromPosition(XPosition, YPosition));
		}
	}
	
	return ResultArray;
}

void ARZ_WorldGrid::GetTileGroup(FRZ_WorldTileGroupData& GroupResult, ARZ_WorldTile* OriginTile, int32 GroupSize,
                            const ASS_Pawn* PawnToIgnore)
{
	GroupResult.bIsValid = true;
	GroupResult.bIsInSpawn = true;
	GroupResult.OriginTile = OriginTile;
	GroupResult.TileArray.Empty();
	
	if (OriginTile == nullptr)
	{
		GroupResult.bIsValid = false;
		return;
	}
	
	for (int32 XOffset = 0; XOffset < GroupSize; XOffset++)
	{
		for (int32 YOffset = 0; YOffset < GroupSize; YOffset++)
		{
			ARZ_WorldTile* TileToAdd = GetTileFromPosition(
				OriginTile->GetTileData().PositionX + XOffset,
				OriginTile->GetTileData().PositionY + YOffset
			);

			if (TileToAdd == nullptr)
			{
				GroupResult.bIsValid = false;
				return;
			}

			if (TileToAdd->GetTileData().Type != ERZ_WorldTileType::Spawn)
			{
				GroupResult.bIsInSpawn = false;
			}

			if (TileToAdd->GetRegisteredPawn() != nullptr)
			{
				if (PawnToIgnore == nullptr)
				{
					GroupResult.bIsValid = false;
					return;
				}
				
				if (TileToAdd->GetRegisteredPawn() != PawnToIgnore)
				{
					GroupResult.bIsValid = false;
					return;
				}
			}

			GroupResult.TileArray.Add(TileToAdd);
		}
	}
}

void ARZ_WorldGrid::GetSpawnTileGroupFromTile(FRZ_WorldTileGroupData& GroupResult, ARZ_WorldTile* OriginTile, int32 GroupSize,
                                         ESS_Team Team)
{
	TArray<FRZ_WorldTileGroupData> GroupResults;
	GroupResult.bIsValid = false;
	GroupResult.bIsInSpawn = false;

	const float MinYPosition = OriginTile->GetTileData().PositionY - GroupSize + 1;
	const float MinXPosition = OriginTile->GetTileData().PositionX - GroupSize + 1;
	
	for (int32 YIndex = MinYPosition; YIndex <=  OriginTile->GetTileData().PositionY; YIndex++)
	{
		for (int32 XIndex = MinXPosition; XIndex <= OriginTile->GetTileData().PositionX; XIndex++)
		{
			ARZ_WorldTile* OffsettedOriginTile = GetTileFromPosition(XIndex, YIndex);
			if (OffsettedOriginTile)
			{
				FRZ_WorldTileGroupData GroupData;
				GetTileGroup(GroupData, OffsettedOriginTile, GroupSize);
				if (GroupData.bIsValid && GroupData.bIsInSpawn)
				{
					GroupResult = GroupData;
					GroupResults.Add(GroupResult);
				}
			}
		}
	}
}

void ARZ_WorldGrid::GetValidTileGroupsFromSquare(TArray<FRZ_WorldTileGroupData>& GroupsResult, TArray<ARZ_WorldTile*> SquareTiles,
                                            int32 GroupSize)
{
	GroupsResult.Empty();
	TArray<ARZ_WorldTile*> UsedTiles;
	
	for (const auto& SquareTile : SquareTiles)
	{
		if (UsedTiles.Contains(SquareTile) == false)
		{
			FRZ_WorldTileGroupData TileGroup;
			GetTileGroup(TileGroup, SquareTile, GroupSize);
			if (TileGroup.bIsValid)
			{
				bool bContainsSquareTiles = true;
				for (const auto& GroupTile1 : TileGroup.TileArray)
				{
					if (SquareTiles.Contains(GroupTile1) == false || UsedTiles.Contains(GroupTile1))
					{
						bContainsSquareTiles = false;
					}
				}

				if (bContainsSquareTiles)
				{
					GroupsResult.Add(TileGroup);

					for (const auto& GroupTile2 : TileGroup.TileArray)
					{
						UsedTiles.Add(GroupTile2);
					}
				}
			}
		}
	}
}
*/

