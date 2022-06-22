/// RemzDNB
///
/// RZ_GameSettings.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RZ_GameSettings.generated.h"

UCLASS()
class RZ_GAME_API URZ_GameSettings : public UDataAsset
{
	GENERATED_BODY()

public:

	URZ_GameSettings();
	
	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	class UDataTable* ControlSettingsPresets;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	uint8 AISpawnCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	uint8 AIWaveDelay;
	
	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
	UMaterialInterface* ItemSpawnMaterial_Valid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
	UMaterialInterface* ItemSpawnMaterial_Invalid;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	TSubclassOf<ACharacter> DefaultCharacterClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	TSubclassOf<AActor> DefaultSpawnManagerDemoActor;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	class UParticleSystem* SpawnParticle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character")
	UMaterial* TargetSplineMeshMaterial;

	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items")
	TArray<FName> DefaultItems;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* CharacterBehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	class UBehaviorTree* TurretBehaviorTree;
	
	//

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> DamageMarkerWidgetClass;

	//

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

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDebugGameState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDebugPlayerController;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
	bool bDebugPerceptionComponent;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grid")
	TSubclassOf<AActor> GridManagerClass;
};
