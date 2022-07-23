/// RemzDNB

#include "RZ_Common.h"

#define LOCTEXT_NAMESPACE "FRZ_CommonModule"

void FRZ_CommonModule::StartupModule()
{
}

void FRZ_CommonModule::ShutdownModule()
{
}

void FRZ_CommonModule::RegisterNativeGameplayTags()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRZ_CommonModule, RZ_Common)

#pragma region +++ Utility library ...

RZ_CommonLibrary::RZ_CommonLibrary()
{
}

TArray<AActor*> RZ_CommonLibrary::OrderActorsByDistance(TArray<AActor*> ActorsToOrder, FVector TargetLocation)
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

AActor* RZ_CommonLibrary::GetClosestActorFromLocation(TArray<AActor*> ActorArray, FVector TargetLocation)
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

bool RZ_CommonLibrary::CompareFloats(float Value1, float Value2, float Acceptance)
{
	RZ_CommonLibrary::PrintFloatToScreen("RZ_CommonLibrary::CompareFloat : ", Value1 - Value2, FColor::Orange, -1, 3.0f);
	
	if (FMath::Abs(Value1) - FMath::Abs(Value2) <= Acceptance)
		return true;

	return false;
}

void RZ_CommonLibrary::PrintStringToScreen(const FString& PrefixString, const FString& InputString, FColor Color, int32 Key, float TimeToDisplay)
{
	const FString StringToPrint = PrefixString + InputString;

	GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, Color, StringToPrint);
}

void RZ_CommonLibrary::PrintFloatToScreen(const FString& PrefixString, float InputFloat, FColor Color, int32 Key, float TimeToDisplay)
{
	const FString StringToPrint = PrefixString + FString::SanitizeFloat(InputFloat, 4);

	GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, Color, StringToPrint);
}

void RZ_CommonLibrary::PrintVectorToScreen(const FString& PrefixString, FVector InputVector, FColor Color, int32 Key, float TimeToDisplay)
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

void RZ_CommonLibrary::PrintBoolToScreen(bool bBoolToPrint, FString PrefixString, float TimeToDisplay)
{
	FString StringToPrint;
	if (bBoolToPrint)
		StringToPrint = "true";
	else
		StringToPrint = "false";

	GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, FColor::Red, PrefixString + FString::Printf(TEXT(" : %s"), *StringToPrint));
}

void RZ_CommonLibrary::PrintRotatorToScreen(FRotator RotatorToPrint, FString PrefixString, int32 Key, float TimeToDisplay)
{
	const FString StringToPrint = PrefixString + " / Pitch : " + FString::SanitizeFloat(RotatorToPrint.Pitch) + " / Yaw : " + FString::SanitizeFloat(RotatorToPrint.Yaw) + " / Roll : " + FString::SanitizeFloat(RotatorToPrint.Roll);
	GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, FColor::Magenta, StringToPrint);
}

#pragma endregion