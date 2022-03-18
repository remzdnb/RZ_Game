#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RZ_UIManagerPluginSettings.generated.h"

UCLASS()
class URZ_UIManagerPluginSettings : public UDataAsset
{
	GENERATED_BODY()

public:

	URZ_UIManagerPluginSettings();

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class URZ_MenuLayoutWidget> MenuLayoutWBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> MenuLayoutButtonWBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> ExitWBP;
	
	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDebugUIManager;
};
