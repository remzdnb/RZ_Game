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
#include "RZ_AttributeComponent.h"
#include "UI/RZ_ActorManager_GridWidget.h"
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

	UFUNCTION()
	void UpdateControlSettings(const FName& NewPresetName);

	//

	FControllerInteractionModeUpdatedDelegate OnControllerInteractionModeUpdated;
	
protected:

	ARZ_GameMode* GameMode;
	ARZ_GameState* GameState;
	URZ_GameSettings* GameSettings;
	ARZ_WorldSettings* WorldSettings;
	ARZ_CameraManager* CameraManager;

	TWeakObjectPtr<ARZ_Character> PossessedCharacter;

private:
	
	UFUNCTION()
	void OnCharacterDamaged(const FRZ_DamageInfo& DamageInfo);

	UFUNCTION()
	void OnCharacterEquippedItem(AActor* EquippedItem);

	UFUNCTION()
	void Debug(float DeltaTime);

	//

	FName ControlSettingsPresetName;
	FRZ_ControlSettings ControlSettings;
	
	/// Interaction
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	
	UFUNCTION()
	virtual void UpdateInteractionMode(ERZ_ControllerInteractionMode NewInteractionMode);

protected:

	ERZ_ControllerInteractionMode InteractionMode;
	
	/// Player traces
	/// Networking WIP.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
protected:

	UFUNCTION()
	void UpdateCursorTraces(float DeltaTime);
	
	UFUNCTION()
	void UpdateCrosshairTrace(float TraceStartHeight, float DeltaTime);

	UFUNCTION()
	void UpdateHoveredItem(float DeltaTime);

	UFUNCTION()
	void SetTargetLocation(const FVector& NewTargetLocation);

	UFUNCTION(Server, Reliable)
	void SetTargetLocation_Server(const FVector& NewTargetLocation);
	
	//

	IRZ_ActorInterface* LastHoveredItemInterface;

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

