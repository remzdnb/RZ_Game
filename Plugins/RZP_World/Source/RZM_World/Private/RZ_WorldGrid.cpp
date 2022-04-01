#include "RZ_WorldGrid.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

ARZ_WorldGrid::ARZ_WorldGrid()
{
	RootSceneComp = CreateDefaultSubobject<USceneComponent>(FName("RootSceneComp"));
	RootComponent = RootSceneComp;

	InstancedStaticMeshComp = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("InstancedStaticMeshComp"));
	InstancedStaticMeshComp->SetupAttachment(RootComponent);
	InstancedStaticMeshComp->SetMobility(EComponentMobility::Static);
	InstancedStaticMeshComp->SetCollisionProfileName("Ground");

	DirectionArrowComp = CreateDefaultSubobject<UArrowComponent>(FName("DirectionArrowComp"));
	DirectionArrowComp->SetupAttachment(RootComponent);

	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bCanEverTick = false;
}

void ARZ_WorldGrid::RegenerateGrid()
{
	InstancedStaticMeshComp->ClearInstances();
	
	FVector SpawnLocation;
	FTransform SpawnTransform;

	// Grid tiles
	
	for (int32 XIndex = 0; XIndex < GridSize.X; XIndex++)
	{
		for (int32 YIndex = 0; YIndex < GridSize.Y; YIndex++)
		{
			SpawnLocation = FVector(XIndex * TileSize, YIndex * TileSize, GetActorLocation().Z);
			SpawnTransform = FTransform(GetActorRotation(), SpawnLocation, FVector(1.0f));
			SpawnTile(nullptr, SpawnTransform, XIndex, YIndex, TileSize / 2, TileSize / 2, ERZ_WorldTileType::Grid);
		}
	}

	// Border tiles

	if (!bSpawnBorderTiles) { return; }
	
	for (int32 YIndex = 0; YIndex < GridSize.Y; YIndex++)
	{
		// South
		SpawnLocation = FVector(BorderTileLength * -1 - TileSize / 2, YIndex * TileSize, 0.0f);
		SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnLocation, FVector(1.0f));
		SpawnTile(nullptr, SpawnTransform, -1, YIndex, BorderTileLength, TileSize / 2, ERZ_WorldTileType::Border);

		// North
		SpawnLocation = FVector(GridSize.X * TileSize + BorderTileLength - TileSize / 2, YIndex * TileSize, 0.0f);
		SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnLocation, FVector(1.0f));
		SpawnTile(nullptr, SpawnTransform, GridSize.X, YIndex, BorderTileLength, TileSize / 2, ERZ_WorldTileType::Border);
	}

	for (int32 XIndex = 0; XIndex < GridSize.X; XIndex++)
	{
		// West
		SpawnLocation = FVector(XIndex * TileSize, BorderTileLength * -1 - TileSize / 2, 0.0f);
		SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnLocation, FVector(1.0f));
		SpawnTile(nullptr, SpawnTransform, XIndex, -1, TileSize / 2, BorderTileLength, ERZ_WorldTileType::Border);

		// East
		SpawnLocation = FVector(XIndex * TileSize, GridSize.Y * TileSize + BorderTileLength - TileSize / 2, 0.0f);
		SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnLocation, FVector(1.0f));
		SpawnTile(nullptr, SpawnTransform, XIndex, GridSize.Y, TileSize / 2, BorderTileLength, ERZ_WorldTileType::Border);
	}
	
	// SouthWest corner
	SpawnLocation = FVector(BorderTileLength * -1 - TileSize / 2, BorderTileLength * -1 - TileSize / 2, 0.0f);
	SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnLocation, FVector(1.0f));
	SpawnTile(nullptr, SpawnTransform, -1, -1, BorderTileLength, BorderTileLength, ERZ_WorldTileType::Border);

	// SouthEast corner
	SpawnLocation = FVector(BorderTileLength * -1 - TileSize / 2, GridSize.Y * TileSize + BorderTileLength - TileSize / 2, 0.0f);
	SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnLocation, FVector(1.0f));
	SpawnTile(nullptr, SpawnTransform, -1, GridSize.Y, BorderTileLength, BorderTileLength, ERZ_WorldTileType::Border);

	// NorthWest corner
	SpawnLocation = FVector(GridSize.X * TileSize + BorderTileLength - TileSize / 2, BorderTileLength * -1 - TileSize / 2, 0.0f);
	SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnLocation, FVector(1.0f));
	SpawnTile(nullptr, SpawnTransform, GridSize.X, -1, BorderTileLength, BorderTileLength, ERZ_WorldTileType::Border);

	// NorthEast corner
	SpawnLocation = FVector(GridSize.X * TileSize + BorderTileLength - TileSize / 2, GridSize.Y * TileSize + BorderTileLength - TileSize / 2, 0.0f);
	SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnLocation, FVector(1.0f));
	SpawnTile(nullptr, SpawnTransform, GridSize.X, GridSize.Y, BorderTileLength, BorderTileLength, ERZ_WorldTileType::Border);
}

void ARZ_WorldGrid::SpawnTile(TSubclassOf<AActor> TileClass, const FTransform& SpawnTransform, int32 PositionX,
	int32 PositionY, float SizeX, float SizeY, ERZ_WorldTileType TileType)
{
	if (!InstancedStaticMeshComp) { return; }

	InstancedStaticMeshComp->AddInstance(SpawnTransform);
}

/*
void ARZ_WorldGrid::SpawnTile(TSubclassOf<AActor> TileClass, const FTransform& SpawnTransform, int32 PositionX,
                         int32 PositionY, float SizeX, float SizeY, ERZ_WorldTileType TileType)
{
	ARZ_WorldTile* NewTile = GetWorld()->SpawnActorDeferred<ARZ_WorldTile>(
		TileClass,
		SpawnTransform,
		this,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
	);
	if (NewTile)
	{
		FRZ_WorldTileData NewTileData;
		NewTileData.Position.X = PositionX;
		NewTileData.Position.Y = PositionY;
		NewTileData.Type = TileType;
		NewTile->Init(NewTileData);
		NewTile->GetCollisionBox()->SetBoxExtent(FVector(SizeX, SizeY, 1.0f));
		UGameplayStatics::FinishSpawningActor(NewTile, SpawnTransform);
	}

	if (TileType == ERZ_WorldTileType::Grid)
		GridTiles.Add(NewTile);
}


void ARZ_WorldGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetActorLocation(FVector::ZeroVector);
	SetActorRotation(FRotator::ZeroRotator);

	KingStartPosition.X = FMath::Clamp(KingStartPosition.X, 0, GridSize.X - 1);
	KingStartPosition.Y = FMath::Clamp(KingStartPosition.Y, 0, GridSize.Y - 1);
}

void ARZ_WorldGrid::BeginPlay()
{
	Super::BeginPlay();

	GInstance = Cast<USS_GameInstance>(GetGameInstance());
	GState = Cast<ASS_GameState>(GetWorld()->GetGameState());
	GState->OnGamePhaseUpdatedEvent.AddUniqueDynamic(this, &ARZ_WorldGrid::OnGamePhaseUpdated);

	const FRZ_IntRect SouthKingStartRect = FRZ_IntRect(KingStartPosition, KingStartPosition);
	const FRZ_IntRect NorthKingStartRect = FRZ_IntRect(
		FIntPoint(GridSize.X - KingStartPosition.X - 1, GridSize.Y - KingStartPosition.Y - 1),
		FIntPoint(GridSize.X - KingStartPosition.X - 1, GridSize.Y - KingStartPosition.Y - 1)
	);
	SouthTeamKing = SpawnPawn(SouthKingStartRect, "King", ESS_Team::South);
	NorthTeamKing = SpawnPawn(NorthKingStartRect, "King", ESS_Team::North);
	SouthTeamKing->OnPawnMoveCompletedEvent.AddUniqueDynamic(this, &ARZ_WorldGrid::UpdateTilesOwnership);
	NorthTeamKing->OnPawnMoveCompletedEvent.AddUniqueDynamic(this, &ARZ_WorldGrid::UpdateTilesOwnership);
	UpdateTilesOwnership();
}

ASS_Pawn* ARZ_WorldGrid::SpawnPawn(const FRZ_IntRect& SpawnRect, const FName& PawnDataRowName, ESS_Team Team,
                              bool bIsDemoPawn)
{
	if (!bIsDemoPawn && GetLocalRole() != ROLE_Authority)
		return nullptr;
	
	const FSS_PawnData* const PawnData = GInstance->GetPawnDataFromRow(PawnDataRowName);
	if (PawnData == nullptr)
		return nullptr;
	
	const FVector SpawnLocation = GetTileFromPosition(SpawnRect.Min)->GetActorLocation();
	const FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnLocation, FVector(1.0f));
	
	ASS_Pawn* NewPawn = GetWorld()->SpawnActorDeferred<ASS_Pawn>(
		PawnData->Class,
		SpawnTransform,
		this,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	if (NewPawn)
	{
		NewPawn->Init_Multicast(
			PawnDataRowName,
			SpawnRect,
			Team,
			bIsDemoPawn,
			true
		);

		if (!bIsDemoPawn)
		{
			RegisterPawnToGrid(NewPawn, SpawnRect);
			PawnArray.Add(NewPawn);
		}
		
		UGameplayStatics::FinishSpawningActor(NewPawn, SpawnTransform);
	}

	return NewPawn;
}

bool ARZ_WorldGrid::RequestPawnMovement(ASS_Pawn* PawnToMove, const FIntPoint& MoveVector)
{
	FRZ_IntRect TargetMoveRect = PawnToMove->GridRect;

	if (MoveVector.X > 0)
	{
		TargetMoveRect.Min.X += 1;
		TargetMoveRect.Max.X += 1;
	}
	else if (MoveVector.X < 0)
	{
		TargetMoveRect.Min.X -= 1;
		TargetMoveRect.Max.X -= 1;
	}
	else if (MoveVector.Y > 0)
	{
		TargetMoveRect.Min.Y += 1;
		TargetMoveRect.Max.Y += 1;
	}
	else if (MoveVector.Y < 0)
	{
		TargetMoveRect.Min.Y -= 1;
		TargetMoveRect.Max.Y -= 1;
	}
	else
	{
		return false;
	}

	if (IsValidRect(TargetMoveRect, PawnToMove))
	{
		UnregisterPawnFromGrid(PawnToMove);
		RegisterPawnToGrid(PawnToMove, TargetMoveRect);
		PawnToMove->GridRect = TargetMoveRect;
		PawnToMove->StartMoveToTileGroup();
		return true;
	}

	return false;
	
}

void ARZ_WorldGrid::RegisterPawnToGrid(ASS_Pawn* PawnToRegister, const FRZ_IntRect& PawnRect)
{
	for (const auto& Tile : GetTilesFromRect(PawnRect))
	{
		Tile->RegisterPawnBPI(PawnToRegister);
	}
}

void ARZ_WorldGrid::UnregisterPawnFromGrid(const ASS_Pawn* PawnToUnregister)
{
	for (const auto& Tile : GetTilesFromRect(PawnToUnregister->GridRect))
	{
		Tile->UnregisterPawnBPI();
	}
}

void ARZ_WorldGrid::OnGamePhaseUpdated(ESS_GamePhase NewGamePhase)
{
	if (NewGamePhase == ESS_GamePhase::Strategic)
	{
		UpdateTilesOwnership();
	}
}

void ARZ_WorldGrid::UpdateTilesOwnership()
{
	for (const auto& Tile : GridTiles)
	{
		if (Tile->GetTileData().Position.X <= SouthTeamKing->GridRect.Min.X &&
			Tile->GetTileData().Position.X < NorthTeamKing->GridRect.Min.X)
		{
			Tile->SetOwningTeam(ESS_Team::South);
		}
		else if (Tile->GetTileData().Position.X >= NorthTeamKing->GridRect.Min.X &&
			Tile->GetTileData().Position.X > SouthTeamKing->GridRect.Min.X)
		{
			Tile->SetOwningTeam(ESS_Team::North);
		}
		else
		{
			Tile->SetOwningTeam(ESS_Team::Neutral);
		}
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
