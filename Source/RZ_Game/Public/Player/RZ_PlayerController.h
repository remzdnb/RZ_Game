/// RemzDNB
///
///	Base PlayerController class.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "RZ_GameTypes.h"
#include "GameFramework/PlayerController.h"
#include "RZ_PlayerController.generated.h"

class ARZ_GameMode;
class ARZ_GameState;
class URZ_GameSettings;
class ARZ_WorldSettings;
class ARZ_CameraManager;
class ARZ_UIManager;
class URZ_SettingsWidget;
class ARZ_Character;
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
	
	const FName& GetPlayerConfigPresetName() const { return ControlSettingsPresetName; }
	
	//
	
	ARZ_GameMode* GameMode;
	ARZ_GameState* GameState;
	URZ_GameSettings* GameSettings;
	ARZ_WorldSettings* WorldSettings;
	ARZ_CameraManager* CameraManager;
	FName ControlSettingsPresetName;
	FRZ_ControlSettings ControlSettings;
	TWeakObjectPtr<ARZ_Character> PossessedCharacter;

	///// Targeting
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

protected:

	FRotator TargetControlRotation; // lerped control ?
	
private:

	UPROPERTY()
	FVector TargetLocation;

	UFUNCTION()
	void UpdateTargetLocation(const FVector& NewTargetLocation);

	UFUNCTION(Server, Reliable)
	void UpdateTargetLocation_Server(const FVector& NewTargetLocation);

	UFUNCTION()
	void UpdateTargetFromCursor();
	
	UFUNCTION() // Running on both server and autonomous proxies, server result gets replicated to simulated proxies. ?
	void UpdateTargetFromScreenCenter();

	
	///// UI
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FName, TSubclassOf<UUserWidget>> MenuWidgets;
	
protected:

	ARZ_UIManager* UIManager;
	URZ_SettingsWidget* SettingsWidget;
	
	///// Input
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

public:
	
	virtual void SetupInputComponent() override;

protected:
	
	virtual void LookUpAxis(float AxisValue);
	virtual void LookRightAxis(float AxisValue);
	virtual void CameraZoomAxis(float AxisValue);
	virtual void CameraRotationAxis(float AxisValue);
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

	void OnCameraRotateRightKeyPressed();
	void OnCameraRotateLeftKeyPressed();

	void OnQuickSlot1Pressed();
	void OnQuickSlot2Pressed();
	void OnQuickSlot3Pressed();
	void OnQuickSlot4Pressed();
	void OnQuickSlot5Pressed();

	///// Custom console commands
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	private:

	UFUNCTION(Exec)
	void SpawnAIController();
	
	///// Wth
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:

	// Example usage GetEnumValueAsString<EVictoryEnum>("EVictoryEnum", VictoryEnum))); 

	template<typename TEnum>
	static FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value) {
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
		if (!enumPtr) return FString("Invalid");
		return enumPtr->GetNameByValue((int64)Value).ToString();
	}
};
