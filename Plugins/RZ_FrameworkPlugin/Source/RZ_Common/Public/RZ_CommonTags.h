#pragma once

#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

struct RZ_COMMON_API FRZ_CommonTags : public FGameplayTagNativeAdder
{
	FGameplayTag GameActor;
	FGameplayTag GameActor_Enabled;
	FGameplayTag NewTestMdr;
 
	// Static accessor for accessing the tags. Access tags using:
	// FRZ_CommonTags::Get().Damage_Fire for example.
	FORCEINLINE static const FRZ_CommonTags& Get() { return CommonTags; }
 
protected:
	
	// Called on editor startup to register and assign the native tags.
	virtual void AddTags() override
	{
		UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
 
		GameActor = Manager.AddNativeGameplayTag(TEXT("GameActor"));
		GameActor_Enabled = Manager.AddNativeGameplayTag(TEXT("GameActor.Enabled"));
		NewTestMdr = Manager.AddNativeGameplayTag(TEXT("NewTestMdr"));
	}

private:
	
	//Private static object for the global tags. Use the Get() function to access externally.
	static FRZ_CommonTags CommonTags;
};