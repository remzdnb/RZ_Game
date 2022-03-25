/// RemzDNB
///
///	RZ_LoadoutMenuWidget.h
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RZ_LoadoutMenuWidget.generated.h"

class URZ_ItemActorModuleSettings;
class URZ_ItemManagerModuleSettings;
class URZ_ItemUIModuleSettings;
class URZ_ItemManagerComponent;
class ARZ_ItemRenderer;

UCLASS()
class RZM_ITEMUI_API URZ_LoadoutMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override; // Called once when created.
	virtual void NativeConstruct() override; // Called when added to viewport manually or by a WidgetSwitcher.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; // Called every frame.

	//

	UFUNCTION()
	void OnNewItemManagerComponent(URZ_ItemManagerComponent* NewItemManagerComp);
	
private:

	UFUNCTION()
	void UpdateItemActorsWidgets(uint8 TabIndex);

	//
	
	URZ_ItemActorModuleSettings* ItemActorModuleSettings;
	URZ_ItemManagerModuleSettings* ItemManagerModuleSettings;
	URZ_ItemUIModuleSettings* ItemUIModuleSettings;
	URZ_ItemManagerComponent* ItemManagerComp;
	ARZ_ItemRenderer* ItemRenderer;

	//
	
	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ItemSlotsContainer;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ItemActorsContainer;

};



