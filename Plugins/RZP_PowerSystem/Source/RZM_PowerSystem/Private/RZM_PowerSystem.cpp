#include "RZM_PowerSystem.h"

#include "RZ_PowerComponent.h"

#define LOCTEXT_NAMESPACE "FRZM_PowerSystemModule"

void FRZM_PowerSystemModule::StartupModule()
{
}

void FRZM_PowerSystemModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRZM_PowerSystemModule, RZM_PowerSystem)


FRZ_PowerGridInfo::FRZ_PowerGridInfo()
{
	GridID = -1;
	ProducedPower = 0.0f;
	ConsumedPower = 0.0f;
}

void FRZ_PowerGridInfo::UpdateTotalGridPower()
{
	ProducedPower = 0.0f;
	ConsumedPower = 0.0f;
	
	for (const auto& AttachedPowerComponent : AttachedPowerComponents)
	{
		ProducedPower += AttachedPowerComponent->GetPowerComponentSettings().MaxProducedPower;
		ConsumedPower += AttachedPowerComponent->GetPowerComponentSettings().MaxConsumedPower;
	}
}
