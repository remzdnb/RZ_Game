#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RZ_GameSettings.generated.h"

#define TOPDOWNPROJECTILEPLANEHEIGHT 150.0f

class URZ_PawnManagerPluginSettings;
class URZ_ItemManagerModuleSettings;

UCLASS()
class RZ_GAME_API URZ_GameSettings : public UDataAsset
{
	GENERATED_BODY()

public:

	URZ_GameSettings();
	
	///

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	class UDataTable* ControlSettingsPresets;

	///
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	class UParticleSystem* SpawnParticle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	UMaterial* TargetSplineMeshMaterial;

	///

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* CharacterBehaviorTree;
	
	///

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> LoadoutHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> LoadoutMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> ItemButton_HUD_WidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> ItemButton_Menu_WidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> DevWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> HomeWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> CharacterEditionMainWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> CrosshairWidgetClass;

	///

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Misc")
	UStaticMesh* EngineCubeMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Misc")
	UStaticMesh* EngineSphereMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Misc")
	UStaticMesh* EnginePlaneMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Misc")
	UStaticMesh* EngineCylinderMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Misc")
	UStaticMesh* EngineConeMesh;



};
