/// RemzDNB
///
///	Base PlayerController class.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RZ_Game.h"
//
#include "GameFramework/PlayerController.h"
#include "UI/RZ_HUDLayoutWidget.h"
#include "RZ_PlayerController.generated.h"

#define TRACEMAXLENGTH 50000.0f

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewCharacterSelectedDelegate, ARZ_Character*, NewPossessedCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNewTargetPawnSelectedDelegate, APawn*, NewSelectedPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FControllerInteractionModeUpdatedDelegate, ERZ_ControllerInteractionMode, NewMode);

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

	FNewCharacterSelectedDelegate OnNewCharacterSelected;
	FNewTargetPawnSelectedDelegate OnNewTargetPawnSelected;
	FControllerInteractionModeUpdatedDelegate OnControllerInteractionModeUpdated;

private:
	
	TWeakObjectPtr<ARZ_Character> SelectedCharacter;
	TWeakObjectPtr<APawn> SelectedTargetPawn;

	FName ControlSettingsPresetName;
	FRZ_ControlSettings ControlSettings;
	FInputModeGameAndUI DefaultInputMode;
	ERZ_ControllerInteractionMode InteractionMode;

	ARZ_GameMode* GameMode;
	ARZ_GameState* GameState;
	URZ_GameSettings* GameSettings;
	ARZ_WorldSettings* WorldSettings;
	ARZ_CameraManager* CameraManager;

public:

	UFUNCTION()
	void SetSelectedCharacter(ARZ_Character* CharacterToSelect);

	UFUNCTION()
	void SetSelectedTargetPawn(APawn* TargetPawnToSelect);
	
	UFUNCTION()
	void UpdateControlSettings(const FName& NewPresetName);

	UFUNCTION()
	virtual void UpdateInteractionMode(ERZ_ControllerInteractionMode NewInteractionMode);
	
private:

	UFUNCTION()
	void Debug(float DeltaTime);
	
	/// Player traces
	/// Networking WIP.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
protected:

	UFUNCTION()
	void UpdateCursorTraces(float DeltaTime);
	
	UFUNCTION()
	void UpdateCrosshairTrace(float TraceStartHeight, float DeltaTime);

	UFUNCTION()
	void UpdateHoveredActor(float DeltaTime);

	UFUNCTION()
	void SetTargetLocation(const FVector& NewTargetLocation);

	UFUNCTION(Server, Reliable)
	void SetTargetLocation_Server(const FVector& NewTargetLocation);
	
	//

	TWeakObjectPtr<AActor> LastHoveredActor;

	FHitResult CursorToWorldHitResult;
	FHitResult CursorToGroundHitResult;
	FHitResult CursorToViewPlaneHitResult;
	FHitResult CrosshairToWorldHitResult;
	
	UPROPERTY()
	FVector TargetLocation;

	/// UI
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////	


public:

	virtual void OnMenuOpened(bool bNewIsVisible);
	
	UFUNCTION(Client, Reliable)
	void OnDamageDealt_Client(float Amount, const FVector& Location);
	
protected:
	
	ARZ_UIManager* UIManager;
	URZ_ActorManager_MainWidget* ActorManagerWidget;
	URZ_InventoryMenuWidget* InventoryMenuWidget;
	URZ_InventoryHUDWidget* InventoryHUDWidget;
	URZ_HUDLayoutWidget* HUDLayoutWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FName, TSubclassOf<UUserWidget>> MenuWidgets; // ?
	
	/// Input
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

public:
	
	virtual void SetupInputComponent() override;

protected:
	
	virtual void LookUpAxis(float AxisValue);
	virtual void LookRightAxis(float AxisValue);
	virtual void OnLookUpKeyPressed();
	virtual void OnLookDownKeyPressed();
	virtual void OnLookRightKeyPressed();
	virtual void OnLookLeftKeyPressed();
	void OnZoomInKeyPressed();
	void OnZoomOutKeyPressed();
	virtual void OnToggleViewKeyPressed();
	
	//
	
	void MoveForwardAxis(float AxisValue);
	void MoveRightAxis(float AxisValue);
	void OnRunKeyPressed();
	void OnRunKeyReleased();
	void OnJumpKeyPressed();

	//

	virtual void OnRKeyPressed();
	
	virtual void OnLeftMouseButtonPressed();
	virtual void OnLeftMouseButtonReleased();
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
	void OnQuickSlot7KeyPressed();
	void OnQuickSlot8KeyPressed();
	void OnQuickSlot9KeyPressed();
	void OnHolsterKeyPressed();
	void OnQuickBarUpKeyPressed();
	void OnQuickBarDownKeyPressed();

	/// Custom console commands
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:

	UFUNCTION(Exec)
	void SpawnAIController();

	UFUNCTION(Exec)
	void AddInventoryItem(const FName& ItemName);

};

