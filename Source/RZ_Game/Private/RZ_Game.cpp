/// RemzDNB

#include "RZ_Game.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, RZ_Game, "RZ_Game" );

IRZ_PawnInterface::IRZ_PawnInterface()
{
	TeamID = 0;
	PawnOwnerShip = ERZ_PawnOwnership::WaveAI;
}

uint8 IRZ_PawnInterface::GetTeamID() const
{
	return TeamID;
}

ERZ_PawnOwnership IRZ_PawnInterface::GetPawnOwnership() const
{
	return PawnOwnerShip;
}

void IRZ_PawnInterface::SetTeamID(uint8 NewTeamID)
{
	TeamID = NewTeamID;
}

void IRZ_PawnInterface::SetPawnOwnerShip(ERZ_PawnOwnership NewPawnOwnership)
{
	PawnOwnerShip = NewPawnOwnership;
}

void IRZ_PawnInterface::SetAssignedTarget(AActor* NewAssignedTarget)
{
	AssignedTarget = NewAssignedTarget;
}

ERZ_AIAttitude IRZ_PawnInterface::GetAIAttitude(
	const IRZ_PawnInterface* OriginActorInterface,
	const IRZ_PawnInterface* TargetActorInterface
)
{
	if (OriginActorInterface->GetTeamID() != TargetActorInterface->GetTeamID())
	{
		return ERZ_AIAttitude::Aggressive;
	}
	else
	{
		return ERZ_AIAttitude::Neutral;
	}
}
