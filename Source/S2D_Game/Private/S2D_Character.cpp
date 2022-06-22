// RemzDNB

#include "S2D_Character.h"
#include "S2D_BuildComponent.h"
#include "S2D_BuildableInterface.h"
//
#include "RZ_InventoryComponent.h"

AS2D_Character::AS2D_Character(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BuildComp = CreateDefaultSubobject<US2D_BuildComponent>(FName("BuildComp"));
}

void AS2D_Character::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InventoryCT->OnItemSelected.AddUniqueDynamic(this, &AS2D_Character::OnInventoryItemSelected);
}

void AS2D_Character::BeginPlay()
{
	Super::BeginPlay();
}

void AS2D_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BuildComp->PlayerTargetLocation = PlayerTargetLocation;
}

void AS2D_Character::OnInventoryItemSelected(AActor* SelectedItem)
{
	Super::OnInventoryItemSelected(SelectedItem);

	RZ_UtilityLibrary::PrintStringToScreen("AS2D_Character::OnInventoryItemSelected", "", FColor::Red, -1, 3.0f);
	
	BuildComp->StopBuilding();
	BuildComp->StartBuilding(SelectedItem);
}