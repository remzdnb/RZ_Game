/// RemzDNB
///
///	Base PlayerController class.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
#include "RZM_Shared.h"
#include "RZM_InventorySystem.h"
//
#include "GameFramework/PlayerController.h"
#include "Pawn/RZ_PawnCombatComponent.h"
#include "RZ_PlayerController.generated.h"

class ARZ_GameMode;
class ARZ_GameState;
class URZ_GameSettings;
class ARZ_WorldSettings;
class ARZ_CameraManager;
class ARZ_UIManager;
class ARZ_Character;
//
class URZ_InventoryMenuWidget;
class URZ_InventoryHUDWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerControllerModeUpdatedDelegate, ERZ_PlayerControllerMode, NewMode);

UCLASS()
class RZ_GAME_API ARZ_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ARZ_PlayerController();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnRep_Pawn() override;

	//
	
	void UpdateControlSettings(const FName& NewPresetName);



	void ToggleSpawnMode(bool bNewIsEnabled, AActor* DemoActor = nullptr);

	//

	FPlayerControllerModeUpdatedDelegate OnPlayerControllerModeUpdated;
	
private:
	
	UFUNCTION()
	void OnCharacterDamaged(const FRZ_DamageInfo& DamageInfo);

	UFUNCTION()
	void OnCharacterEquippedItem(AActor* EquippedItem);

	UFUNCTION()
	void Debug(float DeltaTime);

	//
	
	ARZ_GameMode* GameMode;
	ARZ_GameState* GameState;
	URZ_GameSettings* GameSettings;
	ARZ_WorldSettings* WorldSettings;
	
	TWeakObjectPtr<ARZ_CameraManager> CameraManager;
	TWeakObjectPtr<ARZ_Character> PossessedCharacter;

	FName ControlSettingsPresetName;
	FRZ_ControlSettings ControlSettings;
	ERZ_PlayerControllerMode PlayerControllerMode;
	
	/// Player traces
	/// Networking WIP.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
private:

	UFUNCTION()
	void UpdateCursorTraces(float DeltaTime);
	
	UFUNCTION()
	void UpdateScreenCenterTraces(float DeltaTime);

	UFUNCTION()
	void UpdateHoveredItem(float DeltaTime);

	UFUNCTION()
	void SetTargetLocation(const FVector& NewTargetLocation);

	UFUNCTION(Server, Reliable)
	void SetTargetLocation_Server(const FVector& NewTargetLocation);
	
	//

	IRZ_ItemInterface* LastHoveredItemInterface;

	FHitResult CursorToWorldHitResult;
	FHitResult CursorToGroundHitResult;
	FHitResult CursorToViewPlaneHitResult;
	
	UPROPERTY()
	FVector TargetLocation;

	/// UI
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////	


public:
	
	UFUNCTION(Client, Reliable)
	void OnDamageDealt_Client(float Amount, const FVector& Location);
	
private:
	
	ARZ_UIManager* UIManager;
	URZ_InventoryMenuWidget* InventoryMenuWidget;
	URZ_InventoryHUDWidget* InventoryHUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FName, TSubclassOf<UUserWidget>> MenuWidgets; // ?
	
	/// Input
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

public:
	
	virtual void SetupInputComponent() override;

protected:
	
	void LookUpAxis(float AxisValue);
	void LookRightAxis(float AxisValue);
	void OnLookUpKeyPressed();
	void OnLookDownKeyPressed();
	void OnLookRightKeyPressed();
	void OnLookLeftKeyPressed();
	void OnZoomInKeyPressed();
	void OnZoomOutKeyPressed();
	
	//
	
	void MoveForwardAxis(float AxisValue);
	void MoveRightAxis(float AxisValue);
	void OnRunKeyPressed();
	void OnRunKeyReleased();
	void OnJumpKeyPressed();

	//
	
	void OnLeftMouseButtonPressed();
	void OnLeftMouseButtonReleased();
	void OnRightMouseButtonPressed();
	void OnRightMouseButtonReleased();
	void OnMiddleMouseButtonPressed();
	void OnMiddleMouseButtonReleased();
	void OnUseKeyPressed();

	//
	
	void OnToggleMenuKeyPressed();
	void OnQuickSlot1KeyPressed();
	void OnQuickSlot2KeyPressed();
	void OnQuickSlot3KeyPressed();
	void OnQuickSlot4KeyPressed();
	void OnQuickSlot5KeyPressed();
	void OnQuickSlot6KeyPressed();
	void OnQuickBarUpKeyPressed();
	void OnQuickBarDownKeyPressed();

	//


	void OnToggleSpawnModeKeyPressed();
	void OnToggleSpawnModeKeyReleased();

	/// Custom console commands
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:

	UFUNCTION(Exec)
	void SpawnAIController();

	UFUNCTION(Exec)
	void AddInventoryItem(const FName& ItemName);
};

