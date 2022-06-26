/// RemzDNB

#include "RZM_Shared.h"

#pragma region +++ Module setup ...

#define LOCTEXT_NAMESPACE "FRZM_SharedModule"

void FRZM_SharedModule::StartupModule()
{
}

void FRZM_SharedModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRZM_SharedModule, RZM_Shared)

#pragma endregion

#pragma region +++ Types / Settings ...

URZ_SharedModuleSettings::URZ_SharedModuleSettings()
{
}

const FRZ_ActorSettings* URZ_SharedModuleSettings::GetActorSettingsFromTableRow(const FName& RowName) const
{
	if (ActorSettingsDataTable == nullptr || RowName == "Empty")
		return nullptr;

	const FString ContextString;
	const FRZ_ActorSettings* ItemData = ActorSettingsDataTable->FindRow<FRZ_ActorSettings>(RowName, ContextString);
	if (ItemData)
	{
		return ItemData;
	}

	UE_LOG(LogTemp, Error, TEXT("URZ_SharedModuleSettings::GetActorSettingsFromTableRow - Row not found : %s"), *RowName.ToString());
	return nullptr;
}

#pragma endregion

#pragma region +++ ItemInterface ...

IRZ_ActorInterface::IRZ_ActorInterface()
{

}

void IRZ_ActorInterface::InitActorSettings(const UWorld* World, const FName& TableRowName)
{
	IRZ_SharedModuleInterface* SharedModuleInterface =
		Cast<IRZ_SharedModuleInterface>(World->GetGameInstance());

	if (!SharedModuleInterface) { return; }
	if (!SharedModuleInterface->GetSharedModuleSettings()) { return; }
	if (!SharedModuleInterface->GetSharedModuleSettings()->GetActorSettingsFromTableRow(TableRowName)) { return; }
	
	ActorSettings = *SharedModuleInterface->GetSharedModuleSettings()->GetActorSettingsFromTableRow(TableRowName);
}

void IRZ_ActorInterface::SetWantToUse(bool bNewWantsTouse, ERZ_UseType UseType)
{
}

void IRZ_ActorInterface::OnHoverStart()
{
}

void IRZ_ActorInterface::OnHoverEnd()
{
}

#pragma endregion

#pragma region +++ Utility library ...

RZ_UtilityLibrary::RZ_UtilityLibrary()
{
}

TArray<AActor*> RZ_UtilityLibrary::OrderActorsByDistance(TArray<AActor*> ActorsToOrder, FVector TargetLocation)
{
	TArray<AActor*> TempArray = ActorsToOrder;
	TArray<AActor*> ResultArray;

	for (int32 Index = 0; Index < ActorsToOrder.Num(); Index++)
	{
		AActor* ClosestActor = GetClosestActorFromLocation(TempArray, TargetLocation);
		ResultArray.Add(ClosestActor);
		TempArray.Remove(ClosestActor);
	}
	
	return ResultArray;
}

AActor* RZ_UtilityLibrary::GetClosestActorFromLocation(TArray<AActor*> ActorArray, FVector TargetLocation)
{
	float MinDistance = 999999999999999999.0f;
	AActor* ResultActor = nullptr;

	for (const auto& Actor : ActorArray)
	{
		if (Actor)
		{
			const float NewDistance = FVector::Dist(Actor->GetActorLocation(), TargetLocation);
			if (NewDistance < MinDistance)
			{
				MinDistance = NewDistance;
				ResultActor = Actor;
			}
		}
	}
	
	return ResultActor;
}

bool RZ_UtilityLibrary::CompareFloats(float Value1, float Value2, float Acceptance)
{
	RZ_UtilityLibrary::PrintFloatToScreen("RZ_UtilityLibrary::CompareFloat : ", Value1 - Value2, FColor::Orange, -1, 3.0f);
	
	if (FMath::Abs(Value1) - FMath::Abs(Value2) <= Acceptance)
		return true;

	return false;
}

void RZ_UtilityLibrary::PrintStringToScreen(const FString& PrefixString, const FString& InputString, FColor Color, int32 Key, float TimeToDisplay)
{
	const FString StringToPrint = PrefixString + InputString;

	GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, Color, StringToPrint);
}

void RZ_UtilityLibrary::PrintFloatToScreen(const FString& PrefixString, float InputFloat, FColor Color, int32 Key, float TimeToDisplay)
{
	const FString StringToPrint = PrefixString + FString::SanitizeFloat(InputFloat, 4);

	GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, Color, StringToPrint);
}

void RZ_UtilityLibrary::PrintVectorToScreen(const FString& PrefixString, FVector InputVector, FColor Color, int32 Key, float TimeToDisplay)
{
	const FString StringToPrint = PrefixString
		+ "X : " + FString::SanitizeFloat(InputVector.X, 4)
		+ "Y : " + FString::SanitizeFloat(InputVector.Y, 4)
		+ "Z : " + FString::SanitizeFloat(InputVector.Z, 4);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, Color, StringToPrint);
	}
}

void RZ_UtilityLibrary::PrintBoolToScreen(bool bBoolToPrint, FString PrefixString, float TimeToDisplay)
{
	FString StringToPrint;
	if (bBoolToPrint)
		StringToPrint = "true";
	else
		StringToPrint = "false";

	GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Red, PrefixString + FString::Printf(TEXT(" : %s"), *StringToPrint));
}

void RZ_UtilityLibrary::PrintRotatorToScreen(FRotator RotatorToPrint, FString PrefixString, int32 Key, float TimeToDisplay)
{
	const FString StringToPrint = PrefixString + " / Pitch : " + FString::SanitizeFloat(RotatorToPrint.Pitch) + " / Yaw : " + FString::SanitizeFloat(RotatorToPrint.Yaw) + " / Roll : " + FString::SanitizeFloat(RotatorToPrint.Roll);
	GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, FColor::Magenta, StringToPrint);
}

#pragma endregion