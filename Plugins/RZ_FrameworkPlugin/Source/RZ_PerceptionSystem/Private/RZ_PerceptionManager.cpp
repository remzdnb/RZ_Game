#include "RZ_PerceptionManager.h"
//
#include "AbilitySystemComponent.h"
//
#include "EngineUtils.h"
#include "GameplayTagAssetInterface.h"

ARZ_PerceptionManager::ARZ_PerceptionManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ARZ_PerceptionManager::BeginPlay()
{
	Super::BeginPlay();
}

void ARZ_PerceptionManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RegisteredPawns.Empty();
	
	for (TActorIterator<APawn> Pawn(GetWorld()); Pawn; ++Pawn)
	{
		const UAbilitySystemComponent* ASComp = Cast<UAbilitySystemComponent>(
			Pawn->GetComponentByClass(UAbilitySystemComponent::StaticClass())
		);
		if (ASComp)
		{
			if (ASComp->HasMatchingGameplayTag(FRZ_CommonTags::Get().GameActor_Enabled))
			{
				RegisteredPawns.Add(*Pawn);
			}
		}
	}

	Debug(DeltaTime);
}

void ARZ_PerceptionManager::Debug(float DeltaTime)
{
	/*if (!bDebug) { return; }
	
	const FString SeparatorString = " ------------------------------------------------------------- ";
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, SeparatorString);;
	
	for (const auto& Grid : PowerGrids)
	{
		FString GridString = "Active Grid - ID == " + FString::FromInt(Grid.GridID);
		for (const auto& AttachedPowerComponent : Grid.AttachedPowerComponents)
		{
			GridString += " // Attached == " + AttachedPowerComponent->GetOwner()->GetName();
		}

		GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, GridString);;
	}
	
	const FString FinalString = " ------------------- ARZ_PerceptionManager::Debug ------------------- ";
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Blue, FinalString);;*/
}


