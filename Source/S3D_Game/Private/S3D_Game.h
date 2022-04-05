#pragma once

#include "CoreMinimal.h"
#include "S3D_Game.generated.h"

#define S3DCHARACTERVIEWHEIGHT 155.0f

UCLASS()
class US3D_GameSettings : public UDataAsset
{
	GENERATED_BODY()

public:

	US3D_GameSettings();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> HUDLayoutWidget;
};