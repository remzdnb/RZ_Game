/// RemzDNB
///
/// RZ_GameSettings.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Engine/DataAsset.h"
#include "RZM_BuildingSystem.h"
#include "RZM_PowerSystem.h"
#include "RZ_GameSettings.generated.h"

UCLASS()
class RZ_GAME_API URZ_GameSettings : public UDataAsset
{
	GENERATED_BODY()

public:

	URZ_GameSettings();
	
	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plugins")
	TSubclassOf<ARZ_BuildingManager> BuildingManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Plugins")
	TSubclassOf<ARZ_PowerManager> PowerManagerClass;
	
	//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	class UDataTable* ControlSettingsPresets;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	uint8 AISpawnCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game")
	uint8 AIWaveDelay;

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
	TSubclassOf<UUserWidget> LoadoutHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> LoadoutMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> ItemButton_HUD_WidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> ItemButton_Menu_WidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> ActorManager_MainWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> ActorManager_ButtonWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> ActorManager_GridWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> ActorManager_ActorWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> DamageMarkerWidgetClass;

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
