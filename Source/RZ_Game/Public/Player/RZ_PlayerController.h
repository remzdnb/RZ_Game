/// RemzDNB
///
///	Base PlayerController class.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "RZ_GameTypes.h"
#include "GameFramework/PlayerController.h"
#include "Pawn/RZ_PawnCombatComponent.h"
#include "RZ_PlayerController.generated.h"

class ARZ_GameMode;
class ARZ_GameState;
class URZ_GameSettings;
class ARZ_WorldSettings;
class ARZ_CameraManager;
class ARZ_UIManager;
class URZ_SettingsWidget;
class ARZ_Character;
//
class URZ_LoadoutMenuWidget;
class URZ_LoadoutHUDWidget;
//
class UDataTable;

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
	
private:

	UFUNCTION()
	void OnCharacterDamaged(const FRZ_DamageInfo& DamageInfo);

	//
	
	ARZ_GameMode* GameMode;
	ARZ_GameState* GameState;
	URZ_GameSettings* GameSettings;
	ARZ_WorldSettings* WorldSettings;
	
	TWeakObjectPtr<ARZ_CameraManager> CameraManager;
	TWeakObjectPtr<ARZ_Character> PossessedCharacter;

	FName ControlSettingsPresetName;
	FRZ_ControlSettings ControlSettings;
	
	/// Targeting
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
private:

	UPROPERTY()
	FVector TargetLocation;

	UFUNCTION()
	void SetTargetLocation(const FVector& NewTargetLocation);

	UFUNCTION(Server, Reliable)
	void SetTargetLocation_Server(const FVector& NewTargetLocation);

	UFUNCTION()
	FVector UpdateTargetFromCursor();
	
	UFUNCTION() // Running on both server and autonomous proxies, server result gets replicated to simulated proxies. ?
	void UpdateTargetFromScreenCenter();

	
	/// UI
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////	


public:
	
	UFUNCTION(Client, Reliable)
	void OnDamageDealt_Client(float Amount, const FVector& Location);
	
private:

	//

	ARZ_UIManager* UIManager;
	URZ_SettingsWidget* SettingsWidget;
	URZ_LoadoutMenuWidget* LoadoutMenuWidget;
	URZ_LoadoutHUDWidget* LoadoutHUDWidget;

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
	
	virtual void MoveForwardAxis(float AxisValue);
	virtual void MoveRightAxis(float AxisValue);

	
	virtual void OnLeftMouseButtonPressed();
	virtual void OnLeftMouseButtonReleased();
	void OnRightMouseButtonPressed();
	void OnRightMouseButtonReleased();
	void OnMiddleMouseButtonPressed();
	void OnMiddleMouseButtonReleased();
	virtual void OnUseKeyPressed();
	virtual void OnTabKeyPressed();
	void OnShiftKeyPressed();
	virtual void OnSpaceBarKeyPressed();

	void OnQuickSlot1Pressed();
	void OnQuickSlot2Pressed();
	void OnQuickSlot3Pressed();
	void OnQuickSlot4Pressed();
	void OnQuickSlot5Pressed();

	/// Custom console commands
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	private:

	UFUNCTION(Exec)
	void SpawnAIController();
};
