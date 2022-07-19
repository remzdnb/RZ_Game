/// RemzDNB
///
///	RZ_UIManager.h
///	Basic interface system, handles two different layouts for both menu and HUD widgets.
///	Menu widgets are added in a widget switcher while HUD widgets are directly added to screen.
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RZ_UIManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNewMenuWidgetSelectedDelegate,
                                             const FName&, WidgetName,
                                             UUserWidget*, SelectedWidget);

UCLASS()
class RZ_UISYSTEM_API ARZ_UIManager : public AHUD
{
	GENERATED_BODY()

public:

	ARZ_UIManager();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	///
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FNewMenuWidgetSelectedDelegate OnNewMenuWidgetSelected;

	///
	
	UFUNCTION(BlueprintCallable)
	void ToggleMenu(bool bNewIsOpen);

	UFUNCTION(BlueprintCallable)
	void ToggleHUD(bool bNewIsOpen);
	
	UFUNCTION(BlueprintCallable)
	UUserWidget* CreateMenuWidget(TSubclassOf<UUserWidget> WidgetClass, const FName& WidgetName);

	UFUNCTION(BlueprintCallable)
	UUserWidget* CreateHUDWidget(TSubclassOf<UUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable)
	void AddHUDWidget(UUserWidget* WidgetToAdd, bool bSaveReference = true);

	///
	
	FORCEINLINE bool IsMenuOpen() const { return bIsMenuOpen; }
	FORCEINLINE bool IsHUDOpen() const { return bIsHUDOpen; }
	FORCEINLINE class URZ_MenuLayoutWidget* GetMenuLayoutWidget() const { return MenuLayoutWidget; }
	
private:

	const class URZ_UISystemSettings* UISettings;
	
	///
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsMenuOpen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsHUDOpen;
	
	UPROPERTY()
	URZ_MenuLayoutWidget* MenuLayoutWidget;

	UPROPERTY()
	TArray<UUserWidget*> HUDWidgets;

	//

	// Example usage GetEnumValueAsString<EVictoryEnum>("EVictoryEnum", VictoryEnum))); 

	template<typename TEnum>
	static FORCEINLINE FString GetEnumValueAsString(const FString& Name, TEnum Value) {
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, *Name, true);
		if (!enumPtr) return FString("Invalid");
		return enumPtr->GetNameByValue((int64)Value).ToString();
	}
};