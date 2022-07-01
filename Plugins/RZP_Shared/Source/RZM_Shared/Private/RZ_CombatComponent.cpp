/// RemzDNB

#include "RZ_CombatComponent.h"
// Engine
#include "Net/UnrealNetwork.h"

URZ_CombatComponent::URZ_CombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	TeamID = 1;
}

void URZ_CombatComponent::BeginPlay()
{
	Super::BeginPlay();
}