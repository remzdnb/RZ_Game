/// RemzDNB

#include "RZ_Game.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, RZ_Game, "RZ_Game" );

IRZ_PawnInterface::IRZ_PawnInterface()
{
}

void IRZ_PawnInterface::SetAssignedTarget(AActor* NewAssignedTarget)
{
	AssignedTarget = NewAssignedTarget;
}
